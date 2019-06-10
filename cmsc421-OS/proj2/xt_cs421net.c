/*
 * Raise an interrupt upon each target skb, storing it for later
 * processing.
 *
 * Copyright(c) 2016-2018 Jason Tang <jtang@umbc.edu>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#define pr_fmt(fmt) "CS421Net: " fmt

#include <linux/completion.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/spinlock.h>
#include <linux/workqueue.h>

#include <linux/netfilter.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter/xt_LOG.h>

#include <linux/ip.h>
#include <linux/tcp.h>

/* defined in arch/x86/kernel/irq.c */
extern int trigger_irq(unsigned);

#define CS421NET_IRQ 6
static DEFINE_SPINLOCK(lock);
static DECLARE_COMPLETION(retrieved);
static bool cs421net_enabled;
struct workqueue_struct *cs421net_wq;

struct incoming_data {
	char *data;
	size_t len;
	struct list_head list;
};
static LIST_HEAD(cs421net_list);

/**
 * cs421net_enable() - start capturing data from the network
 *
 * This device will now raise interrupts when data arrive.
 */
void cs421net_enable(void)
{
	cs421net_enabled = true;
	reinit_completion(&retrieved);
}

EXPORT_SYMBOL(cs421net_enable);

/**
 * cs421net_disable() - stop capturing data from the network
 *
 * This device will stop raising interrupts when data arrive.
 */
void cs421net_disable(void)
{
	cs421net_enabled = false;
	if (!completion_done(&retrieved))
		complete_all(&retrieved);
}

EXPORT_SYMBOL(cs421net_disable);

/**
 * cs421net_get_data() - retrieve the oldest pending data
 *
 * This function is safe to be called from within interrupt context.
 *
 * If all data have been retrieved, then this function returns
 * NULL. Otherwise, it returns the oldest data.
 *
 * WARNING: The returned buffer is NOT A STRING; it is not necessarily
 * null-terminated.
 *
 * @len: out parameter to store data length
 * Return: allocated data buffer (caller is responsible for freeing
 * it), or %NULL if none pending
 */
char *cs421net_get_data(size_t * const len)
{
	struct incoming_data *oldest;
	unsigned long flags;
	void *payload;

	spin_lock_irqsave(&lock, flags);
	oldest =
	    list_first_entry_or_null(&cs421net_list, struct incoming_data,
				     list);
	if (!oldest) {
		spin_unlock_irqrestore(&lock, flags);
		*len = 0;
		return NULL;
	}
	payload = oldest->data;
	*len = oldest->len;
	list_del(&oldest->list);
	kfree(oldest);
	spin_unlock_irqrestore(&lock, flags);

	complete(&retrieved);
	return payload;
}

EXPORT_SYMBOL(cs421net_get_data);

/**
 * cs421net_work_func() - function invoked by the workqueue
 *
 * Raise an interrupt, then wait for the interrupt handler to
 * acknowledge the interrupt.
 */
static void cs421net_work_func(struct work_struct *work)
{
	pr_info("Raising interrupt %u\n", CS421NET_IRQ);
	if (trigger_irq(CS421NET_IRQ) < 0)
		pr_err("Could not generate interrupt\n");
}

static DECLARE_WORK(cs421net_work, cs421net_work_func);

/**
 * filter_tg() - perform packet mangling
 *
 * For each skb, if it is a TCP packet then add it to the payload list
 * and schedule an interrupt.
 */
static unsigned int
filter_tg(struct sk_buff *skb, const struct xt_action_param *par)
{
	struct iphdr *iph = ip_hdr(skb);
	u8 hdr_len;
	struct incoming_data *data;
	unsigned long flags;

	if (!cs421net_enabled || !iph || iph->protocol != IPPROTO_TCP)
		return XT_CONTINUE;
	hdr_len = skb_transport_offset(skb) + tcp_hdrlen(skb);
	if (skb->len > hdr_len) {
		data = kmalloc(sizeof(*data), GFP_ATOMIC);
		if (!data)
			goto out;
		data->len = skb->len - hdr_len;
		data->data = kmalloc(data->len, GFP_ATOMIC);
		if (!data->data) {
			kfree(data);
			goto out;
		}
		memcpy(data->data, skb->data + hdr_len, data->len);
		spin_lock_irqsave(&lock, flags);
		list_add_tail(&data->list, &cs421net_list);
		spin_unlock_irqrestore(&lock, flags);
		queue_work(cs421net_wq, &cs421net_work);
	}
out:
	return XT_CONTINUE;
}

static struct xt_target filter_tg_reg __read_mostly = {
	.name = "LOG",
	.family = NFPROTO_IPV4,
	.target = filter_tg,
	.targetsize = sizeof(struct xt_log_info),
	.table = "mangle",
	.me = THIS_MODULE,
};

static int __init cs421net_init(void)
{
	int retval;

	cs421net_wq = create_singlethread_workqueue("CS421Net");
	if (!cs421net_wq) {
		retval = -ENOMEM;
		goto out;
	}
	retval = xt_register_target(&filter_tg_reg);
	if (retval < 0)
		destroy_workqueue(cs421net_wq);
out:
	pr_info("initialization returning %d\n", retval);
	return retval;
}

static void __exit cs421net_exit(void)
{
	struct incoming_data *data, *tmp;

	cs421net_enabled = false;
	xt_unregister_target(&filter_tg_reg);
	complete_all(&retrieved);
	cancel_work_sync(&cs421net_work);
	destroy_workqueue(cs421net_wq);
	list_for_each_entry_safe(data, tmp, &cs421net_list, list) {
		kfree(data->data);
		kfree(data);
	}
	pr_info("exited\n");
}

module_init(cs421net_init);
module_exit(cs421net_exit);

MODULE_DESCRIPTION("CS421 Virtual Network");
MODULE_LICENSE("GPL");

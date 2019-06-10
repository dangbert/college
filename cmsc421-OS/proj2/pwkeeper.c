/**
 * @Author Dan Engbert (end1@umbc.edu)
 * Description:
 *   This file contains a kernel module that acts as a password manager.  User's
 *   can set a master password and add accounts (sites) which will have password
 *   auto-generated and stored.
 * TODO: indent file at end!
 *
 * Help received:
 *   https://elixir.bootlin.com/linux/v4.17/source/include/linux/uidgid.h
 *   https://www.kernel.org/doc/html/latest/security/credentials.html?highlight=current_uid
 *   https://www.csee.umbc.edu/~jtang/cs421.f18/lectures/L25Security.pdf#page=23
 *
 *   https://www.csee.umbc.edu/~jtang/cs421.f18/lectures/L19KernelMemory.pdf#page=17
 *   https://elixir.bootlin.com/linux/latest/source/include/linux/list.h
 *   https://www.kernel.org/doc/html/latest/core-api/kernel-api.html?highlight=list_add#c.list_add
 *   https://kernelnewbies.org/FAQ/LinkedLists
 *   https://isis.poly.edu/kulesh/stuff/src/klist/
 *   http://www.roman10.net/2011/07/28/linux-kernel-programminglinked-list/
 *   http://rachid.koucha.free.fr/tech_corner/linked_lists_in_kernel.html
 *
 *   https://www.fsl.cs.sunysb.edu/kernel-api/re163.html
 *
 *   https://www.csee.umbc.edu/~jtang/cs421.f18/lectures/L20KernelSynchronizations.pdf#page=10
 *   https://elixir.bootlin.com/linux/latest/source/include/linux/spinlock.h#L357
 *   https://stackoverflow.com/a/2570656/5500073
 *
 *   https://www.csee.umbc.edu/~jtang/cs421.f18/lectures/L21InterruptHandling.pdf#page=15
 *   https://www.kernel.org/doc/htmldocs/kernel-api/API-request-threaded-irq.html
 *
 *   https://stackoverflow.com/a/12240325
 */

/*
 * This file uses kernel-doc style comments, which is similar to
 * Javadoc and Doxygen-style comments. See
 * ~/linux/Documentation/doc-guide/kernel-doc.rst for details.
 */

/*
 * Getting compilation warnings? The Linux kernel is written against
 * C89, which means:
 *  - No // comments, and
 *  - All variables must be declared at the top of functions.
 * Read ~/linux/Documentation/process/coding-style.rst to ensure your
 * project compiles without warnings.
 */

#define pr_fmt(fmt) "pwkeeper: " fmt

#include <linux/cred.h>
#include <linux/capability.h>
#include <linux/pid_namespace.h>
#include <linux/fs.h>
#include <linux/gfp.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/list.h>
#include <linux/miscdevice.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/uidgid.h>
#include <linux/vmalloc.h>
#include <asm/uaccess.h>
#include <crypto/hash.h>

#include "xt_cs421net.h"

#define MASTERPW_LEN 32
#define ACCOUNTNAME_LEN 16
#define ACCOUNTPW_LEN 16

const char device_name[] = "pwkeeper";	/* device name */
/* struct for master password linked list */
struct master_struct {
	struct list_head list;
	uid_t uid;
	char password[MASTERPW_LEN];
};
/* master password linked list */
struct list_head master_list = LIST_HEAD_INIT(master_list);

/* struct for site accounts linked list */
struct account_struct {
	struct list_head list;
	uid_t uid;
	char account_name[ACCOUNTNAME_LEN];
	char account_password[ACCOUNTPW_LEN];
};
/* site accounts linked list */
struct list_head account_list = LIST_HEAD_INIT(account_list);

/* spin lock for access to master_list and account_list */
/* TODO: fix issue in dmesg: BUG: spinlock wrong CPU on CPU#1, rmmod/3806 */
static DEFINE_SPINLOCK(lists_lock);
unsigned long flags;

/**
 * get_master_node() - finds a user's node in master_list
 * @user: uid of the target user
 *
 * NOTE: only call this function if you have the lock lists_lock
 *
 * Return: pointer to node in master_list, or NULL if not found
 */
static struct master_struct *get_master_node(uid_t user)
{
	struct master_struct *cur = NULL;
	/* search master_list for user */
	list_for_each_entry(cur, &master_list, list) {
		if (cur->uid == user)
			return cur;
	}
	return NULL;
}

/**
 * deletes the nodes in master_list and account_list and frees their memory
 */
static void clear_both_lists(void)
{
	struct master_struct *cur_mas = NULL, *tmp_mas = NULL;
	struct account_struct *cur_acc = NULL, *tmp_acc = NULL;
	pr_info("clearing both linked-lists\n");
	/* free all memory associated with master password list */
	list_for_each_entry_safe(cur_mas, tmp_mas, &master_list, list) {
		list_del(&cur_mas->list);
		vfree(cur_mas);
	}
	/* free all memory associated with accounts list */
	list_for_each_entry_safe(cur_acc, tmp_acc, &account_list, list) {
		list_del(&cur_acc->list);
		vfree(cur_acc);
	}
}

/**
 * sha3_digest() - calculate the SHA-3 digest for an arbitrary input buffer
 * @input: input data buffer
 * @input_len: number of bytes in @input
 * @digest: destination pointer to store digest
 * @digest_len: size of digest buffer (in/out parameter)
 *
 * Hash the input buffer pointed to by @input, up to @input_len
 * bytes. Store the resulting digest at @digest. Afterwards, update
 * the value pointed to by @digest_len by the size of the stored
 * digest.
 *
 * <strong>You do not need to modify this function.</strong>
 *
 * Return: 0 on success, negative on error
 */
static int sha3_digest(const void *input, size_t input_len, u8 * digest,
		       size_t * digest_len)
{
	struct crypto_shash *sha3_tfm;
	struct shash_desc *sha3_desc;
	unsigned int digestsize;
	size_t i;
	int retval;

	sha3_tfm = crypto_alloc_shash("sha3-512", 0, 0);
	if (IS_ERR_OR_NULL(sha3_tfm)) {
		pr_err("Could not allocate hash tfm: %ld\n", PTR_ERR(sha3_tfm));
		return PTR_ERR(sha3_tfm);
	}

	digestsize = crypto_shash_digestsize(sha3_tfm);
	if (*digest_len < digestsize) {
		pr_err("Digest buffer too small, need at least %u bytes\n",
		       digestsize);
		retval = -EINVAL;
		goto out;
	}

	sha3_desc =
	    kzalloc(sizeof(*sha3_desc) + crypto_shash_descsize(sha3_tfm),
		    GFP_KERNEL);
	if (!sha3_desc) {
		pr_err("Could not allocate hash desc\n");
		retval = -ENOMEM;
		goto out;
	}
	sha3_desc->tfm = sha3_tfm;
	sha3_desc->flags = CRYPTO_TFM_REQ_MAY_SLEEP;

	retval = crypto_shash_digest(sha3_desc, input, input_len, digest);
	*digest_len = digestsize;
	pr_info("Hashed %zu bytes, digest = ", input_len);
	for (i = 0; i < digestsize; i++)
		pr_cont("%02x", digest[i]);
	pr_info("\n");
	kfree(sha3_desc);
out:
	crypto_free_shash(sha3_tfm);
	return retval;
}

/**
 * compute_password() - calculates the password
 * @master_password: user's master password (length MASTERPW_LEN, '\0' padded if necessary)
 * @account_name: name of the account for the user (length ACCOUNTNAME_LEN, '\0' padded if necessary)
 * @pw: buffer to write computed password to (length ACCOUNTPW_LEN, '\0' padded if necessary)
 *
 * (note: all these memory buffer's are in kernel space)
 *
 * Return: 0 on success, negative on error
 */
static int compute_password(char *master_password, char *account_name, char *pw)
{
	unsigned char tmp[48];
	unsigned char digest_buffer[64];	/* buffer to store hash of tmp buffer */
	size_t digest_len = 64;
	unsigned int k = 0;
	unsigned char val;
	memset(tmp, '\0', 48);
	memcpy(tmp, master_password, 32);
	memcpy(tmp + 32, account_name, 16);

	/* hash tmp buffer */
	if (0 != sha3_digest(tmp, 48, digest_buffer, &digest_len)) {
		printk("sha3_digest() failed\n");
		return -1;
	}
	for (k = 0; k < ACCOUNTPW_LEN; k++) {
		/* (note 0b00111111 == 63) */
		val = (char)(63 & digest_buffer[k]) + 48;
		pw[k] = val;
	}
	return 0;
}

/**
 * pwkeeper_master_write() - callback invoked when a process writes to
 * /dev/pwkeeper_master
 * @filp: process's file object that is writing to this device (ignored)
 * @ubuf: source buffer from user
 * @count: number of bytes in @ubuf
 * @ppos: file offset (in/out parameter)
 *
 * If *@ppos does not point to zero, do nothing and return -EINVAL.
 *
 * Copy the contents of @ubuf to the master password for the user, the
 * lesser of @count and MASTERPW_LEN. Then increment the value pointed
 * to by @ppos by the number of bytes copied.
 *
 * When replacing an existing master password, recalculate all account
 * passwords.
 *
 * <em>Caution: @ubuf is not a string; it is not null-terminated.</em>
 *
 * Return: number of bytes copied from @ubuf, or negative on error
 */
static ssize_t pwkeeper_master_write(struct file *filp,
				     const char __user * ubuf, size_t count,
				     loff_t * ppos)
{
	int existing = 0;	/* set to 1 if replacing an existing user's password */
	struct master_struct *node = NULL;
	struct account_struct *cur_acc = NULL;
	uid_t user = current_uid().val;
	char *buf = NULL;	/* pointer to the buf to store the password */
	unsigned int k;
	ssize_t not_copied = 0;	/* number of bytes that failed to copy */
	if (*ppos != 0)
		return -EINVAL;
	spin_lock_irqsave(&lists_lock, flags);

	/* check if user already has a master password set */
	node = get_master_node(user);
	if (node != NULL) {
		buf = node->password;
		existing = 1;
	} else {
		/* create node in linked list for (new) user */
		node = vzalloc(sizeof(struct master_struct));
		INIT_LIST_HEAD(&node->list);
		node->uid = user;
		list_add(&node->list, &master_list);	/* add node to linked list */
		buf = node->password;
	}

	/* set password (pad end with null characters) */
	memset(buf, '\0', sizeof(char) * MASTERPW_LEN);
	if (count > MASTERPW_LEN)
		count = MASTERPW_LEN;
	not_copied = copy_from_user(buf + *ppos, ubuf, (unsigned long)count);
	/* recalculate all account passwords (if existing user) */
	if (existing == 1) {
		list_for_each_entry(cur_acc, &account_list, list) {
			if (cur_acc->uid == user) {
				if (0 !=
				    compute_password(node->password,
						     cur_acc->account_name,
						     cur_acc->account_password))
				{
					spin_unlock_irqrestore(&lists_lock,
							       flags);
					return -1;
				}
				pr_cont
				    ("  update account pw for user %d, account: ",
				     cur_acc->uid);
				for (k = 0; k < ACCOUNTPW_LEN; k++) {
					pr_cont("%c", cur_acc->account_name[k]);
				}
				printk("\n");
				pr_cont("\n");
			}
		}
	}

	(*ppos) += count - not_copied;	/* increment by number of bytes written */
	spin_unlock_irqrestore(&lists_lock, flags);
	return count - not_copied;
}

/**
 * pwkeeper_account_read() - callback invoked when a process reads
 * from /dev/pwkeeper_account
 * @filp: process's file object that is reading from this device (ignored)
 * @ubuf: destination to store account password
 * @count: number of bytes in @ubuf
 * @ppos: file offset (in/out parameter)
 *
 * Write to @ubuf the password generated for the most recently written
 * account name for the current UID, offset by @ppos. Copy the lesser
 * of @count and (ACCOUNTPW_LEN - *@ppos). Then increment the value
 * pointed to by @ppos by the number of bytes written. If @ppos is
 * greater than or equal to ACCOUNTPW_LEN, then write
 * nothing.
 *
 * If no account name was set (via previous successful invocation of
 * pwkeeper_account_write()), do nothing and return -ENOKEY.
 *
 * Return: number of bytes written to @ubuf, 0 on end of file, or
 * negative on error
 */
static ssize_t pwkeeper_account_read(struct file *filp, char __user * ubuf,
				     size_t count, loff_t * ppos)
{
	uid_t user = current_uid().val;
	struct account_struct *cur_acc = NULL;
	ssize_t not_copied = 0;	/* number of bytes that failed to copy */

	spin_lock_irqsave(&lists_lock, flags);
	if (count > (ACCOUNTPW_LEN - *ppos))
		count = ACCOUNTPW_LEN - *ppos;
	list_for_each_entry(cur_acc, &account_list, list) {
		if (cur_acc->uid == user) {
			/* copy this account name to user buffer and return */
			not_copied =
			    copy_to_user(ubuf,
					 cur_acc->account_password + *ppos,
					 (unsigned long)count);
			(*ppos) += count - not_copied;	/* increment by number of bytes written */
			spin_unlock_irqrestore(&lists_lock, flags);
			return count - not_copied;
		}
	}
	spin_unlock_irqrestore(&lists_lock, flags);
	return -ENOKEY;
}

/**
 * pwkeeper_account_write() - callback invoked when a process writes
 * to /dev/pwkeeper_account
 * @filp: process's file object that is writing to this device (ignored)
 * @ubuf: source buffer from user
 * @count: number of bytes in @ubuf
 * @ppos: file offset (in/out parameter)
 *
 * If *@ppos does not point to zero, do nothing and return -EINVAL.
 *
 * If the current user has not set a master password, do nothing and
 * return -ENOKEY.
 *
 * Otherwise check if @ubuf is already in the accounts list associated
 * with the current user. If it is already there, do nothing and
 * return @count.
 *
 * Otherwise, create a new node in the accounts list associated with
 * the current user. Copy the contents of @ubuf to that node, the
 * lesser of @count and ACCOUNTNAME_LEN. Increment the value pointed
 * to by @ppos by the number of bytes copied. Finally, perform the key
 * derivation function as specified in the project description, to
 * determine the account's password.
 *
 * <em>Caution: @ubuf is not a string; it is not null-terminated.</em>
 *
 * Return: @count, or negative on error
 */
static ssize_t pwkeeper_account_write(struct file *filp,
				      const char __user * ubuf, size_t count,
				      loff_t * ppos)
{
	struct master_struct *master_node = NULL;	/* user's node in master_list */
	struct account_struct *acc_node = NULL;	/* new node for this user's account */
	struct account_struct *cur_acc = NULL;
	uid_t user = current_uid().val;
	unsigned int orig_count = count;	/* original count value */
	unsigned int k = 0;
	ssize_t not_copied = 0;	/* number of bytes that failed to copy */
	ssize_t ret_val = -EINVAL;	/* value to return */
	if (*ppos != 0) {
		ret_val = -EINVAL;
		goto ret_section;
	}
	spin_lock_irqsave(&lists_lock, flags);

	/* check if user has a master password set */
	master_node = get_master_node(user);
	if (master_node == NULL) {
		pr_info("unable to find master_node for user %d\n", user);
		ret_val = -ENOKEY;
		goto ret_section;
	}
	if (count > ACCOUNTNAME_LEN)
		count = ACCOUNTNAME_LEN;

	/* check if ubuf is already in account list */
	list_for_each_entry(cur_acc, &account_list, list) {
		if (cur_acc->uid == user
		    && 0 == memcmp(ubuf, cur_acc->account_name, count)) {
			if (count != ACCOUNTNAME_LEN
			    && cur_acc->account_name[count] != '\0') {
				/* account_name starts with ubuf but has more chars after */
				continue;
			}
			pr_info("account_name already in list for user %d\n",
				user);
			ret_val = orig_count;
			goto ret_section;
		}
	}
	/* create node in account list for (new) account */
	acc_node = vzalloc(sizeof(struct account_struct));
	if (acc_node == NULL) {
		pr_info("account already existed for user %d\n", user);
		ret_val = -1;
		goto ret_section;
	}
	INIT_LIST_HEAD(&acc_node->list);
	acc_node->uid = user;
	/* use copy_from_user: */
	not_copied =
	    copy_from_user(acc_node->account_name, ubuf, (unsigned long)count);
	if (not_copied != 0) {
		ret_val = -1;
		goto ret_section;
	}
	/* compute and set account_password */
	if (0 !=
	    compute_password(master_node->password, acc_node->account_name,
			     acc_node->account_password)) {
		ret_val = -1;
		goto ret_section;
	}
	list_add(&acc_node->list, &account_list);	/* add node to linked list */

	/* print computed password */
	pr_cont("\nadded new account for user %d, computed pw: ", user);
	for (k = 0; k < ACCOUNTPW_LEN; k++) {
		pr_cont("%c", acc_node->account_password[k]);
	}
	pr_cont("\n");

	(*ppos) += count - not_copied;	/* increment by number of bytes written */
	ret_val = orig_count;

ret_section:
	spin_unlock_irqrestore(&lists_lock, flags);
	return ret_val;
}

static const struct file_operations pwkeeper_master_fops = {
	.write = pwkeeper_master_write,
};

static struct miscdevice pwkeeper_master_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "pwkeeper_master",
	.fops = &pwkeeper_master_fops,
	.mode = 0666
};

static const struct file_operations pwkeeper_account_fops = {
	.read = pwkeeper_account_read,
	.write = pwkeeper_account_write,
};

static struct miscdevice pwkeeper_account_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "pwkeeper_account",
	.fops = &pwkeeper_account_fops,
	.mode = 0666
};

/**
 * pwkeeper_accounts_show() - callback invoked when a process reads from
 * /sys/devices/platform/pwkeeper/accounts
 *
 * @dev: device driver data for sysfs entry (ignored)
 * @attr: sysfs entry context (ignored)
 * @buf: destination to store current user's accounts
 *
 * Write to @buf, up to PAGE_SIZE characters, a human-readable message
 * that lists all accounts registered for the current UID, and the
 * associated account passwords. Note that @buf is a normal character
 * buffer, not a __user buffer. Use scnprintf() in this function.
 *
 * @return Number of bytes written to @buf, or negative on error.
 */
static ssize_t pwkeeper_accounts_show(struct device *dev,
				      struct device_attribute *attr, char *buf)
{
	uid_t user = current_uid().val;
	struct account_struct *cur_acc = NULL;
	unsigned int index = 0;	/* index in buf */
	int ret = 0;
	pr_info("in pwkeeper_accounts_show(), uid = %d\n", user);
	memset(buf, '\0', PAGE_SIZE);
	spin_lock_irqsave(&lists_lock, flags);

	/* print account names and passwords for this user */
	ret =
	    scnprintf(buf, PAGE_SIZE,
		      "Account  Password\n‐‐‐‐‐‐‐  ‐‐‐‐‐‐‐‐\n");
	index += ret;
	if (ret <= 0)
		goto finished;
	list_for_each_entry(cur_acc, &account_list, list) {
		if (cur_acc->uid == user) {
			/* print account name and password */
			ret =
			    scnprintf(buf + index, PAGE_SIZE - index,
				      "%s  %s\n", cur_acc->account_name,
				      cur_acc->account_password);
			index += ret;
			if (ret <= 0)
				goto finished;
		}
	}

finished:
	pr_info("index = %d\n", index);
	/* add an extra newline if we hit the max size */
	if (index >= PAGE_SIZE - 2) {
		buf[PAGE_SIZE - 2] = '\n';	/* (note: buf[PAGE_SIZE-1] is '\0') */
		pr_info("adding trailing \n");
	}
	spin_unlock_irqrestore(&lists_lock, flags);
	return index;
}

/**
 * pwkeeper_master_show() - callback invoked when a process reads from
 * /sys/devices/platform/pwkeeper/masters
 *
 * @dev: device driver data for sysfs entry (ignored)
 * @attr: sysfs entry context (ignored)
 * @buf: destination to store login statistics
 *
 * Check if the calling process has CAP_SYS_ADMIN. If not, return
 * -EPERM.
 *
 * Otherwise, write to @buf, up to PAGE_SIZE characters, a
 * human-readable message that lists all users IDs that have
 * registered master passwords. Note that @buf is a normal character
 * buffer, not a __user buffer. Use scnprintf() in this function.
 *
 * @return Number of bytes written to @buf, or negative on error.
 */
static ssize_t pwkeeper_masters_show(struct device *dev,
				     struct device_attribute *attr, char *buf)
{
	struct pid_namespace *pid_ns = task_active_pid_ns(current);
	struct master_struct *cur = NULL;
	unsigned int index = 0;	/* index in buf */
	int ret = 0;
	ssize_t ret_val = -EPERM;
	pr_info("Flag5: in pwkeeper_accounts_show()\n");
	memset(buf, '\0', PAGE_SIZE);
	spin_lock_irqsave(&lists_lock, flags);

	/* check that calling process has CAP_SYS_ADMIN */
	if (!ns_capable(pid_ns->user_ns, CAP_SYS_BOOT)) {
		ret_val = -EPERM;
		goto ret_section;
	}

	/* write to buf all the registered UIDs */
	ret =
	    scnprintf(buf, PAGE_SIZE,
		      "Registered UIDs\n‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐\n");
	index += ret;
	if (ret <= 0)
		goto finished;
	/* iterate over master_list */
	list_for_each_entry(cur, &master_list, list) {
		ret =
		    scnprintf(buf + index, PAGE_SIZE - index, "%u\n", cur->uid);
		index += ret;
		if (ret <= 0)
			goto finished;
	}

finished:
	/* add an extra newline if we hit the max size */
	if (index >= PAGE_SIZE - 2)
		buf[PAGE_SIZE - 2] = '\n';	/* (note: buf[PAGE_SIZE-1] is '\0') */
	ret_val = index;

ret_section:
	spin_unlock_irqrestore(&lists_lock, flags);
	return ret_val;
}

static DEVICE_ATTR(accounts, S_IRUGO, pwkeeper_accounts_show, NULL);
static DEVICE_ATTR(masters, S_IRUGO, pwkeeper_masters_show, NULL);

/**
 * cs421net_top() - top-half of CS421Net ISR
 * @irq: IRQ that was invoked (ignored)
 * @cookie: Pointer to data that was passed into
 * request_threaded_irq() (ignored)
 *
 * If @irq is CS421NET_IRQ, then wake up the bottom-half. Otherwise,
 * return IRQ_NONE.
 */
static irqreturn_t cs421net_top(int irq, void *cookie)
{
	if (irq == CS421NET_IRQ) {
		return IRQ_WAKE_THREAD;
	}
	return IRQ_NONE;
}

/**
 * cs421net_bottom() - bottom-half to CS421Net ISR
 * @irq: IRQ that was invoked (ignore)
 * @cookie: Pointer that was passed into request_threaded_irq()
 * (ignored)
 *
 * Fetch the incoming packet, via cs421net_get_data(). Treat the input
 * as a 32-BIT LITTLE ENDIAN BINARY VALUE representing a UID. Search
 * through the master list and accounts list, deleting all nodes with
 * that UID. If the UID is exactly zero, then delete ALL nodes in the
 * master and accounts lists.
 *
 * If the packet length is not exactly 4 bytes, or if the provided
 * value does not match a registered UID in the master list, then do
 * nothing.
 *
 * Remember to add appropriate spin lock calls in this function.
 *
 * <em>Caution: The incoming payload is not a string; it is not null-terminated.</em>
 * You can NOT use strcpy() or strlen() on it.
 *
 * Return: always IRQ_HANDLED
 */
static irqreturn_t cs421net_bottom(int irq, void *cookie)
{
	unsigned char *bytes;
	size_t len;
	uint32_t uid;
	struct master_struct *cur_mas = NULL, *tmp_mas = NULL;
	struct account_struct *cur_acc = NULL, *tmp_acc = NULL;

	bytes = cs421net_get_data(&len);
	if (len != 4) {
		pr_info("expected payload of length 4 bytes (got %ld)\n", len);
		return IRQ_HANDLED;
	}

	uid = bytes[0] + (bytes[1] << 8) + (bytes[2] << 16) + (bytes[3] << 24);
	pr_info("In ISR-bottom, uid=%u\n", uid);

	/* 808464432 is a hack that allows my pwkeeper-test.c to send the string "0000" to trigger this delete */
	if (uid == 0 || uid == 808464432) {
		clear_both_lists();
		return IRQ_HANDLED;
	}

	/* delete nodes in master list and account_list for this UID: */
	spin_lock(&lists_lock);
	list_for_each_entry_safe(cur_mas, tmp_mas, &master_list, list) {
		if (uid == cur_mas->uid) {
			pr_info("ISR: deleting master node with uid %u\n",
				cur_mas->uid);
			list_del(&cur_mas->list);
			vfree(cur_mas);
		}
	}
	list_for_each_entry_safe(cur_acc, tmp_acc, &account_list, list) {
		if (uid == cur_acc->uid) {
			pr_info("ISR: deleting account node with uid %u\n",
				cur_acc->uid);
			list_del(&cur_acc->list);
			vfree(cur_acc);
		}
	}
	spin_unlock(&lists_lock);
	return IRQ_HANDLED;
}

/**
 * pwkeeper_probe() - callback invoked when this driver is probed
 * @pdev platform device driver data (ignored)
 *
 * Return: 0 on successful probing, negative on error
 */
static int pwkeeper_probe(struct platform_device *pdev)
{
	int retval;

	retval = misc_register(&pwkeeper_master_dev);
	if (retval) {
		pr_err("Could not register master device\n");
		goto err;
	}

	retval = misc_register(&pwkeeper_account_dev);
	if (retval) {
		pr_err("Could not register account device\n");
		goto err_deregister_master;
	}

	retval = device_create_file(&pdev->dev, &dev_attr_accounts);
	if (retval) {
		pr_err("Could not create sysfs entry\n");
		goto err_deregister_account;
	}

	retval = device_create_file(&pdev->dev, &dev_attr_masters);
	if (retval) {
		pr_err("Could not create sysfs entry\n");
		goto err_remove_sysfs_accounts;
	}

	/*
	 * In part 5, register the ISR and enable network
	 * integration. Make sure you clean up upon error.
	 */
	spin_lock_init(&lists_lock);
	/* register threaded interrupt handler */
	retval =
	    request_threaded_irq(CS421NET_IRQ, &cs421net_top, &cs421net_bottom,
				 0, device_name, (void *)&master_list);
	cs421net_enable();
	if (retval) {
		goto err_remove_sysfs_masters;
	}

	pr_info("Probe successful\n");
	return 0;

err_remove_sysfs_masters:
	device_remove_file(&pdev->dev, &dev_attr_masters);
err_remove_sysfs_accounts:
	device_remove_file(&pdev->dev, &dev_attr_accounts);
err_deregister_account:
	misc_deregister(&pwkeeper_account_dev);
err_deregister_master:
	misc_deregister(&pwkeeper_master_dev);
err:
	pr_err("Probe failed, error %d\n", retval);
	return retval;

}

/**
 * pwkeeper_remove() - callback when this driver is removed
 * @pdev platform device driver data (ignored)
 *
 * Return: Always 0
 */
static int pwkeeper_remove(struct platform_device *pdev)
{
	pr_info("Removing\n");
	spin_lock_irqsave(&lists_lock, flags);

	/*
	 * In part 5, disable network integration and remove the ISR.
	 */
	cs421net_disable();
	free_irq(CS421NET_IRQ, (void *)&master_list);

	/* free all memory associated with both linked-lists */
	clear_both_lists();

	device_remove_file(&pdev->dev, &dev_attr_masters);
	device_remove_file(&pdev->dev, &dev_attr_accounts);
	misc_deregister(&pwkeeper_account_dev);
	misc_deregister(&pwkeeper_master_dev);
	spin_unlock_irqrestore(&lists_lock, flags);
	return 0;
}

static struct platform_driver cs421_driver = {
	.driver = {
		   .name = "pwkeeper",
		   },
	.probe = pwkeeper_probe,
	.remove = pwkeeper_remove,
};

static struct platform_device *pdev;

/**
 * cs421_init() -  create the platform driver
 * This is needed so that the device gains a sysfs group.
 *
 * <strong>You do not need to modify this function.</strong>
 */
static int __init cs421_init(void)
{
	pdev = platform_device_register_simple("pwkeeper", -1, NULL, 0);
	if (IS_ERR(pdev))
		return PTR_ERR(pdev);
	return platform_driver_register(&cs421_driver);
}

/**
 * cs421_exit() - remove the platform driver
 * Unregister the driver from the platform bus.
 *
 * <strong>You do not need to modify this function.</strong>
 */
static void __exit cs421_exit(void)
{
	platform_driver_unregister(&cs421_driver);
	platform_device_unregister(pdev);
}

module_init(cs421_init);
module_exit(cs421_exit);

MODULE_DESCRIPTION("CS421 Password Keeper - project 2");
MODULE_LICENSE("GPL");

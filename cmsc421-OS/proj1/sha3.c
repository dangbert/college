/**
 * @Author Dan Engbert (end1@umbc.edu)
 * This file contains a module that can be loaded into the kernel, creating 2
 *   drivers /dev/sha3_data and /dev/sha3_ctl which help hash data from the user
 *   that read/writes to them.
 *
 * Help received:
 *   http://derekmolloy.ie/writing-a-linux-kernel-module-part-2-a-character-device/
 *   https://elixir.bootlin.com/linux/v4.17/source/include/linux/fs.h#L1704
 *   https://elixir.bootlin.com/linux/v4.17/source/include/linux/miscdevice.h#L65
 *   https://www.codeproject.com/Articles/112474/A-Simple-Driver-for-Linux-OS
 *   https://www.fsl.cs.sunysb.edu/kernel-api/re256.html
 *   https://www.fsl.cs.sunysb.edu/kernel-api/re257.html
 *   https://elixir.bootlin.com/linux/v4.17/source/include/linux/kernel.h#L405
 *   https://www.browserling.com/tools/sha3-hash
 */

/*
 * This file uses kernel-doc style comments, which is similar to
 * Javadoc and Doxygen-style comments.  See
 * ~/linux/Documentation/kernel-doc-nano-HOWTO.txt for details.
 * https://www.linuxjournal.com/article/2920
 * https://www.kernel.org/doc/htmldocs/kernel-api/API-misc-register.html
 */

/*
 * Getting compilation warnings?  The Linux kernel is written against
 * C89, which means:
 *  - No // comments, and
 *  - All variables must be declared at the top of functions.
 * Read ~/linux/Documentation/CodingStyle to ensure your project
 * compiles without warnings.
 */

#define pr_fmt(fmt) "SHA3: " fmt

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/mm.h>
#include <linux/vmalloc.h>
#include <asm/uaccess.h>
#include <crypto/hash.h>

#define DIGEST_SIZE 64		/* length of a message digest */

/* global variables: */
static char *sha3_data_buffer;	/* buffer of data to be hashed */
static unsigned char *digest_buffer;	/* stores the buffer's digest */
int calculated = 0;		/* 1 if a message digest has been previously calculated */

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
 * sha3_data_read() - callback invoked when a process reads from /dev/sha3_data
 * @filp: process's file object that is reading from this device (ignored)
 * @ubuf: destination buffer to store data
 * @count: number of bytes in @ubuf
 * @ppos: file offset (in/out parameter)
 *
 * Write to @ubuf the contents of the SHA-3 input data buffer (offset by @ppos).
 * Copy the lesser of @count and (PAGE_SIZE - *@ppos).
 * Then increment the value pointed to by @ppos by the number of bytes written.
 * If @ppos is greater than or equal to PAGE_SIZE, then write nothing.
 *
 * Return: number of bytes written to @ubuf, 0 on end of file, or
 * negative on error
 */
static ssize_t sha3_data_read(struct file *filp, char __user * ubuf,
			      size_t count, loff_t * ppos)
{
	ssize_t not_copied = 0;	/* number of bytes that failed to copy */
	/* ensure we don't copy past the end of the sha3_data_buffer */
	if (*ppos >= PAGE_SIZE)
		return 0;
	if ((PAGE_SIZE - *ppos) < count)
		count = PAGE_SIZE - *ppos;

	/* copy @count bytes from sha3_data_buffer (starting at offset @ppos) into @ubuf */
	not_copied =
	    copy_to_user(ubuf, sha3_data_buffer + *ppos, (unsigned long)count);
	(*ppos) += count - not_copied;	/* increment by number of bytes written */
	return count - not_copied;
}

/**
 * sha3_data_write() - callback invoked when a process writes to /dev/sha3_data
 * @filp: process's file object that is writing to this device (ignored)
 * @ubuf: source input data buffer from user
 * @count: number of bytes in @ubuf
 * @ppos: file offset (in/out parameter)
 *
 * Copy the contents of @ubuf to the SHA-3 input data buffer (offset by @ppos).
 * Copy the lesser of @count and (PAGE_SIZE - *@ppos). Then increment the value
 * pointed to by @ppos by the number of bytes copied.
 * If @ppos is greater than or equal to PAGE_SIZE, then copy nothing.
 *
 * Return: number of bytes copied from @ubuf, or negative on error
 */
static ssize_t sha3_data_write(struct file *filp, const char __user * ubuf,
			       size_t count, loff_t * ppos)
{
	ssize_t not_copied = 0;	/* number of bytes that failed to copy */
	/* ensure we don't copy past the end of the sha3_data_buffer */
	if (*ppos >= PAGE_SIZE)
		return 0;
	if ((PAGE_SIZE - *ppos) < count)
		count = PAGE_SIZE - *ppos;

	/* copy @count bytes from ubuf into the input buffer (starting at offset @ppos) */
	not_copied =
	    copy_from_user(sha3_data_buffer + *ppos, ubuf,
			   (unsigned long)count);
	return count - not_copied;
}

/**
 * sha3_data_mmap() - callback invoked when a process mmap()s to /dev/sha3_data
 * @filp: process's file object that is writing to this device (ignored)
 * @vma: virtual memory allocation object containing mmap() request
 *
 * Create a mapping from kernel memory (specificall, the SHA-3 input
 * data buffer) into user space.
 *
 * <strong>You do not need to do modify this function.</strong>
 *
 * Return: 0 on success, negative on error.
 */
static int sha3_data_mmap(struct file *filp, struct vm_area_struct *vma)
{
	unsigned long size = (unsigned long)(vma->vm_end - vma->vm_start);
	unsigned long page = vmalloc_to_pfn(sha3_data_buffer);

	if (size > PAGE_SIZE)
		return -EIO;
	vma->vm_pgoff = 0;
	vma->vm_page_prot = PAGE_SHARED;
	if (remap_pfn_range(vma, vma->vm_start, page, size, vma->vm_page_prot))
		return -EAGAIN;
	return 0;
}

/* sha3_data device: */
static const struct file_operations sha3_data_fops = {
	.owner = THIS_MODULE,
	.read = sha3_data_read,
	.write = sha3_data_write,
	.mmap = sha3_data_mmap
};

static struct miscdevice sha3_data_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "sha3_data",
	.fops = &sha3_data_fops,
	.mode = 0666
};

/**
 * sha3_ctl_read() - callback invoked when a process reads from /dev/sha3_ctl
 *
 * @filp: process's file object that is reading from this device (ignored)
 * @ubuf: destination buffer to store data
 * @count: number of bytes in @ubuf
 * @ppos: file offset (in/out parameter)
 *
 * Write to @ubuf the contents of the most recently calculated message
 * digest, offset by @ppos. See sha3_ctl_write(). Copy the lesser of
 * @count and (digest length - *@ppos). Then increment the value
 * pointed to by @ppos by the number of bytes written. If @ppos is
 * greater than or equal to the digest length, then write nothing.
 *
 * If no message digests have been calculated, then write nothing at
 * all to @ubuf and simply return 0.
 *
 * Return: number of bytes written to @ubuf, 0 on end of file, or
 * negative on error
 */
static ssize_t sha3_ctl_read(struct file *filp, char __user * ubuf,
			     size_t count, loff_t * ppos)
{
	ssize_t not_copied = 0;	/* number of bytes that failed to copy */
	/* ensure we don't copy past the end of the sha3_data_buffer */
	if (*ppos >= DIGEST_SIZE || calculated == 0)
		return 0;
	if ((DIGEST_SIZE - *ppos) < count)
		count = DIGEST_SIZE - *ppos;

	/* copy @count bytes from sha3_data_buffer (starting at offset @ppos) into @ubuf */
	not_copied =
	    copy_to_user(ubuf, digest_buffer + *ppos, (unsigned long)count);
	(*ppos) += count - not_copied;	/* increment by number of bytes written */
	return count - not_copied;
}

/**
 * sha3_ctl_write() - callback invoked when a process writes to /dev/sha3_ctl
 * @filp: process's file object that is writing to this device (ignored)
 * @ubuf: source buffer from user
 * @count: number of bytes in @ubuf
 * @ppos: file offset (ignored)
 *
 * Interpreting @ubuf as an ASCII decimal value, convert it to an
 * unsigned value via kstrtoul_from_user(). If that value is greater
 * than PAGE_SIZE, then return an error. Otherwise, call sha3_digest()
 * to hash the input data.
 *
 * Return: @count, or negative on error
 */
static ssize_t sha3_ctl_write(struct file *filp, const char __user * ubuf,
			      size_t count, loff_t * ppos)
{
	size_t digest_len = DIGEST_SIZE;	/* will store number of bytes in the calculated digest */
	unsigned long data_len = 0;	/* number of bytes in sha3_data_buffer to hash */
	/* convert ubuf string to a long */
	if (0 != kstrtoul_from_user(ubuf, count, 10, &data_len))
		return -1;
	if (data_len > PAGE_SIZE)
		return -1;

	/* hash the data buffer and store result in digest buffer */
	if (0 !=
	    sha3_digest(sha3_data_buffer, data_len, digest_buffer, &digest_len))
		return -1;
	calculated = 1;
	return digest_len;
}

/* sha3_ctl device: */
static const struct file_operations sha3_ctl_fops = {
	.read = sha3_ctl_read,
	.write = sha3_ctl_write,
};

static struct miscdevice sha3_ctl_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "sha3_ctl",
	.fops = &sha3_ctl_fops,
	.mode = 0666
};

/**
 * sha3_init() - entry point into the SHA-3 kernel module
 * Return: 0 on successful initialization, negative on error
 */
static int __init sha3_init(void)
{
	pr_info("Loading module\n");

	sha3_data_buffer = vzalloc(PAGE_SIZE);
	if (!sha3_data_buffer) {
		pr_err("Could not allocate memory\n");
		return -ENOMEM;
	}
	digest_buffer = vzalloc(DIGEST_SIZE);
	if (!sha3_data_buffer) {
		pr_err("Could not allocate memory\n");
		return -ENOMEM;
	}
	misc_register(&sha3_data_dev);
	misc_register(&sha3_ctl_dev);

	return 0;
}

/**
 * sha3_exit() - called by kernel to clean up resources
 */
static void __exit sha3_exit(void)
{
	pr_info("Unloading module\n");
	vfree(sha3_data_buffer);
	misc_deregister(&sha3_data_dev);
	misc_deregister(&sha3_ctl_dev);
}

module_init(sha3_init);
module_exit(sha3_exit);

MODULE_DESCRIPTION("CS421 SHA-3 driver");
MODULE_LICENSE("GPL");

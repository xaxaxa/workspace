/*  
 *  hello-1.c - The simplest kernel module.
 */

#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/fs.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");

int troll_open(struct inode *i, struct file *f)
{
	try_module_get(THIS_MODULE);
	return 0;
}
ssize_t troll_read(struct file *f, char __user *buf, size_t len, loff_t *offset)
{
	loff_t off=*offset;
	char* s1="troll";
	char* s2=" lol";
	size_t len1=5;
	size_t len2=4;
	size_t tmp_len=len;
	//printk(KERN_INFO "troll_read called; len=%d off=%d\n",len,off);
	if(off<len1)
	{
		size_t tmp=len<(len1-off)?len:(len1-off);
		int ret=copy_to_user(buf, s1+off, tmp);
		//printk(KERN_INFO "copy_to_user: len=%d off=%d ret=%d\n",tmp,off,ret);
		len-=tmp;
		buf+=tmp;
	}
	else
	{
		size_t offset2;
		size_t tmp;
		off-=len1;
		offset2=off % len2;
		tmp=len<(len2-offset2)?len:(len2-offset2);
		copy_to_user(buf, s2+offset2, tmp);
		len-=tmp;
		buf+=tmp;
	}
	while(len>0)
	{
		size_t tmp=len<len2?len:len2;
		copy_to_user(buf, s2, tmp);
		len-=tmp;
		buf+=tmp;
	}
	return tmp_len;
}
int troll_release(struct inode *i, struct file *f)
{
	module_put(THIS_MODULE);
	return 0;
}

static struct file_operations troll_ops = {
.read = troll_read,
//.write = troll_write,
.open = troll_open,
.release = troll_release
};



static int troll_major;

int __init troll_init(void)
{
	printk(KERN_INFO "initializing troll\n");
	troll_major = register_chrdev(0, "troll", &troll_ops);
	printk(KERN_INFO "troll: major number=%d\n", troll_major);
	/* 
	 * A non 0 return means init_module failed; module can't be loaded. 
	 */
	return 0;
}
void __exit troll_exit(void)
{
	unregister_chrdev(troll_major, "troll");
	printk(KERN_INFO "exit_module()\n");
}
module_init(troll_init);
module_exit(troll_exit);


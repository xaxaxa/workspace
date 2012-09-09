/*
 * trollfs - troll filesystem
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 */

#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/stat.h>
#include <linux/namei.h>
#include <linux/mount.h>
#include <linux/statfs.h>
#include <linux/file.h>

/*
 * trollfs private data structures
 * - super_block: trollfs_filesystem
 * - dentry: trollfs_dentryData
 * - inode: trollfs_iNodeData
 */

///s: struct super_block
#define SB_PRIVDATA(s) ((s).s_fs_info)
#define SB_PRIVDATA1(s) ((trollfs_Filesystem*)SB_PRIVDATA(s))
///d: struct dentry
#define DENTRY_PRIVDATA(d) ((d).d_fsdata)
#define DENTRY_PRIVDATA1(d) ((trollfs_DentryData*)DENTRY_PRIVDATA(d))
#define DENTRY_BASE(d) ((DEntry *)DENTRY_PRIVDATA(d))

#define INODE_PRIVDATA(i) ((i).i_private)
#define INODE_PRIVDATA1(i) ((trollfs_INodeData*)INODE_PRIVDATA(i))

#define CLASSOF(strct) struct strct
#define NEW(clss) ((clss*)kmalloc(sizeof(clss), GFP_KERNEL))
#define NEW_S(strct) ((CLASSOF(strct)*)kmalloc(sizeof(CLASSOF(strct)), GFP_KERNEL))

///obj: void*
#define DELETE(obj) kfree(obj)

#define ON_OOM() printk(KERN_WARNING "%s: out of memory\n", __FUNCTION__)

///e: Error (aka int)
#define ERROR_TO_POINTER(e) ERR_PTR(-e)

//for convenience
//C sucks; you have to put "struct type_x" instead of just "type_x".
typedef struct super_block SuperBlock;
typedef struct dentry DEntry;
typedef struct inode INode;
typedef struct qstr QStr;
typedef int Error;
typedef unsigned long INodeNumber;

//private data structure of super_block
struct trollfs_filesystem
{	//class filesystem
	DEntry *baseDir;
};
//private data structure of dentry
struct trollfs_dentryData
{
	DEntry *baseDir;
};
//private data structure of inode
struct trollfs_iNodeData
{
	INode *baseINode;
};
struct trollfs_options
{
	const char* baseDir;
	
};

//for convenience
typedef struct trollfs_filesystem trollfs_Filesystem;
typedef struct trollfs_dentryData trollfs_DentryData;
typedef struct trollfs_iNodeData trollfs_INodeData;
typedef struct trollfs_options trollfs_Options;



int trollfs_parseOptions(trollfs_Options* opt, char* options);
int trollfs_mount(SuperBlock *sb, void *options, int silent);
int trollfs_get_sb(struct file_system_type *fs_type, int flags,
	const char *dev_name, void *raw_data, struct vfsmount *mnt);
void trollfs_kill_sb(struct super_block *sb);
int trollfs_d_hash(DEntry *dentry, QStr *name);
int trollfs_d_compare(DEntry *dentry, QStr *a, QStr *b);
int trollfs_d_delete(DEntry *dentry);
void trollfs_d_release(DEntry *dentry);
void trollfs_d_iput(DEntry *dentry, INode *inode);

static struct file_system_type trollfs_filesystem_ops =
{
	.owner          = THIS_MODULE,
	.name           = "trollfs",
	.get_sb         = trollfs_get_sb,
	.kill_sb        = trollfs_kill_sb,
	.fs_flags       = 0,
};
static struct dentry_operations trollfs_d_op = {
	d_revalidate:	NULL,
	d_hash:		trollfs_d_hash,
	d_compare:		trollfs_d_compare,
	d_release:		trollfs_d_release,
	d_delete:		trollfs_d_delete,
	d_iput:		trollfs_d_iput,
};
static struct inode_operations trollfs_file_inode_ops = {
	setattr:		trollfs_setattr,
	getattr:		trollfs_getattr,
	setxattr:		trollfs_setxattr,
	getxattr:		trollfs_getxattr,
	listxattr:		trollfs_listxattr,
	removexattr:	trollfs_removexattr
};
struct inode_operations trollfs_dir_inode_iops =
{
	create:	trollfs_create,
	lookup:	trollfs_lookup,
	link:		trollfs_link,
	unlink:	trollfs_unlink,
	symlink:	trollfs_symlink,
	mkdir:	trollfs_mkdir,
	rmdir:	trollfs_rmdir,
	mknod:	trollfs_mknod,
	rename:	trollfs_rename,
	getattr:	trollfs_getattr,
	setattr:	trollfs_setattr,
	setxattr:	trollfs_setxattr,
	getxattr:	trollfs_getxattr,
	listxattr:	trollfs_listxattr,
	removexattr:trollfs_removexattr
};



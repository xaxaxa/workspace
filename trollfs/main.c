/*
 * trollfs - troll filesystem
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
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


#include "trollfs.h"

MODULE_LICENSE("GPL");

///initialize an empty inode
Error trollfs_read_inode(INode *inode)
{
	INODE_PRIVDATA(*inode)=NEW(trollfs_INodeData);
	if(!INODE_PRIVDATA(*inode)) {
		ON_OOM();
		return ENOMEM;
	}
	inode->i_version++;
	inode->i_op = &trollfs_inode_ops;
	inode->i_fop = &trollfs_file_ops;
	inode->i_mapping->a_ops = &trollfs_address_space_ops;
}

INode *trollfs_iget(SuperBlock *sb, INodeNumber ino)
{
	Error ret=0;
	INode *inode;

	inode = iget_locked(sb, ino);
	if (!inode) {
		ON_OOM(); ret=ENOMEM;
		goto fail;
	}
	if (!(inode->i_state & I_NEW))
		//inode already in inode cache
		return inode;

	if(ret=trollfs_read_inode(inode)) {
		goto fail1;
	}

	unlock_new_inode(inode);
	return inode;
fail1:
	unlock_new_inode(inode);
fail:
	return ERROR_TO_POINTER(ret);
}

int trollfs_d_hash(DEntry *d, QStr *name)
{
	return DENTRY_BASE(d)->d_op->d_hash(DENTRY_BASE(d), name);
}
int trollfs_d_compare(DEntry *d, QStr *a, QStr *b)
{
	return DENTRY_BASE(d)->d_op->d_compare(DENTRY_BASE(d), a, b);
}
int trollfs_d_delete(DEntry *d)
{
	return DENTRY_BASE(d)->d_op->d_delete(DENTRY_BASE(d));
}
void trollfs_d_release(DEntry *d)
{
	return DENTRY_BASE(d)->d_op->d_release(DENTRY_BASE(d));
}
void trollfs_d_iput(DEntry *d, INode *inode)
{
	return DENTRY_BASE(d)->d_op->d_iput(DENTRY_BASE(d), inode);
}



Error trollfs_parseOptions(trollfs_Options* opt, char* options)
{
	opt->baseDir=options;
	return 0;
}
Error trollfs_mount(SuperBlock *sb, void *options, int silent)
{
	Error tmp_err;
	Error ret=0;
	if((!options) || (!sb)) return -EINVAL;
	trollfs_Options opt;
	
	//no, this is not an error. the "=" is not used for comparison.
	if(tmp_err=trollfs_parseOptions(&opt, (char*)options))
		return tmp_err;
	SB_PRIVDATA(*sb)=NEW(trollfs_Filesystem);
	if(!SB_PRIVDATA(*sb)) {
		ON_OOM();
		ret=ENOMEM;
		goto fail;
	}
	
	//lookup baseDir and get its dirent
	struct nameidata tmp_namei;
	if(tmp_err=path_lookup(opt.baseDir, LOOKUP_FOLLOW, &tmp_namei)) {
		printk(KERN_WARNING "could not access baseDir: errno=%d", tmp_err);
		return tmp_err;
	}
	SB_PRIVDATA1(*sb)->baseDir=tmp_namei.path.dentry;
	
	//create the root dentry (root directory of the fs)
	if(!(sb->s_root=NEW(DEntry))) {
		ON_OOM();
		ret=ENOMEM;
		goto fail1;
	}
	sb->s_root->d_sb=sb;
	sb->s_root->d_parent = sb->s_root;
	DENTRY_PRIVDATA(sb->s_root) = SB_PRIVDATA1(*sb)->baseDir;
	sb->s_root->d_op = &trollfs_d_op;
	
	///TBD: this does NOT work. when vfs calls into the base filesystem
	///via the inode's inode_operations, it'll pass in its dentry which
	///is OUR dentry, and the base FS doesn't expect this.
	//sb->s_root->d_inode = DENTRY_BASE(sb->s_root)->d_inode;
	
	sb->s_root->d_inode=trollfs_iget(sb,iunique(sb, 25));
	
	return 0;
fail2:
	DELETE(sb->s_root);
fail1:
	DELETE(SB_PRIVDATA(*sb));
fail:
	return -ret;
}
int trollfs_get_sb(struct file_system_type *fs_type, int flags,
	const char *dev_name, void *raw_data, struct vfsmount *mnt)
{
	return get_sb_nodev(fs_type, flags, raw_data, trollfs_mount, mnt);
}
void trollfs_kill_sb(struct super_block *sb)
{
	generic_shutdown_super(sb);
}

int __init trollfs_init(void)
{
	printk(KERN_INFO "initializing troll\n");
	return register_filesystem(&trollfs_filesystem_ops);
	/* 
	 * A non 0 return means init_module failed; module can't be loaded. 
	 */
	return 0;
}
void __exit trollfs_exit(void)
{
	printk(KERN_INFO "exit_module()\n");
	unregister_filesystem(&trollfs_filesystem_ops);
}
module_init(trollfs_init);
module_exit(trollfs_exit);


/*
 * untitled.cxx
 * 
 * Copyright 2011  <xaxaxa@xaxaxa>
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
 * 
 */
#define FUSE_USE_VERSION 27
#include <iostream>
#include <fuse/fuse_lowlevel.h>
#include <map>
#include <errno.h>
#include <sys/stat.h>
#include <memory.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <sys/types.h>
#include <cplib/cplib.hpp>
using namespace std;
using namespace xaxaxa;
struct ino
{
	//long n;
	long parent;
	string name;
	virtual mode_t mode()=0;
};
struct file:public ino
{
	string data;
	virtual mode_t mode()
	{
		return S_IFREG|0777;
	}
};
struct dir:public ino
{
	map<string,long> entries;
	virtual mode_t mode()
	{
		return S_IFDIR|0777;
	}
};
dir* root=new dir();
map<long,ino*> inodes;
void addinode(long parent, long n, ino* i)
{
	dynamic_cast<dir*>(inodes[parent])->entries[i->name]=n;
	inodes[n]=i;
	i->parent=parent;
}
bool op_stat(long i, struct stat& st)
{
	map<long,ino*>::iterator it=inodes.find(i);
    if(it==inodes.end())return false;
    ino* inode=(*it).second;
    st.st_ino=i;
	st.st_nlink=1;
	st.st_mode=inode->mode();
    if(st.st_mode&S_IFREG)st.st_size=((file*)inode)->data.length();
    return true;
}
void op_lookup(fuse_req_t req, fuse_ino_t parent, const char *name)
{
	//cout << "lookup " << name << endl;
	dir* tmp=dynamic_cast<dir*>(inodes[parent]);
	map<string,long>::iterator it=tmp->entries.find(string(name));
	if(it==tmp->entries.end())
	{
		//cout << "ENOENT" << endl;
		fuse_reply_err(req, ENOENT);
		return;
	}
	fuse_entry_param ent;
    memset(&ent, 0, sizeof(ent));
    ent.attr_timeout = 2.0;
    ent.entry_timeout = 2.0;
    ent.ino=(*it).second;
    op_stat(ent.ino,ent.attr);
    //cout << (*it).second << endl;
    fuse_reply_entry(req, &ent);
}
void op_getattr(fuse_req_t req, fuse_ino_t i,
        struct fuse_file_info *fi)
{
	//cout << "getattr " << i << endl;
	struct stat st;
    memset(&st, 0, sizeof(st));
    //map<long,ino*>::iterator it=inodes.find(i);
    if(!op_stat(i,st))
    {
		fuse_reply_err(req, ENOENT);
		//cout << "ENOENT" << endl;
		return;
	}
    fuse_reply_attr(req, &st, 1.0);
}
static int reply_buf_limited(fuse_req_t req, const char *buf, size_t bufsize,
        off_t off, size_t maxsize)
{
    if ((size_t)off < bufsize)
        return fuse_reply_buf(req, buf + off,
                min(bufsize - off, maxsize));
    else
        return fuse_reply_buf(req, NULL, 0);
}
void addent(const fuse_req_t& req, StringBuilder& sb, const string name, long i)
{
	struct stat stbuf;
    memset(&stbuf, 0, sizeof(stbuf));
    stbuf.st_ino=i;
    int tmp=sb.Length+fuse_add_direntry(req, NULL, 0, name.c_str(), NULL, 0);
    sb.EnsureCapacity(tmp);
    fuse_add_direntry(req,((char*)sb.buf)+sb.Length,sb.Capacity-sb.Length,name.c_str(),&stbuf,tmp);
    sb.Length=tmp;
}
void op_readdir(fuse_req_t req, fuse_ino_t i, size_t size,
        off_t off, struct fuse_file_info *fi)
{
	//cout << "readdir " << i << " off=" << off << "; size=" << size << endl;
	dir* tmp=dynamic_cast<dir*>(inodes[i]);
	//dirbuf b;
	StringBuilder sb(256);

    //memset(&b, 0, sizeof(b));
    //dirbuf_add(req, &b, ".", i);
    //dirbuf_add(req, &b, "..", tmp->parent);
    addent(req,sb,".",i);
    addent(req,sb,"..",tmp->parent);
    
    map<string,long>::iterator it;
    for(it=tmp->entries.begin();it!=tmp->entries.end();it++)
    {
		//dirbuf_add(req, &b, (*it).first.c_str(), (*it).second);
		addent(req,sb,(*it).first.c_str(), (*it).second);
	}
    reply_buf_limited(req, (char*)sb.buf, sb.Length, off, size);
    //fuse_reply_buf(req, (char*)sb.buf,sb.Length);
    //free(b.p);
}
void op_open(fuse_req_t req, fuse_ino_t i,
        struct fuse_file_info *fi)
{
	//cout << "open " << i << endl;
	map<long,ino*>::iterator it=inodes.find(i);
    if(it==inodes.end())
    {
		fuse_reply_err(req, ENOENT);
		return;
	}
	if(!((*it).second->mode()&S_IFREG))
	{
		fuse_reply_err(req, EISDIR);
		return;
	}
	fuse_reply_open(req, fi);
}
void op_read(fuse_req_t req, fuse_ino_t i, size_t size,
        off_t off, struct fuse_file_info *fi)
{
	map<long,ino*>::iterator it=inodes.find(i);
    if(it==inodes.end())
    {
		fuse_reply_err(req, ENOENT);
		return;
	}
	if(!((*it).second->mode()&S_IFREG))
	{
		fuse_reply_err(req, EISDIR);
		return;
	}
	file* f=dynamic_cast<file*>((*it).second);
	if((size_t)off>=f->data.length() || size<=0)
	{
        fuse_reply_buf(req, NULL,  0);
        return;
    }
	size_t maxsize=f->data.length()-off;
    fuse_reply_buf(req, f->data.c_str()+off, size>maxsize?maxsize:size);
}
int main(int argc, char** argv)
{
	/*struct stat st;
	cout << sizeof(st.st_ino) << endl;
	return 0;*/
	inodes[1]=root;
	inodes[1]->parent=1;
	file tmp;
	tmp.name="aaaaa";
	tmp.data="fuck";
	addinode(1,2,&tmp);
	file tmp1;
	tmp1.name="xxx";
	tmp1.data="hjgfhdghfkdsghkjfdhgkjd\n";
	addinode(1,3,&tmp1);
	
	fuse_lowlevel_ops ops;
	memset(&ops,0,sizeof(ops));
	ops.lookup=op_lookup;
	ops.getattr=op_getattr;
	ops.readdir=op_readdir;
	ops.open=op_open;
	ops.read=op_read;
	struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
    struct fuse_chan *ch;
    int err = -1;
    char *mountpoint;
    if (fuse_parse_cmdline(&args, &mountpoint, NULL, NULL) != -1 &&
            (ch = fuse_mount(mountpoint, &args)) != NULL)
    {
		fuse_session *se;
		se = fuse_lowlevel_new(&args, &ops, sizeof(ops), NULL);
		if (se != NULL) {
			if (fuse_set_signal_handlers(se) != -1) {
				fuse_session_add_chan(se, ch);
				err = fuse_session_loop(se);
				fuse_remove_signal_handlers(se);
				fuse_session_remove_chan(ch);
			}
			fuse_session_destroy(se);
		}
		fuse_unmount(mountpoint, ch);
	}
	return 69;
}

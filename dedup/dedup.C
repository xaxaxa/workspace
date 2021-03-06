#define _BSD_SOURCE
#include <iostream>
#include <functional>
#include <dirent.h>
#include <vector>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <map>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <algorithm>

using namespace std;

//1KiB minimum size
#define DEDUP_MIN_FILESIZE 1024
#define BUFFERSIZE (1024*4)



/*template<class T, int groupSize=64>class workQueue
{
	class itemGroup
	{
		T items[groupSize];
		int start,length;
		itemGroup* next;
		itemGroup(): start(0), length(0), next(NULL)
		{}
	};
	itemGroup* firstGroup;
	itemGroup* lastGroup;
	workQueue():firstGroup(NULL), lastGroup(NULL)
	{}
	void appendGroup()
	{
		itemGroup* g=new itemGroup();
		lastGroup->next=g;
		lastGroup=g;
	}
	void popGroup()
	{
		itemGroup* tmp=firstGroup;
		if(tmp==lastGroup)
			firstGroup=lastGroup=NULL;
		else {
			firstGroup=firstGroup->next;
		}
		delete tmp;
	}
	void append(const T& item)
	{
		if(lastGroup==NULL) {
			lastGroup=new itemGroup();
			if(firstGroup==NULL)firstGroup=lastGroup;
		}
		if(lastGroup->start+lastGroup->length>=groupSize)
			appendGroup();
		lastGroup->items[lastGroup->start+lastGroup->length]=item;
		lastGroup->length++;
	}
	bool pop(T& out)
	{
	retry:
		if(firstGroup==NULL)return false;
		if(firstGroup->length<=0)return false;
	
		int tmp_start=firstGroup->start;
		int tmp_length=firstGroup->length;
		if(tmp_start>=groupSize) goto retry;
		out=firstGroup->items[tmp_start];
		
		if(!__sync_bool_compare_and_swap(&firstGroup->start, tmp_start, tmp_start+1);
		if(!__sync_bool_compare_and_swap(&firstGroup->length, tmp_length, tmp_length-1);
		if((--firstGroup->length)==0)
			popGroup();
		return true;
	}
};*/



//structs
struct fileItem
{
	string path;
};
struct openedFileItem
{
	fileItem f;
	int fd;
};
struct bufferItem
{
	uint8_t* buf;
	openedFileItem f;
};
struct fileItem1
{
	uint64_t fileSize;
	fileItem f;
};
struct fileGroup
{
	uint64_t fileSize;
	vector<fileItem> files;
};


//global vars
volatile uint64_t bytesSaved=0;
volatile int cur_pos;
vector<fileGroup> tasks;
pthread_mutex_t stdio_mutex;

//utility functions

//the variable passed in to cur_iterator should also be declared as volatile
int atomic_get_and_increment(volatile int& cur_iterator)
{
	/*int tmp=cur_iterator;
	while(!__sync_bool_compare_and_swap(&cur_iterator, tmp, tmp+1));
	return tmp;*/
	return __sync_fetch_and_add(&cur_iterator, 1);
}

string path_combine(string p1, string p2) {
	/*int i=0, next_i;
	bool first=true;
	while((next_i=p2.find("/", i))>-1) {
		if(next_i==i) {
			if(first) //starts with "/"
				return p2;
			goto cont;
		}
		string comp=p2.substr(i, next_i-i-1);
		if(comp.compare("..")==0) {
			
		}
	cont:
		first=false; i=next_i+1;
	}*/
}

void scanDir(const function<void(const string&,dirent&)>& cb, string dir)
{
	//cout << "scanning " << dir << endl;
	DIR* d=opendir(dir.c_str());
	if(d==NULL) {
		perror(dir.c_str());
		return;
	}
	int len = offsetof(dirent, d_name) + pathconf(dir.c_str(), _PC_NAME_MAX) + 1;
	char ent[len];
	dirent* ent1=(dirent*)ent;
	while(readdir_r(d, (dirent*)ent, &ent1)==0 && ent1!=NULL) {
		if(strcmp(ent1->d_name, ".")==0 || strcmp(ent1->d_name, "..")==0)
			continue;
		cb(dir, *ent1);
		int namelen=strlen(ent1->d_name);
		if(ent1->d_type==DT_DIR)
		{
			scanDir(cb, dir+"/"+string(ent1->d_name, namelen));
		}
	}
	closedir(d);
	
}

int doRead(int fd, uint8_t* buf, int len)
{
	int off=0;
	while(off<len) {
		int br=read(fd, buf+off, len-off);
		if(br<=0) return off;
		off+=br;
	}
	return off;
}

void processDuplicates(uint64_t filesize, const vector<fileItem>& q)
{
	pthread_mutex_lock(&stdio_mutex);
	cout << "the following files of size " << filesize << " are identical:" << endl;
	for(int i=0;i<q.size();i++) {
		cout << "\t" << q[i].path << endl;
	}
	pthread_mutex_unlock(&stdio_mutex);
	__sync_fetch_and_add(&bytesSaved,filesize*(q.size()-1));
}

bool compareBuffers(const bufferItem& b1, const bufferItem& b2)
{
	return memcmp(b1.buf, b2.buf, BUFFERSIZE)<0;
}

void compareFiles(uint64_t filesize, uint64_t& br, const vector<openedFileItem>& files)
{
	//cout << "comparing " << files.size() << " files" << endl;
	if(files.size()<2)return;
	///////////////////////////
	vector< vector<openedFileItem> > cur_queues;
	{
		bool diverge(false);
		uint8_t* buf=new uint8_t[BUFFERSIZE*files.size()];
		while(br<filesize) {
			vector<bufferItem> bufs;
			uint64_t expected_br=filesize-br;
			if(expected_br>BUFFERSIZE)expected_br=BUFFERSIZE;
			for(int i=0;i<files.size();i++) {
				uint64_t _br=doRead(files[i].fd,buf+(BUFFERSIZE*i), BUFFERSIZE);
				if(_br==expected_br) {
					bufs.push_back({buf+(BUFFERSIZE*i), files[i]});
					if(br<BUFFERSIZE) memset(buf+(BUFFERSIZE*i)+_br, 0, BUFFERSIZE-_br);
				}
				else
					diverge=true;
			}
			br+=expected_br;
			sort(bufs.begin(), bufs.end(), compareBuffers);
			
			cur_queues.resize(0);
			vector<openedFileItem> cur_queue;
			uint8_t* cur_buffer=NULL;
			for(auto it=bufs.begin();it!=bufs.end();it++) {
				//cout << (*it).first << " " << (*it).second << endl;
				if(cur_buffer==NULL || memcmp((*it).buf, cur_buffer, BUFFERSIZE)==0) {
					if(cur_buffer==NULL)cur_buffer=(*it).buf;
				} else {
					diverge=true;
					cur_queues.push_back(cur_queue);
					cur_queue.resize(0);
					cur_buffer=(*it).buf;
				}
				cur_queue.push_back((*it).f);
			}
			cur_queues.push_back(cur_queue);
			if(diverge) {
				delete[] buf;
				goto do_diverge;
			}
		}
		delete[] buf;
		goto aaaaa;
	}
do_diverge:
	for(int i=0;i<cur_queues.size();i++) {
		uint64_t br1=br;
		compareFiles(filesize, br1, cur_queues[i]);
	}
	return;
aaaaa:
	vector<fileItem> fileItems;
	for(int i=0;i<files.size();i++) {
		fileItems.push_back(files[i].f);
	}
	processDuplicates(filesize, fileItems);
}

void processQueue(uint64_t filesize, const vector<fileItem>& q)
{
	if(q.size()<=1 || filesize<DEDUP_MIN_FILESIZE) return;
	//cout << "processing queue of " << q.size() << " " << filesize << " byte files" << endl;
	/*if(q.size()==2) {
		//skip hashing if there's only 2 files
		uint8_t* buf=NULL;
		int f1,f2;
		if((f1=open(q[0].path.c_str(), O_RDONLY))<0) goto fail1;
		if((f2=open(q[1].path.c_str(), O_RDONLY))<0) goto fail2;
		
		

		
		
		
		buf=new uint8_t[BUFFERSIZE*2];
		while(true) {
			int i1=doRead(f1,buf,BUFFERSIZE);
			int i2=doRead(f2,buf+BUFFERSIZE,BUFFERSIZE);
			if(i1!=i2) goto fail3;
			if(i1==0) break;
			if(memcmp(buf, buf+BUFFERSIZE, i1)!=0) goto fail3;
		}
		delete[] buf;
		close(f2); close(f1);
		//files are the same
		processDuplicates(filesize, q);
		return;
	fail3:
		delete[] buf;
		close(f2);
	fail2:
		close(f1);
	fail1:
		return;
	} else {*/
		uint8_t* buf;
		uint64_t br;
		vector<openedFileItem> files;
		//open all files
		for(int i=0;i<q.size();i++) {
			openedFileItem it;
			if((it.fd=open(q[i].path.c_str(), O_RDONLY))<0) {
				perror("open");
			} else {
				it.f=q[i];
				files.push_back(it);
			}
		}
		br=0;
		compareFiles(filesize, br, files);
		
	_fail1:
		for(int i=0;i<files.size();i++)
			close(files[i].fd);
		return;
	//}
}


bool comparefileItem1(const fileItem1& a, const fileItem1& b)
{
	return a.fileSize<b.fileSize;
}

void* comparerThread(void* unused)
{
	while(true) {
		int tmp=atomic_get_and_increment(cur_pos);
		//printf("i: %i\n", tmp);
		if(tmp>=tasks.size()) return NULL;
		processQueue(tasks[tmp].fileSize, tasks[tmp].files);
	}
}
fileGroup* appendTask()
{
	tasks.resize(tasks.size()+1);
	return &tasks[tasks.size()-1];
}
int main(int argc, char** argv)
{
	if(argc<2) {
		cout << "usage: " << argv[0] << " DIR" << endl;
		return 1;
	}
	vector<fileItem1> entries;
	scanDir([&entries](const string& p, dirent& ent) {
		
		//cout << path << endl;
		if(ent.d_type!=DT_REG)return;
		string path=(p+"/"+string(ent.d_name));
		struct stat st;
		if(stat(path.c_str(), &st)!=0)
			return;
		entries.push_back({st.st_size, {path}});
	}, argv[1]);
	sort(entries.begin(),entries.end(),comparefileItem1);
	
	fileGroup* cur_queue;
	uint64_t cur_filesize=0;
	
	for(auto it=entries.begin();it!=entries.end();it++) {
		//cout << (*it).first << " " << (*it).second.path << endl;
		if((*it).fileSize!=0 && (*it).fileSize==cur_filesize) {
		} else {
			///processQueue(cur_filesize, cur_queue);
			cur_queue=appendTask();
			cur_filesize=(*it).fileSize;
			cur_queue->fileSize=cur_filesize;
		}
		cur_queue->files.push_back((*it).f);
	}
	
	int nthreads=16;
	pthread_t threads[nthreads];
	cur_pos=0;
	for(int i=0;i<nthreads;i++) {
		pthread_create(&threads[i],NULL,comparerThread,NULL);
	}
	for(int i=0;i<nthreads;i++) {
		void* ret;
		pthread_join(threads[i],&ret);
	}
	
	cout << "you can save " << bytesSaved << " bytes of disk space" << endl;
}


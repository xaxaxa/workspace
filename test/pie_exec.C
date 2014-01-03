#include <dlfcn.h>
#include <stdio.h>
#include <stdint.h>
#include <elf.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>
#include <stdexcept>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>
#include <sys/syscall.h>
#include <linux/unistd.h>

using namespace std;
typedef uint8_t u8;
typedef uint16_t u16;
typedef int16_t s16;
typedef uint32_t u32;
typedef int32_t s32;
typedef Elf32_Ehdr ELFHeader;
typedef Elf32_Phdr ELFPHeader;
typedef Elf32_auxv_t ELFAUXV;
typedef unsigned long int PTR;

#define ROUNDUP(x, y)   ((((x)+((y)-1))/(y))*(y))
#define	ALIGN(k, v)	(((k)+((v)-1))&(~((v)-1)))
#define	ALIGNDOWN(k, v)	(size_t(size_t(k)/v)*v)
size_t pgsz=4096;

void throwUNIXException() {
	throw runtime_error(strerror(errno));
}

//get page protection flags from elf program header flags
u32 getProtectionFlags(u32 elfSegmentFlags) {
	u32 prot=0;
	if((elfSegmentFlags&PF_R)==PF_R) prot |= PROT_READ;
	if((elfSegmentFlags&PF_W)==PF_W) prot |= PROT_WRITE;
	if((elfSegmentFlags&PF_X)==PF_X) prot |= PROT_EXEC;
	if(prot==0) prot=PROT_NONE;
	return prot;
}

//maps all LOAD segments, and returns base offset (usually random)
//assumes PIE; non-PIE executables will never be supported
//does NOT check that offsets are not out-of-bounds
//returns NULL on failure, and sets errno appropriately
u8* mapExecutable(ELFHeader* header, int fdToMap, int fdOffset=0) {
	u8* file=(u8*)header;
	ELFPHeader* pHeaders=(ELFPHeader*)(file+header->e_phoff);	//array
	//find the maximum among the end addresses of all mappings;
	//this is the minimum size of the contiguous memory range
	//that the whole program will fit in
	u8* mappingSize=NULL;	//not an actual pointer, just an int large enough to
							//store a pointer
	for(int i=0;i<(int)header->e_phnum;i++)
		if(pHeaders[i].p_type==PT_LOAD) {
			//end address of this mapping
			u8* endAddr=(u8*)(pHeaders[i].p_vaddr+pHeaders[i].p_memsz);
			if(endAddr>mappingSize) mappingSize=endAddr;
		}
	printf("mappingSize=0x%x\n",(size_t)mappingSize);
	//remap the whole file
	u8* base=(u8*)mmap(NULL,(size_t)mappingSize,PROT_NONE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
	if(base==NULL) return NULL;
	
	//create the real mappings
	for(int i=0;i<(int)header->e_phnum;i++)
		if(pHeaders[i].p_type==PT_LOAD) {
			u8* mapToUnaligned=(u8*)(base+(size_t)pHeaders[i].p_vaddr);
			u8* mapTo=(u8*)ALIGNDOWN(mapToUnaligned,pgsz);
			u8* mappingEnd=mapToUnaligned+PTR(pHeaders[i].p_memsz);
			u8* dataEnd=mapToUnaligned+PTR(pHeaders[i].p_filesz);
			mappingEnd=(u8*)ROUNDUP((PTR)mappingEnd,pgsz);
			u8* fileMappingEnd=(u8*)ROUNDUP((PTR)dataEnd,pgsz);
			size_t offset=ALIGNDOWN(pHeaders[i].p_offset,pgsz);
			//size_t size=pHeaders[i].p_memsz+size_t(mapToUnaligned-mapTo);
			size_t fileMappingSize=fileMappingEnd-mapTo;
			size_t size=mappingEnd-mapTo;
			printf("mapping segment of size 0x%x to %p\n",size,mapTo);
			void* tmp=mmap(mapTo,fileMappingSize,
				PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_FIXED,
				fdToMap,fdOffset+offset);
			if(tmp==MAP_FAILED) {
				int tmp1=errno;
				munmap(base,(size_t)mappingSize);
				errno=tmp1;
				return NULL;
			}
			if(size>fileMappingSize) {
				//leftover memory range not covered by the backing file
				tmp=mmap(mapTo+fileMappingSize,size-fileMappingSize,PROT_READ|PROT_WRITE,
					MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS,-1,0);
				if(tmp==MAP_FAILED) {
					int tmp1=errno;
					munmap(base,(size_t)mappingSize);
					errno=tmp1;
					return NULL;
				}
			}
			//int zerosz=pHeaders[i].p_memsz-pHeaders[i].p_filesz;
			
			
			int zerosz=int(fileMappingEnd-dataEnd);
			if(zerosz>0) {
				//memset((void*)dataEnd,0,zerosz);
				for(int i=0;i<zerosz;i++) dataEnd[i]=0;
			}
			mprotect(mapTo,size,getProtectionFlags(pHeaders[i].p_flags));
			//brk(mapToUnaligned+pHeaders[i].p_memsz);
		}
	return base;
}
struct executableInfo
{
	u8* image;
	u8* loaderImage;
	int imageLen,loaderLen;
	int fd,loaderfd;
	void destruct() {
		if(image!=NULL)munmap(image,imageLen);
		if(loaderImage!=NULL)munmap(loaderImage,loaderLen);
		if(fd>=0)close(fd);
		if(loaderfd>=0)close(loaderfd);
	}
};
//parses an executable but doesn't instantiate it (create the appropriate mappings for execution)
executableInfo loadExecutable(const char* path) {
	executableInfo inf;
	inf.image=inf.loaderImage=NULL;
	inf.fd=inf.loaderfd=-1;
	
	if((inf.fd=open(path,O_RDONLY))<0) throwUNIXException();
	struct stat st;
	if(fstat(inf.fd,&st)<0) {
		inf.destruct();
		throwUNIXException();
	}
	inf.image=(u8*)mmap(NULL,st.st_size,PROT_READ,MAP_SHARED,inf.fd,0);
	if(inf.image==NULL) {
		inf.destruct();
		throwUNIXException();
	}
	
	ELFHeader* header=(ELFHeader*)inf.image;
	//program headers
	ELFPHeader* pHeaders=(ELFPHeader*)(inf.image+header->e_phoff);	//array
	const char* loaderPath=NULL;
	//find loader
	for(int i=0;i<(int)header->e_phnum;i++) {
		if(pHeaders[i].p_type==PT_INTERP) {
			loaderPath=(char*)(inf.image+pHeaders[i].p_offset);
		}
	}
	if(loaderPath==NULL) return inf;
	
	//load loader
	inf.loaderfd=open(loaderPath,O_RDONLY);
	if(inf.loaderfd<0 || fstat(inf.loaderfd,&st)<0) {
		return inf;
		inf.destruct();
		throwUNIXException();
	}
	inf.loaderImage=(u8*)mmap(NULL,st.st_size,PROT_READ,MAP_SHARED,inf.loaderfd,0);
	if(inf.loaderImage==NULL) {
		inf.destruct();
		throwUNIXException();
	}
	return inf;
}

/*
struct ul_args_t {
	size_t	size;
	int	    cnt;
	char   *block;
};
#define ROUNDUP(x, y)   ((((x)+((y)-1))/(y))*(y))
#define ALLOCATE(size)  \
      mmap(0, (size), PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0)
#define	ALIGN(k, v)	(((k)+((v)-1))&(~((v)-1)))
#define	ALIGNDOWN(k, v)	((unsigned int)(k)&(~((unsigned int)(v)-1)))
unsigned int pgsz=4096;
*/
#define SET_STACK(esp) asm("\tmovl %0, %%esp\n" :: "r"(esp))
#define JMP_ADDR(addr) asm("\tjmp  *%0\n" :: "r" (addr))

//returns addr of new string
u8* pushString(u8*& sp, const char* s) {
	int len=strlen(s);
	sp-=(len+1);
	memcpy(sp,s,len+1);
	return sp;
}
u32* pushInt(u8*& sp, u32 i) {
	sp-=sizeof(u32);
	*(u32*)sp=i;
	return (u32*)sp;
}
template<class T>
T* pushValue(u8*& sp, const T& v) {
	sp-=sizeof(T);
	*(T*)sp=v;
	return (T*)sp;
}
void pushAuxv(u8*& sp, int type, PTR val) {
	ELFAUXV tmp;
	tmp.a_type=type;
	tmp.a_un.a_val=val;
	pushValue(sp,tmp);
}
bool auxvType_isString(u32 type) {
	return type==AT_EXECFN || type==AT_PLATFORM;
}
//set up a stack in a pre-allocated memory space
//returns new stack pointer (points to argc)
u8* setupStack(u8* top, int argc,int envc,int auxc,const char** argv,
	const char** envv,const ELFAUXV* auxv,PTR at_entry,PTR at_phdr,int at_phnum,PTR at_base) {
	char* argv1[argc];
	char* envv1[envc];
	char* auxvstr[auxc];
	u8* sp=top;
	//copy strings to new stack
	for(int i=0;i<argc;i++)
		argv1[i]=(char*)pushString(sp,argv[i]);
	for(int i=0;i<envc;i++)
		envv1[i]=(char*)pushString(sp,envv[i]);
	for(int i=0;i<auxc;i++)
		if(auxvType_isString(auxv[i].a_type))
			auxvstr[i]=(char*)pushString(sp,(char*)auxv[i].a_un.a_val);
	
	//calculate how much space we still need, so we can make sure the
	//final stack is aligned
	int spaceNeeded=sizeof(ELFAUXV)*(6+auxc)+sizeof(void*)*(1+envc+1+argc)+sizeof(int);
	u8* sp1=sp-spaceNeeded;
	//align final stack pointer
	sp1=(u8*)(PTR(sp1)&(~(PTR)0xF));
	sp=sp1+spaceNeeded;
	
	//push arrays onto stack
	ELFAUXV tmp;
	tmp.a_type=0;
	tmp.a_un.a_val=0;
	pushValue(sp,tmp);	//0,0 at end of auxv array
	
	pushAuxv(sp,AT_ENTRY,at_entry);
	pushAuxv(sp,AT_PHDR,at_phdr);
	pushAuxv(sp,AT_PHENT,sizeof(ELFPHeader));
	pushAuxv(sp,AT_PHNUM,at_phnum);
	pushAuxv(sp,AT_BASE,at_base);
	for(int i=auxc-1;i>=0;i--) {
		if(auxvType_isString(auxv[i].a_type)) {
			tmp=auxv[i];
			tmp.a_un.a_val=(PTR)auxvstr[i];
			pushValue(sp,tmp);
		} else pushValue(sp,auxv[i]);
	}
	
	//NULL pointer at end of envp array
	pushValue(sp,(void*)NULL);
	for(int i=envc-1;i>=0;i--)
		pushValue(sp,(void*)envv1[i]);
	//NULL pointer at end of argv array
	pushValue(sp,(void*)NULL);
	for(int i=argc-1;i>=0;i--)
		pushValue(sp,(void*)argv1[i]);
	//argc
	pushValue(sp,(int)argc);
	if((PTR(sp)&0xF)!=0) {
		fprintf(stderr,"stack alignment error; make sure the stack space calculation above "
		"matches the amount of data pushed onto the stack.\n");
		exit(69);
	}
	return sp;
}
extern "C" void _set_esp_and_jmp(void* sp, void* addr);
void instantiateExecutable(const executableInfo& inf, int argc,int envc,int auxc,
	const char** argv, const char** envv, const ELFAUXV* auxv) {
	u8* base;
	u8* loaderBase;
	if((loaderBase=base=mapExecutable((ELFHeader*)inf.image,inf.fd,0))==NULL) throwUNIXException();
	if(inf.loaderImage!=NULL) {
		if((loaderBase=mapExecutable((ELFHeader*)inf.loaderImage,inf.loaderfd,0))==NULL) throwUNIXException();
	}
	
	ELFHeader* header=(ELFHeader*)inf.image;
	ELFHeader* loaderHeader=(ELFHeader*)(inf.loaderImage==NULL?inf.image:inf.loaderImage);
	void* f=(void*)((PTR)loaderHeader->e_entry + loaderBase);
	
	//create new stack
	int stackSize=pgsz*1024;
	u8* stack=(u8*)mmap(NULL,stackSize,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS|MAP_GROWSDOWN|MAP_STACK,-1,0);
	if(stack==MAP_FAILED) throwUNIXException();
	memset(stack,0x13,stackSize);
	stack+=stackSize;
	printf("stack top: %p\n",stack);
	
	//const char* argv[1];
	//argv[0]="aaaaa";
	//argv[1]="/home/xaxaxa/hello";
	const char* env[1];
	env[0]="LD_SHOW_AUXV=1";
	//XXX: assumes that the virtAddr of the first segment is 0, and that the file offset
	//of the first segment is 0
	u8* sp=setupStack(stack,argc,sizeof(env)/sizeof(*env),
		auxc,argv,env,auxv,PTR(base)+PTR(header->e_entry),
		PTR(base)+PTR(header->e_phoff),header->e_phnum,(PTR)loaderBase);
	
	printf("stack pointer: %p\n",sp);
	_set_esp_and_jmp(sp,f);
	asm("hlt");
	printf("zxcvb\n");
}
void* getTCB() {
	u32 gs=0;
	void* addr=NULL;
	asm("mov %gs, %eax");
	asm("movl %%eax, %0" :: "rw" (gs));
	asm("mov %gs:0x0, %eax");
	asm("movl %%eax, %0" :: "rw" (addr));
	return addr;
}
u16 getGS() {
	u32 gs=0;
	asm("mov %gs, %eax");
	asm("movl %%eax, %0" :: "rw" (gs));
	return (u16)gs;
}
PTR _getauxv1(ELFAUXV* auxv, int type) {
	for(ELFAUXV* tmp=auxv;tmp->a_type!=0;tmp++)
		if(int(tmp->a_type)==type) return (PTR)tmp->a_un.a_val;
	return 0;
}
u8* tcb=NULL;
void* realVSyscall=NULL;
extern "C" int _handle_syscall();
extern "C" int do_handle_syscall(int a,int b,int c,int d,int e,int f,int g) {
	//asm("hlt");
	printf("syscall %i\t%08x  %08x  %08x  %08x  %08x  %08x\n",a,b,c,d,e,f,g);
	return syscall(a,b,c,d,e,f,g);
	if(a==252) {
		exit(b);
	}
	return -1;
}

extern "C" void doprint1() {
	printf("syscall called\n");
}
extern "C" void doprint2() {
	printf("syscall called.\n");
}
int main(int argc, char** argv) {
	if(argc<2) {
		printf("usage: %s program [args]\n",argv[0]);
		return 1;
	}
	/*void* lib=dlopen(argv[1],RTLD_LAZY);
	if(lib==NULL) {
		perror(dlerror()); return 2;
	}
	void* entry=dlsym(lib,"main");
	if(entry==NULL) {
		perror(dlerror()); return 2;
	}
	int (*entryFunc)(int, char**);
	entryFunc=(int(*)(int,char**))entry;
	for(int i=0;i<10;i++)entryFunc(argc-1,argv+1);*/
	tcb=(u8*)getTCB();
	
	
	
	
	
	
	//find end of env array
	char** env=argv+argc+1;
	char** envEnd=env;
	while(*envEnd!=NULL)envEnd++;
	
	ELFAUXV* auxv=(ELFAUXV*)(envEnd+1);
	ELFAUXV* auxvEnd=auxv;
	while(auxvEnd->a_type!=0)auxvEnd++;
	int auxc=auxvEnd-auxv;
	
	
	
	executableInfo inf=loadExecutable(argv[1]);
	ELFHeader* header=(ELFHeader*)inf.image;
	
	//AT_SYSINFO,AT_SYSINFO_EHDR,
	int auxvToCopy[14]={AT_HWCAP,AT_PAGESZ,AT_CLKTCK,AT_PLATFORM,
						AT_FLAGS,AT_UID,AT_EUID,AT_GID,AT_EGID,AT_SECURE,AT_RANDOM,AT_EXECFN};
	vector<ELFAUXV> myAuxv;
	myAuxv.push_back({AT_SYSINFO,(PTR)&_handle_syscall});
	ELFHeader vSyscallHeader;
	memset(&vSyscallHeader,0,sizeof(vSyscallHeader));
	vSyscallHeader.e_entry=(PTR)&_handle_syscall;
	myAuxv.push_back({AT_SYSINFO_EHDR,(PTR)&vSyscallHeader});
	for(int i=0;i<auxc;i++) {
		for(int ii=0;ii<int(sizeof(auxvToCopy)/sizeof(*auxvToCopy));ii++) {
			if(int(auxv[i].a_type)==auxvToCopy[ii])
				myAuxv.push_back(auxv[i]);
		}
		if(auxv[i].a_type==AT_SYSINFO_EHDR) {
			ELFHeader* hdr=(ELFHeader*)auxv[i].a_un.a_val;
			printf("hdr=%p\n",hdr);
			realVSyscall=(void*)(hdr->e_entry);
		}
	}
	
	u16 gs=getGS();
	printf("gs=0x%x\n",(int)gs);
	printf("realVSyscall=%p\n",realVSyscall);
	
	
	
	
	
	
	
	
	/*const char* ldso="/lib/i386-linux-gnu/ld-2.17.so";
	int fd=open(ldso,O_RDONLY);
	u8* tmp=(u8*)mmap(NULL,0x21000,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
	mmap(tmp,0x1F000,PROT_READ|PROT_EXEC,MAP_PRIVATE|MAP_FIXED,fd,0);
	mmap(tmp+0x1F000,0x1000,PROT_READ,MAP_PRIVATE|MAP_FIXED,fd,0x1f000);
	mmap(tmp+0x20000,0x1000,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_FIXED,fd,0x20000);
	void* entry=tmp+0xdc0;
	int stackSize=pgsz*1024;
	u8* stack=(u8*)mmap(NULL,stackSize,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS|MAP_GROWSDOWN|MAP_STACK,-1,0);
	stack+=stackSize;
	const char* _argv[1]={"aaaaa"};
	u8* sp=setupStack(stack,1,0,
		myAuxv.size(),_argv,NULL,&myAuxv[0],PTR(entry),
		0,0,(PTR)tmp);
	
	void* jmpto;
	fscanf(stdin,"%p",&jmpto);
	SET_STACK(sp);
	JMP_ADDR(jmpto);*/
	
	
	
	
	
	instantiateExecutable(inf,argc-1,envEnd-env,myAuxv.size(),(const char**)argv+1,
		(const char**)env,&myAuxv[0]);
	
	//*/
	
	
	//dump all stack above &argc
	//u8* sp=(u8*)argv-sizeof(int);
	//VERY UGLY!!!!!!!! ASSUMES INITIAL STACK IS SMALLER THAN 4KB
	//u8* topOfStack=(u8*)ROUNDUP(size_t(sp),pgsz);
	//write(1,sp,topOfStack-sp);
	
	return 0;
}

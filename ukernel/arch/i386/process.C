/*
 * process.C
 *
 *  Created on: Nov 3, 2013
 *      Author: xaxaxa
 */
#include "process.H"
#include "../../include/kernel.H"
#include <elf.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <vector>
#include <unistd.h>
#include "misc.H"
#include <linux/unistd.h>

#define ROUNDUP(x, y)   ((((x)+((y)-1))/(y))*(y))
#define	ALIGN(k, v)	(((k)+((v)-1))&(~((v)-1)))
#define	ALIGNDOWN(k, v)	(size_t(size_t(k)/v)*v)
namespace ukernel
{
	namespace arch
	{
		using namespace std;
		using namespace core;
		ELFHeader vSyscallHeader;
		ELFAUXV* host_auxv = nullptr;
		static const size_t pgsz = 4096;
		static void throwUNIXException() {
			throw runtime_error(strerror(errno));
		}
		void executableInfo_destruct(KernelInstance* k, executableInfo& inf) {
			if (inf.image) k->sys_munmap(inf.image, inf.imageLen);
			if (inf.loaderImage) k->sys_munmap(inf.loaderImage, inf.loaderLen);
			if (inf.fd >= 0) k->sys_close(inf.fd);
			if (inf.loaderfd >= 0) k->sys_close(inf.loaderfd);
		}
		//get page protection flags from elf program header flags
		u32 getProtectionFlags(u32 elfSegmentFlags) {
			u32 prot = 0;
			if ((elfSegmentFlags & PF_R) == PF_R) prot |= PROT_READ;
			if ((elfSegmentFlags & PF_W) == PF_W) prot |= PROT_WRITE;
			if ((elfSegmentFlags & PF_X) == PF_X) prot |= PROT_EXEC;
			if (prot == 0) prot = PROT_NONE;
			return prot;
		}

		//parses an executable but doesn't instantiate it (create the appropriate mappings for execution)
		WithError<executableInfo> loadExecutable(KernelInstance* k, const char* path) {
			executableInfo inf;
			inf.image = inf.loaderImage = NULL;
			inf.fd = inf.loaderfd = 0;
			int err;
			if ((inf.fd = k->sys_open(path, O_RDONLY)) < 0) return ErrorValue(-inf.fd);
			struct stat st;
			if ((err = k->sys_fstat(inf.fd, &st)) < 0) {
				executableInfo_destruct(k, inf);
				return ErrorValue(-err);
			}
			inf.image = (u8*) k->sys_mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, inf.fd, 0);
			if (IS_MMAP_ERROR(inf.image)) {
				executableInfo_destruct(k, inf);
				return ErrorValue(MMAP_ERROR(inf.image));
			}

			ELFHeader* header = (ELFHeader*) inf.image;
			//program headers
			ELFPHeader* pHeaders = (ELFPHeader*) (inf.image + header->e_phoff); //array
			const char* loaderPath = NULL;
			//find loader
			for (int i = 0; i < (int) header->e_phnum; i++) {
				if (pHeaders[i].p_type == PT_INTERP) {
					loaderPath = (char*) (inf.image + pHeaders[i].p_offset);
				}
			}
			if (loaderPath == NULL) return inf;

			//load loader
			inf.loaderfd = k->sys_open(loaderPath, O_RDONLY);
			if ((err = inf.loaderfd) < 0 || (err = k->sys_fstat(inf.loaderfd, &st)) < 0) {
				executableInfo_destruct(k, inf);
				return ErrorValue(-err);
			}
			inf.loaderImage = (u8*) k->sys_mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, inf.loaderfd,
					0);
			if (IS_MMAP_ERROR(inf.loaderImage)) {
				executableInfo_destruct(k, inf);
				return ErrorValue(MMAP_ERROR(inf.loaderImage));
			}
			return inf;
		}
		//maps all LOAD segments, and returns base offset (usually random)
		//assumes PIE; non-PIE executables will never be supported
		//does NOT check that offsets are not out-of-bounds
		//returns NULL on failure, and sets errno appropriately
		WithError<u8*> mapExecutable(KernelInstance* k, ELFHeader* header, int fdToMap, int fdOffset =
				0) {
			u8* file = (u8*) header;
			ELFPHeader* pHeaders = (ELFPHeader*) (file + header->e_phoff); //array
			//find the maximum among the end addresses of all mappings;
			//this is the minimum size of the contiguous memory range
			//that the whole program will fit in
			u8* mappingSize = NULL; //not an actual pointer, just an int large enough to
			//store a pointer
			for (int i = 0; i < (int) header->e_phnum; i++)
				if (pHeaders[i].p_type == PT_LOAD) {
					//end address of this mapping
					u8* endAddr = (u8*) (pHeaders[i].p_vaddr + pHeaders[i].p_memsz);
					if (endAddr > mappingSize) mappingSize = endAddr;
				}
			printf("mappingSize=0x%x\n", (size_t) mappingSize);
			//remap the whole file
			u8* base = (u8*) k->sys_mmap(NULL, (size_t) mappingSize, PROT_NONE,
			MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
			if (IS_MMAP_ERROR(base)) return ErrorValue(MMAP_ERROR(base));

			//create the real mappings
			for (int i = 0; i < (int) header->e_phnum; i++)
				if (pHeaders[i].p_type == PT_LOAD) {
					u8* mapToUnaligned = (u8*) (base + (size_t) pHeaders[i].p_vaddr);
					u8* mapTo = (u8*) ALIGNDOWN(mapToUnaligned, pgsz);
					u8* mappingEnd = mapToUnaligned + PTR(pHeaders[i].p_memsz);
					u8* dataEnd = mapToUnaligned + PTR(pHeaders[i].p_filesz);
					mappingEnd = (u8*) ROUNDUP((PTR )mappingEnd, pgsz);
					u8* fileMappingEnd = (u8*) ROUNDUP((PTR )dataEnd, pgsz);
					size_t offset = ALIGNDOWN(pHeaders[i].p_offset, pgsz);
					//size_t size=pHeaders[i].p_memsz+size_t(mapToUnaligned-mapTo);
					size_t fileMappingSize = fileMappingEnd - mapTo;
					size_t size = mappingEnd - mapTo;
					printf("mapping segment of size 0x%x to %p\n", size, mapTo);
					void* tmp = k->sys_mmap(mapTo, fileMappingSize,
					PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_FIXED, fdToMap, (fdOffset + offset));
					if (IS_MMAP_ERROR(tmp)) {
						k->sys_munmap(base, (size_t) mappingSize);
						return ErrorValue(MMAP_ERROR(tmp));
					}
					if (size > fileMappingSize) {
						//leftover memory range not covered by the backing file
						tmp = k->sys_mmap(mapTo + fileMappingSize, size - fileMappingSize,
						PROT_READ | PROT_WRITE,
						MAP_PRIVATE | MAP_FIXED | MAP_ANONYMOUS, -1, 0);
						if (IS_MMAP_ERROR(tmp)) {
							k->sys_munmap(base, (size_t) mappingSize);
							return ErrorValue(MMAP_ERROR(tmp));
						}
					}
					//int zerosz=pHeaders[i].p_memsz-pHeaders[i].p_filesz;

					int zerosz = int(fileMappingEnd - dataEnd);
					if (zerosz > 0) {
						//memset((void*)dataEnd,0,zerosz);
						for (int i = 0; i < zerosz; i++)
							dataEnd[i] = 0;
					}
					k->sys_mprotect(mapTo, size, getProtectionFlags(pHeaders[i].p_flags));
					//brk(mapToUnaligned+pHeaders[i].p_memsz);
				}
			return base;
		}
		//returns addr of new string
		u8* pushString(u8*& sp, const char* s) {
			int len = strlen(s);
			sp -= (len + 1);
			memcpy(sp, s, len + 1);
			return sp;
		}
		u32* pushInt(u8*& sp, u32 i) {
			sp -= sizeof(u32);
			*(u32*) sp = i;
			return (u32*) sp;
		}
		template<class T>
		T* pushValue(u8*& sp, const T& v) {
			sp -= sizeof(T);
			*(T*) sp = v;
			return (T*) sp;
		}
		void pushAuxv(u8*& sp, int type, PTR val) {
			ELFAUXV tmp;
			tmp.a_type = type;
			tmp.a_un.a_val = val;
			pushValue(sp, tmp);
		}
		bool auxvType_isString(u32 type) {
			return type == AT_EXECFN || type == AT_PLATFORM;
		}
		//set up a stack in a pre-allocated memory space
		//returns new stack pointer (points to argc)
		u8* setupStack(u8* top, int argc, int envc, int auxc, const char** argv, const char** envv,
				const ELFAUXV* auxv, PTR at_entry, PTR at_phdr, int at_phnum, PTR at_base) {
			char* argv1[argc];
			char* envv1[envc];
			char* auxvstr[auxc];
			u8* sp = top;
			//leave space for threadInfo at top of stack
			sp -= sizeof(threadInfo);
			memset(sp, 0, sizeof(threadInfo));

			//copy strings to new stack
			for (int i = 0; i < argc; i++)
				argv1[i] = (char*) pushString(sp, argv[i]);
			for (int i = 0; i < envc; i++)
				envv1[i] = (char*) pushString(sp, envv[i]);
			for (int i = 0; i < auxc; i++)
				if (auxvType_isString(auxv[i].a_type)) auxvstr[i] = (char*) pushString(sp,
						(char*) auxv[i].a_un.a_val);

			//calculate how much space we still need, so we can make sure the
			//final stack is aligned
			int spaceNeeded = sizeof(ELFAUXV) * (6 + auxc) + sizeof(void*) * (1 + envc + 1 + argc)
					+ sizeof(int);
			u8* sp1 = sp - spaceNeeded;
			//align final stack pointer
			sp1 = (u8*) (PTR(sp1) & (~(PTR) 0xF));
			sp = sp1 + spaceNeeded;

			//push arrays onto stack
			ELFAUXV tmp;
			tmp.a_type = 0;
			tmp.a_un.a_val = 0;
			pushValue(sp, tmp); //0,0 at end of auxv array

			pushAuxv(sp, AT_ENTRY, at_entry);
			pushAuxv(sp, AT_PHDR, at_phdr);
			pushAuxv(sp, AT_PHENT, sizeof(ELFPHeader));
			pushAuxv(sp, AT_PHNUM, at_phnum);
			pushAuxv(sp, AT_BASE, at_base);
			for (int i = auxc - 1; i >= 0; i--) {
				if (auxvType_isString(auxv[i].a_type)) {
					tmp = auxv[i];
					tmp.a_un.a_val = (PTR) auxvstr[i];
					pushValue(sp, tmp);
				} else pushValue(sp, auxv[i]);
			}

			//NULL pointer at end of envp array
			pushValue(sp, (void*) NULL);
			for (int i = envc - 1; i >= 0; i--)
				pushValue(sp, (void*) envv1[i]);
			//NULL pointer at end of argv array
			pushValue(sp, (void*) NULL);
			for (int i = argc - 1; i >= 0; i--)
				pushValue(sp, (void*) argv1[i]);
			//argc
			pushValue(sp, (int) argc);
			if ((PTR(sp) & 0xF) != 0) {
				fprintf(stderr, "stack alignment error; make sure the stack space calculation above "
						"matches the amount of data pushed onto the stack.\n");
				exit(69);
			}
			return sp;
		}
		extern "C" void _set_esp_and_jmp(void* sp, void* addr);
		int mapProcess(KernelInstance* k, ProcessImage& p) {
			auto tmp = mapExecutable(k, (ELFHeader*) p.inf.image, p.inf.fd, 0);
			if (!tmp) return -tmp.error;
			p.base = p.loaderBase = tmp();
			if (p.inf.loaderImage != NULL) {
				auto tmp1 = mapExecutable(k, (ELFHeader*) p.inf.loaderImage, p.inf.loaderfd, 0);
				if (!tmp1) return -tmp1.error;
				p.loaderBase = tmp1();
			}
			return 0;
		}
		extern "C" void _call_child_process(void* new_stack, void* entry, void** put_sp);
		extern "C" void _call_child_process_returnto();
		int instantiateExecutable(KernelInstance* k, ProcessImage& img, int argc, int envc, int auxc,
				const char** argv, const char** envv, const ELFAUXV* auxv) {
			executableInfo& inf = img.inf;
			u8* base = img.base;
			u8* loaderBase = img.loaderBase;

			ELFHeader* header = (ELFHeader*) inf.image;
			ELFHeader* loaderHeader = (ELFHeader*) (
					inf.loaderImage == NULL ? inf.image : inf.loaderImage);
			void* f = (void*) ((PTR) loaderHeader->e_entry + loaderBase);

			//create new stack
			//user stacks are always aligned to the stack size
			u8* stack = (u8*) aligned_alloc(stackSize, stackSize);
			if (stack == nullptr) return -errno;
			stack += stackSize;
			printf("stack top: %p\n", stack);
			threadInfo* ti = (threadInfo*) (stack - sizeof(threadInfo));
			//XXX: assumes that the virtAddr of the first segment is 0, and that the file offset
			//of the first segment is 0
			u8* sp = setupStack(stack, argc, envc, auxc, argv, envv, auxv,
					PTR(base) + PTR(header->e_entry), PTR(base) + PTR(header->e_phoff), header->e_phnum,
					(PTR) loaderBase);

			int gs = 0;
			asm volatile("mov %%gs,%0" :: "rw"(gs));
			ti->kernel_gs = gs;
			printf("gs=0x%x\n", gs);
			volatile int exitCode;
			ti->putExitCode = (int*) &exitCode;
			ti->kernel_return_addr = (u8*) &_call_child_process_returnto;
			ti->kernel = k;

			//printf("stack pointer: %p\n", sp);
			_call_child_process(sp, f, (void**) &ti->kernel_return_sp);
			asm volatile("":::"memory");
			//printf("zxcvb %i\n", exitCode);
			return exitCode;
		}
		int ProcessImage::call(KernelInstance* k, int argc, int envc, const char** argv,
				const char** envv) {
			int auxvToCopy[14] = { AT_HWCAP, AT_PAGESZ, AT_CLKTCK, AT_PLATFORM,
			AT_FLAGS, AT_UID, AT_EUID, AT_GID, AT_EGID, AT_SECURE, AT_RANDOM, AT_EXECFN };
			vector<ELFAUXV> myAuxv;
			if (host_auxv != nullptr) {
				ELFAUXV* auxvEnd = host_auxv;
				while (auxvEnd->a_type != 0)
					auxvEnd++;
				int auxc = auxvEnd - host_auxv;
				for (int i = 0; i < auxc; i++) {
					for (int ii = 0; ii < int(sizeof(auxvToCopy) / sizeof(*auxvToCopy)); ii++) {
						if (int(host_auxv[i].a_type) == auxvToCopy[ii]) myAuxv.push_back(host_auxv[i]);
					}
				}
			}
			myAuxv.push_back( { AT_SYSINFO, (PTR) &_handle_syscall });
			vSyscallHeader.e_entry = (PTR) &_handle_syscall;
			myAuxv.push_back( { AT_SYSINFO_EHDR, (PTR) &vSyscallHeader });
			myAuxv.push_back( { AT_PAGESZ, PAGESIZE });
			return instantiateExecutable(k, *this, argc, envc, myAuxv.size(), argv, envv, &myAuxv[0]);
		}
		void setHostAuxv(int argc, char** argv) {
			char** env = argv + argc + 1;
			char** envEnd = env;
			while (*envEnd != NULL)
				envEnd++;
			host_auxv = (ELFAUXV*) (envEnd + 1);
		}
		void exitProcessImage(int ret) {
			threadInfo* ti = get_threadInfo();
			fprintf(stderr, "ret=%i\n", ret);
			*(ti->putExitCode) = ret;
			fprintf(stderr, "kernel_return_addr=%p\n", ti->kernel_return_addr);
			_set_esp_and_jmp(ti->kernel_return_sp, ti->kernel_return_addr);
		}
		WithError<ProcessImage*> loadProcess(core::KernelInstance* k, string path) {
			ProcessImage* tmp = New<ProcessImage>();
			auto inf = loadExecutable(k, path.c_str());
			if (!inf) return ErrorValue(inf.error);
			tmp->inf = inf();
			int err;
			if ((err = mapProcess(k, *tmp)) != 0) {
				executableInfo_destruct(k, tmp->inf);
				Delete(tmp);
				return ErrorValue(err);
			}
			return tmp;
		}
		typedef KernelInstance K;
		void** initSyscallTable() {
			void** tmp = (void**) malloc(sizeof(void*) * 512);
			memset(tmp, 0, sizeof(void*) * 512);
			tmp[__NR_exit] = &K::sys_exit;
			tmp[__NR_read] = &K::sys_read;
			tmp[__NR_write] = &K::sys_write;
			tmp[__NR_open] = &K::sys_open;
			tmp[__NR_openat] = &K::sys_openat;
			tmp[__NR_close] = &K::sys_close;
			tmp[__NR_nanosleep] = &K::sys_nanosleep;
			tmp[__NR_mmap2] = &K::sys_mmap2;
			tmp[__NR_stat64] = &K::sys_stat;
			tmp[__NR_lstat64] = &K::sys_lstat;
			tmp[__NR_fstat64] = &K::sys_fstat;
			tmp[__NR_getdents64] = &K::sys_getdents;
			tmp[__NR_statfs64] = &K::sys_statfs;
			tmp[__NR_fstatfs64] = &K::sys_fstatfs;
			tmp[__NR_exit_group] = &K::sys_exit;
			return tmp;
		}
		inline threadInfo* get_threadInfo() {
			ptr_int sp = 0;
			asm volatile("movl %%esp,%0" :: "rw"(sp));

			//get stack top
			//user stacks are always aligned to the stack size
			sp = ptr_int(sp / stackSize) * stackSize;
			sp += stackSize;
			//asm volatile("hlt");
			return (threadInfo*) (sp - sizeof(threadInfo));
		}
		extern "C" int do_handle_syscall(int a, int b, int c, int d, int e, int f, int g) {
			//asm("hlt");
			fprintf(stderr, "syscall %i\t%08x  %08x  %08x  %08x  %08x  %08x = ", a, b, c, d, e, f, g);
			threadInfo* ti = get_threadInfo();
			KernelInstance* k = ti->kernel;
			int ret;
			if (k->syscallTable[a] == nullptr) ret = syscall(a, b, c, d, e, f, g);
			else {
				int (*func)(void*, int, int, int, int, int,
						int)=(int(*)(void*,int,int,int,int,int,int))k->syscallTable[a];
				ret = func(k, b, c, d, e, f, g);
			}
			fprintf(stderr, "%i\n", ret);
			return ret;
		}
		extern "C" int _get_kthread_gs() {
			//return 0x63;
			threadInfo* inf = get_threadInfo();
			return inf->kernel_gs;
		}
	}
}

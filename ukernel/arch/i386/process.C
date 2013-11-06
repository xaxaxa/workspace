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

#define ROUNDUP(x, y)   ((((x)+((y)-1))/(y))*(y))
#define	ALIGN(k, v)	(((k)+((v)-1))&(~((v)-1)))
#define	ALIGNDOWN(k, v)	(size_t(size_t(k)/v)*v)
namespace ukernel
{
	namespace arch
	{
		using namespace core;
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
			inf.image = (u8*) k->sys_mmap2(NULL, st.st_size, PROT_READ, MAP_SHARED, inf.fd, 0);
			if (inf.image == NULL) {
				executableInfo_destruct(k, inf);
				return ErrorValue(EINVAL);
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
			inf.loaderfd = open(loaderPath, O_RDONLY);
			if ((err = inf.loaderfd) < 0 || (err = fstat(inf.loaderfd, &st)) < 0) {
				executableInfo_destruct(k, inf);
				return ErrorValue(-err);
			}
			inf.loaderImage = (u8*) mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, inf.loaderfd, 0);
			if (inf.loaderImage == NULL) {
				executableInfo_destruct(k, inf);
				return ErrorValue(EINVAL);
			}
			return inf;
		}
		void ProcessImage::call() {
		}
		void ProcessImage::exit() {
		}
		ProcessImage* loadProcess(core::FileDescription* fd) {

		}
	}
}

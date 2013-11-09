/*
 * ukernel.h
 *
 *  Created on: Oct 20, 2013
 *      Author: xaxaxa
 */

#ifndef CORE_H_
#define CORE_H_
#include <stdint.h>
#include <string>
#include <map>
#include <set>
#include <sys/stat.h>
#include <stack>
#include <list>
#include "util.H"
#include "path.H"
using namespace std;
namespace ukernel
{
	typedef int64_t s64;
	typedef uint64_t u64;
	typedef int32_t s32;
	typedef uint32_t u32;
	typedef int16_t s16;
	typedef uint16_t u16;
	typedef int8_t s8;
	typedef uint8_t u8;
	namespace FS
	{
		struct linux_dirent64;
		struct DirectoryEntry;
	}
	namespace core
	{
		struct Constructor
		{
			//call construct() to construct an instance
			//in an already-allocated space (passed in as
			//the void pointer)
			Delegate<void(void*)> construct;
			//size (in bytes) that each instance needs
			int objSize;
		};
		struct EventQueue
		{
			void signal(Delegate<void()> f);
			Delegate<void()> wait();
			void processEvent();
		};
		struct EventCallback
		{
			EventQueue* queue;
			Delegate<void()> f;
			EventCallback() :
					queue(NULL) {
			}
			void signal() {
				queue->signal(f);
			}
			void operator()() {
				signal();
			}
		};
		//wait for an async event synchronously
		template<class T>
		struct SyncWaiter
		{
			T value;
			int error;
			void operator()(T value, int err) {
				this->value = value;
				this->error = err;
			}
			SyncWaiter(AsyncValue<T>& av, EventQueue& q) {
				error = -1;
				av.wait(this);
				while (error == -1)
					q.processEvent();
			}
		};
		template<class T> static inline void syncWait(AsyncValue<T>& av, EventQueue& q) {
			struct CB
			{
				AsyncValue<T>& av;
				void operator()(T value, int err) {
					av.value = value;
					av.error = err;
				}
			} cb { av };
			while (cb.av.error == -1)
				q.processEvent();
		}
		struct KernelInstance;
		struct FileDescription: public RefCount
		{
			KernelInstance* kernel;
			enum
			{
				T_File = 1, T_Socket
			} type;
			virtual AsyncValue<int> read(u8* buffer, s64 length)=0;
			virtual AsyncValue<int> write(u8* buffer, s64 length)=0;
			virtual AsyncValue<int> getdents(FS::linux_dirent64* buffer, int bufSize)=0;
			virtual int stat(struct stat *buf)=0;
			int sread(u8* buffer, s64 length);
			int swrite(u8* buffer, s64 length);
			virtual u8* mmap(u8* addr, ptr_int length, s64 offset, int prot, int flags)=0;
			virtual ~FileDescription() {
			}
		};
		//a file or directory fd
		struct FileDescription_File: public FileDescription
		{
			FS::Path path;
			s64 offset;
			int flags;
			AsyncValue<int> read(u8* buffer, s64 length) override;
			AsyncValue<int> write(u8* buffer, s64 length) override;
			AsyncValue<int> getdents(FS::linux_dirent64* buffer, int bufSize) override;
			int stat(struct stat *buf) override;
			u8* mmap(u8* addr, ptr_int length, s64 offset, int prot, int flags) override;
			FileDescription_File();
			~FileDescription_File() override;
		};
		struct FileDescription_HostFD: public FileDescription
		{
			int fd;
			bool noClose;
			AsyncValue<int> read(u8* buffer, s64 length) override;
			AsyncValue<int> write(u8* buffer, s64 length) override;
			AsyncValue<int> getdents(FS::linux_dirent64* buffer, int bufSize) override;
			int stat(struct stat *buf) override;
			u8* mmap(u8* addr, ptr_int length, s64 offset, int prot, int flags) override;
			FileDescription_HostFD(int fd, bool noClose = false);
			~FileDescription_HostFD() override;
		};
		struct FDTable
		{
			IDSet fds;
		};
	}
}

#endif

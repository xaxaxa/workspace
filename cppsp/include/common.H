/*
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * */
/*
 * common.H
 *
 *  Created on: Apr 6, 2013
 *      Author: xaxaxa
 */

#ifndef CPPSP_COMMON_H_
#define CPPSP_COMMON_H_
#include <cpoll/cpoll.H>
#include <vector>
#include <unordered_map>
#include "page.H"
#include "stringutils.H"
using namespace std;
using CP::AsyncValue;
using CP::Future;
namespace cppsp
{
	extern const char* gxx;
	class ParserException: public std::exception
	{
	public:
		string message;
		int32_t number;
		ParserException();
		ParserException(int32_t number);
		ParserException(string message, int32_t number = 0);
		~ParserException() throw ();
		const char* what() const throw ();
	};
	class CompileException: public std::exception
	{
	public:
		string message;
		string compilerOutput;
		CompileException();
		CompileException(string message);
		~CompileException() throw ();
		const char* what() const throw ();
	};
	class cppspManager;
	/**
	 Internal API.
	 */
	class loadedPage
	{
	public:
		cppspManager* manager;
		loadedPage& operator=(const loadedPage& other) = delete;
		timespec lastLoad { 0, 0 }; //CLOCK_REALTIME
		timespec lastCheck { 0, 0 }; //CLOCK_MONOTONIC
		void* dlHandle;
		const uint8_t* stringTable;
		int stringTableLen;
		typedef int (*getObjectSize_t)();
		typedef Page* (*createObject_t)(void* mem);
		typedef Page* (*createObject1_t)(RGC::Allocator* alloc);
		typedef void (*initModule_t)(ModuleParams& p);
		typedef void (*deinitModule_t)();
		int (*getObjectSize)();
		createObject_t createObject;
		createObject1_t createObject1;
		RGC::Ref<CP::File> compile_fd;
		CP::MemoryStream ms;
		//Delegate<void(loadedPage&)> compileCB;
		struct _loadCB
		{
			Delegate<void(loadedPage*, exception* ex)> cb;
		};
		vector<_loadCB> loadCB;
		string tmpDir;
		string path;
		string cPath;
		string txtPath;
		string dllPath;
		int stringTableFD;
		pid_t compilerPID;
		bool loaded;
		bool compiling;
		void readCB(int r);
		void deleteTmpfiles();
		void afterCompile(bool success);
		void beginRead();
		void doCompile(CP::Poll& p, string wd, const vector<string>& cxxopts);
		void doLoad();
		void doUnload();
		Page* doCreate(RGC::Allocator* a);
		loadedPage();
		~loadedPage();
		//returns: 0: no-op; 1: should reload; 2: should recompile
		int shouldCompile();
	};
	/**
	 Internal API.
	 */
	struct staticPage
	{
		String data;
		timespec lastLoad { 0, 0 }; //CLOCK_REALTIME
		timespec lastCheck { 0, 0 }; //CLOCK_MONOTONIC
		timespec lastUse; //CLOCK_MONOTONIC
		string path;
		bool loaded;
		void doLoad();
		void doUnload();
		bool shouldReload();
		staticPage();
		~staticPage();
	};
	/**
	 Internal API.
	 */
	class cppspManager
	{
	public:
		/**
		 Server instance that is passed to initModule() of all modules
		 */
		Server* srv;
		unordered_map<String, loadedPage*> cache;
		unordered_map<String, staticPage*> staticCache;
		vector<string> cxxopts;
		timespec curTime { 0, 0 };
		string tmpDir;
		int threadID;
		cppspManager();
		AsyncValue<loadedPage*> loadPage(CP::Poll& p, String wd, String path);
		/**
		 Delete old cache entries.
		 @param minAge the minimum age (in seconds since the entry was last accessed) for an entry to be deleted
		 @unmaintained_api
		 */
		void cleanCache(int minAge);
		staticPage* loadStaticPage(String path);
		bool shouldCheck(loadedPage& p);
		bool shouldCheck(staticPage& p);
		void updateTime() {
			clock_gettime(CLOCK_MONOTONIC, &curTime);
		}
	};
	int tsCompare(struct timespec time1, struct timespec time2);

	/**
	 Internal function. Parses a .cppsp page.
	 @unmaintained_api
	 */
	void doParse(const char* name, const char* in, int inLen, CP::Stream& out, CP::Stream& st_out,
			vector<string>& c_opts);
	/**
	 Internal function. Compiles a .cppsp page.
	 @return file descriptor connected to the standard output of the compiler.
	 @unmaintained_api
	 */
	CP::File* compilePage(string wd, string path, const vector<string>& cxxopts, pid_t& pid);

	/**
	 Internal function.
	 @unmaintained_api
	 */
	void handleError(exception* ex, cppsp::Response& resp, String path);
}
#endif /* COMMON_H_ */

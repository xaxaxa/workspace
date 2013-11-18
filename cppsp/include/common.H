/*
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 2 of the License, or
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
#include "stringutils.H"
using namespace std;
using CP::AsyncValue;
using CP::Future;
namespace cppsp
{
	struct Page;
	struct Server;
	struct Response;
	class loadedPage;
	class Host;
	extern const char* gxx;
	class ParserException: public std::exception
	{
	public:
		string message;
		int32_t number;
		ParserException();
		ParserException(string message);
		ParserException(string message, int line);
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
	//the ModuleParams struct itself (passed in to modules at initialization time) is not
	//guaranteed to exist after initModule() returns
	struct ModuleParams
	{
		Host* host; //guaranteed to exist until after deinitModule() returns
		Server* server; //guaranteed to exist until after deinitModule() returns, if not null
		String filePath; //guaranteed to exist until after deinitModule() returns
		loadedPage* page;
	};
	struct ModuleInfo
	{
		//module name, not filename
		string name;
		void reset() {
			name = "";
		}
	};
	/**
	 Internal API.
	 */
	class loadedPage: public RGC::Object
	{
	public:
		cppspManager* manager;
		loadedPage& operator=(const loadedPage& other) = delete;
		timespec lastLoad { 0, 0 }; //CLOCK_REALTIME
		timespec lastCheck { 0, 0 }; //CLOCK_MONOTONIC
		ModuleInfo info;
		void* dlHandle;
		const uint8_t* stringTable;
		int stringTableLen;
		typedef int (*getObjectSize_t)();
		typedef Page* (*createObject_t)(void* mem);
		typedef Page* (*createObject1_t)(RGC::Allocator* alloc);
		typedef void* (*initModule_t)(ModuleParams& p);
		typedef void (*getModuleInfo_t)(ModuleInfo& inf);
		typedef void (*deinitModule_t)(void*);
		int (*getObjectSize)();
		createObject_t createObject;
		createObject1_t createObject1;
		initModule_t initModule;
		deinitModule_t deinitModule;
		getModuleInfo_t getModuleInfo;
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
		int moduleCount;
		bool loaded;
		bool compiling;
		void readCB(int r);
		void deleteTmpfiles();
		void afterCompile(bool success, bool sync);
		void beginRead();
		//returns whether compilation was completed synchronously
		bool doCompile(CP::Poll& p, string wd, const vector<string>& cxxopts);
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
	struct staticPage: public RGC::Object
	{
		String data;
		String mime;
		int64_t fileLen; //the "len" field in "data" is 32 bit; "data" should not be used for files >2GB
		timespec lastLoad { 0, 0 }; //CLOCK_REALTIME
		timespec lastCheck { 0, 0 }; //CLOCK_MONOTONIC
		string path;
		int fd;
		bool loaded;
		void _loadFD();
		void _loadMap();
		/**
		 @param keepFD whether to keep file descriptor open (if false, it will be closed before returning)
		 @param map whether to map the file into virtual memory
		 */
		void doLoad(bool keepFD, bool map);
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
		unordered_map<String, loadedPage*> cache;
		unordered_map<String, staticPage*> staticCache;
		unordered_map<string, string> mimeTypes; //only for static pages
		vector<string> cxxopts;
		timespec curTime { 0, 0 }; //CLOCK_MONOTONIC
		string tmpDir;
		int threadID;
		//if true, do not delete temporary .C and .so files
		bool debug;
		cppspManager();
		AsyncValue<loadedPage*> loadPage(CP::Poll& p, String wd, String path);
		staticPage* loadStaticPage(String path, bool fd = false, bool map = true);
		/**
		 Delete old cache entries.
		 @param minAge the minimum age (in seconds since the entry was last accessed) for an entry to be deleted
		 @returns whether or not there are any non-persistent items left
		 @unmaintained_api
		 */
		bool cleanCache(int minAge);
		bool shouldCheck(loadedPage& p);
		bool shouldCheck(staticPage& p);
		void updateTime() {
			clock_gettime(CLOCK_MONOTONIC, &curTime);
		}
		void loadMimeDB(CP::StreamReader& in);
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

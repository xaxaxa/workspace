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
 * common.C
 *
 *  Created on: Apr 6, 2013
 *      Author: xaxaxa
 */
#include <cpoll/cpoll.H>
#include <string>
#include <string.h>
#include <exception>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdexcept>
#include <fcntl.h>
#include <dlfcn.h>
#include <libgen.h>
#include <sys/wait.h>
#include "include/common.H"
#include "include/page.H"
#include <errno.h>
#include "include/split.H"
#include <libgen.h>

using namespace CP;
using namespace std;
namespace cppsp
{
	PThreadMutex dlMutex;
	const char* gxx = "g++";
	ParserException::ParserException() :
			message("Parser error") {
	}
	ParserException::ParserException(string message) :
			message("Parser error: " + message) {
	}
	ParserException::ParserException(string message, int line) {
		char tmp[16];
		snprintf(tmp, sizeof(tmp), "%i", line);
		this->message = "Parser error: line " + string(tmp) + ": " + message;
	}
	ParserException::~ParserException() throw () {
	}
	const char* ParserException::what() const throw () {
		return message.c_str();
	}
	CompileException::CompileException() :
			message("Compilation error") {
	}
	CompileException::CompileException(string message) :
			message(message) {
	}
	CompileException::~CompileException() throw () {
	}
	const char* CompileException::what() const throw () {
		return message.c_str();
	}

	//parses a cppsp page and generates code (to out) and string table (to st_out)
	void doParse(const char* name, const char* in, int inLen, Stream& out, Stream& st_out,
			vector<string>& c_opts) {
		const char* s = in;
		const char* end = s + inLen;
		string inherits = "public Page";
		string classname = (name == NULL ? "__cppsp_unnamed_page" : name);
		int st_pos = 0;
		int st_len = 0;
		//int out_initlen=out.length();
		Stream& ms1(out); //declarations outside of the class
		MemoryStream ms2; //declarations outside of the render() function
		MemoryStream ms3; //code inside render()
		StreamWriter sw1(ms1);
		StreamWriter sw2(ms2);
		StreamWriter sw3(ms3);
		sw1.write("#include <cppsp/page.H>\n#include <cpoll/cpoll.H>\n");
		sw1.write("#include <cppsp/common.H>\n#include <cppsp/stringutils.H>\n");
		sw1.write("#include <rgc.H>\n");
		sw1.write("using namespace cppsp;\nusing namespace CP;\n");
		int line = 1;
		while (true) {
			if (s >= end) break;
			const char* old_s = s;
			s = (const char*) memmem(old_s, end - old_s, "<%", 2);
			const char* s_2 = (const char*) memmem(old_s, end - old_s, "<!--#include", 12);
			bool is_ssi = false;
			if (s_2 != NULL && (s_2 < s || s == NULL)) {
				s = s_2;
				is_ssi = true;
			}

			if (s > old_s) {
				st_out.write(old_s, s - old_s);
				st_len += (s - old_s);
			} else if (s == NULL) {
				st_out.write(old_s, end - old_s);
				st_len += (end - old_s);

				sw3.writeF("__writeStringTable(%i,%i);\n", st_pos, st_len - st_pos);
				break;
			}
			for (const char* ch = old_s; ch < s; ch++)
				if (*ch == '\n') line++;
			s += is_ssi ? 12 : 2;
			if (s >= end)
				throw ParserException(
						"reached EOF when looking past " + is_ssi ? "<!--#include" : "\"<%\"", line);
			const char* s1;
			if (is_ssi)
				s1 = (const char*) memmem(s, end - s, "-->", 3);
			else s1 = (const char*) memmem(s, end - s, "%>", 2);
			if (s1 == NULL)
				throw ParserException(
						"reached EOF when looking for matching " + is_ssi ? "-->" : "\"%>\"", line);

			if (is_ssi) {
				const char* s_f = (const char*) memmem(s, s1 - s, "file=", 5);
				if (s_f == nullptr) throw ParserException("SSI without 'file=' parameter", line);
				s_f += 5;
				if (s_f >= end || *s_f != '"')
					throw ParserException("SSI filenames must be surrounded with '\"'", line);
				s_f++;
				const char* e_f = (const char*) memchr(s_f, '"', end - s_f);
				if (e_f == NULL)
					throw ParserException("reached EOF when looking for matching '\"'", line);
				sw3.writeF("__writeStringTable(%i,%i);\n", st_pos, st_len - st_pos);
				st_pos = st_len;
				sw3.writeF("#line %i\n", line);
				sw3.writeF("staticInclude(\"");
				sw3.write(s_f, e_f - s_f);
				sw3.write("\");\n");
			} else switch (*s) {
				case '!':
				{ //compiler option
					c_opts.push_back(string(s + 1, s1 - s - 1));
					break;
				}
				case '@':
				{ //cppsp options
					int nextopt = 0;
					optionParser spl(s + 1, s1 - s - 1);
					while (spl.read()) {
						const char* s1 = spl.value.data();
						int l1 = spl.value.length();
						switch (nextopt) {
							case 0:
							{
								if (l1 == 8 && memcmp(s1, "inherits", 8) == 0)
									nextopt = 1;
								else if (l1 == 5 && memcmp(s1, "class", 5) == 0) nextopt = 2;
								continue;
							}
							case 1:
							{
								inherits = string(s1, l1);
								break;
							}
							case 2:
							{
								if (name == NULL) classname = string(s1, l1);
								break;
							}
						}
						nextopt = 0;
					}
					break;
				}
				case '#':
				{ //declarations outside of the class
					sw1.writeF("#line %i\n", line);
					sw1.write(s + 1, s1 - s - 1);
					break;
				}
				case '$':
				{ //declarations outside of the render() function
					sw2.writeF("#line %i\n", line);
					sw2.write(s + 1, s1 - s - 1);
					break;
				}
				case '=':
				{
					sw3.writeF("__writeStringTable(%i,%i);\n", st_pos, st_len - st_pos);
					st_pos = st_len;

					sw3.writeF("#line %i\n", line);
					sw3.write("output.write(");
					sw3.write(s + 1, s1 - s - 1);
					sw3.write(");\n");
					break;
				}
				default:
				{
					sw3.writeF("__writeStringTable(%i,%i);\n", st_pos, st_len - st_pos);
					st_pos = st_len;

					sw3.writeF("#line %i\n", line);
					sw3.write(s, s1 - s);
					break;
				}
			}
			for (const char* ch = s; ch < s1; ch++)
				if (*ch == '\n') line++;
			s = s1 + int(is_ssi ? 3 : 2);
		}
		sw2.flush();
		sw3.flush();
		sw1.writeF("class %s: %s {\npublic:\n", classname.c_str(), inherits.c_str());
		sw1.write(ms2.data(), ms2.length());
		//the name of the StreamWriter parameter should always be "output" -- this is part of
		//the cppsp API and should not ever be changed; users can rely on its name being "output".
		sw1.write("virtual void render(StreamWriter& output) override {\n");
		sw1.write(ms3.data(), ms3.length());
		sw1.write("}\n};\n");
		sw1.writeF("extern \"C\" int getObjectSize() {return sizeof(%s);}\n", classname.c_str());
		sw1.writeF("extern \"C\" Page* createObject(void* mem) {"
				"if(mem==NULL) return new %s(); else return new (mem) %s();}\n", classname.c_str(),
				classname.c_str());
		sw1.writeF("extern \"C\" Page* createObject1(RGC::Allocator* alloc) {"
				"%s* tmp = new (alloc->alloc(sizeof(%s))) %s(); tmp->allocator=alloc; return tmp;}\n",
				classname.c_str(), classname.c_str(), classname.c_str());
		sw1.flush();
		//out.write(ms1.data(), ms1.length());
	}
	static inline int checkError(int i) {
		if (i < 0) throwUNIXException();
		return i;
	}
	static inline int checkError(int i, String fn) {
		if (i < 0) throwUNIXException(fn.toSTDString());
		return i;
	}
	static inline int checkError(int i, string fn) {
		if (i < 0) throwUNIXException(fn);
		return i;
	}
	static inline void* checkError(void* p) {
		if (p == NULL) throw runtime_error(strerror(errno));
		return p;
	}
	static inline void* checkDLError(void* p) {
		if (p == NULL) throw runtime_error(dlerror());
		return p;
	}
	CP::File* compilePage(string wd, string path, string cPath, string txtPath, string output,
			const vector<string>& cxxopts, pid_t& pid, string& compilecmd) {
		vector<string> c_opts { gxx, gxx, "--std=c++0x", "--shared", "-o", output, cPath };
		c_opts.insert(c_opts.end(), cxxopts.begin(), cxxopts.end());
		{
			File inp(open(path.c_str(), O_RDONLY | O_CLOEXEC));
			MemoryStream ms;
			inp.readToEnd(ms);
			ms.flush();
			//unlink((path + ".C").c_str());
			File out_c(open(cPath.c_str(), O_RDWR | O_CREAT | O_TRUNC | O_CLOEXEC, 0666));
			File out_s(open(txtPath.c_str(), O_RDWR | O_CREAT | O_TRUNC | O_CLOEXEC, 0666));
			cppsp::doParse(NULL, (const char*) ms.data(), ms.length(), out_c, out_s, c_opts);
		}

		const char* cmds[c_opts.size() + 1];
		for (int i = 0; i < (int) c_opts.size(); i++) {
			cmds[i] = c_opts[i].c_str();
		}
		cmds[c_opts.size()] = NULL;

		int p[2];
		checkError(pipe(p));
		//unlink((path + ".so").c_str());
		pid_t tmp = fork();
		if (tmp > 0) {
			close(p[1]);
			pid = tmp;
			for (int i = 1; i < (int) c_opts.size(); i++) {
				compilecmd.append(c_opts[i]);
				compilecmd.append(" ");
			}
			return new CP::File(p[0]);
		} else if (tmp == 0) {
			close(p[0]);
			dup2(p[1], 1);
			dup2(p[1], 2);
			close(p[1]);
			chdir(wd.c_str());
			execvp(cmds[0], (char**) (cmds + 1));
			_exit(1);
		} else {
			checkError(tmp);
		}
		return NULL;
	}
	int tsCompare(struct timespec time1, struct timespec time2) {
		if (time1.tv_sec < time2.tv_sec)
			return (-1); /* Less than. */
		else if (time1.tv_sec > time2.tv_sec)
			return (1); /* Greater than. */
		else if (time1.tv_nsec < time2.tv_nsec)
			return (-1); /* Less than. */
		else if (time1.tv_nsec > time2.tv_nsec)
			return (1); /* Greater than. */
		else return (0); /* Equal. */
	}

#define CONCAT_TO(in,inLen,out,conc) char out[strlen(conc)+inLen+1];\
	memcpy(out,in,inLen);\
	memcpy(out+inLen,conc,strlen(conc));\
	out[strlen(conc)+inLen]=0;
#define TO_C_STR(in,inLen,out) char out[inLen+1];\
	memcpy(out,in,inLen);\
	out[inLen]=0;
	static inline void pageErr_isDir() {
		throw ParserException("requested path is a directory or socket");
	}
	static inline void replace(String s, char f, char t) {
		for (int i = 0; i < s.length(); i++)
			if (s[i] == f) s[i] = t;
	}
	//path for persistent binaries
	static inline string loadedPage_getBinPath(loadedPage* This) {
		if (This->tmpDir.length() == 0) return This->path;
		string path1(This->path.data(), This->path.length()); //make sure it's a copy
		String path1_(path1);
		replace(path1_, '/', '_');
		return This->tmpDir + "/" + path1;
	}
	void loadedPage::readCB(int r) {
		if (r <= 0) {
			compiling = false;
			int status = -1;
			waitpid(compilerPID, &status, 0);

			//keep a copy of the compiled page
			if (status == 0) {
				if (!manager->debug) unlink(cPath.c_str());
				string dll1 = dllPath + ".1";
				link(dllPath.c_str(), dll1.c_str());
				string txt1 = txtPath + ".1";
				link(txtPath.c_str(), txt1.c_str());

				string binPath = loadedPage_getBinPath(this);
				rename(dll1.c_str(), (binPath + ".so").c_str());
				rename(txt1.c_str(), (binPath + ".txt").c_str());
			}
			afterCompile(status == 0, false);
			compile_fd = nullptr;
			//if (compileCB != nullptr) compileCB(*this);
			return;
		}
		ms.bufferPos += r;
		ms.flush();
		beginRead();
	}
	void loadedPage::deleteTmpfiles() {
		if (!manager->debug) {
			unlink(txtPath.c_str());
			unlink(dllPath.c_str());
		}
	}
	void loadedPage::afterCompile(bool success, bool sync) {
		if (!success) {
			rename(cPath.c_str(), (loadedPage_getBinPath(this) + ".C").c_str());
			deleteTmpfiles();
			CompileException exc;
			exc.compilerOutput = string((const char*) ms.data(), ms.length());
			auto tmpcb = loadCB;
			loadCB.clear();
			for (int i = 0; i < (int) tmpcb.size(); i++)
				if (tmpcb[i].cb) tmpcb[i].cb(nullptr, &exc);
			if (sync) throw exc;
			return;
		}
		try {
			if (loaded) doUnload();
			doLoad();
		} catch (exception& ex) {
			deleteTmpfiles();
			auto tmpcb = loadCB;
			loadCB.clear();
			for (int i = 0; i < (int) tmpcb.size(); i++)
				if (tmpcb[i].cb) tmpcb[i].cb(nullptr, &ex);
			if (sync) throw;
			return;
		}
		deleteTmpfiles();
		auto tmpcb = loadCB;
		loadCB.clear();
		for (int i = 0; i < (int) tmpcb.size(); i++)
			if (tmpcb[i].cb) tmpcb[i].cb(this, nullptr);
	}
	void loadedPage::beginRead() {
		if (ms.bufferSize - ms.bufferPos < 4096) ms.flushBuffer(4096);
		compile_fd->read(ms.buffer + ms.bufferPos, ms.bufferSize - ms.bufferPos,
				CP::Callback(&loadedPage::readCB, this));
	}
	bool loadedPage::doCompile(Poll& p, string wd, const vector<string>& cxxopts) {
		string tmp;
		char sss[32];
		snprintf(sss, 32, "%i", rand());
		string sss_(sss);
		if (tmpDir.length() == 0) {
			txtPath = path + "." + sss_ + ".txt";
			dllPath = path + "." + sss_ + ".so";
			cPath = path + "." + sss_ + ".C";
		} else {
			string path1 = string(path.data(), path.length());
			string p = basename((char*) path1.c_str());
			txtPath = tmpDir + "/" + p + "." + sss_ + ".txt";
			dllPath = tmpDir + "/" + p + "." + sss_ + ".so";
			cPath = tmpDir + "/" + p + "." + sss_ + ".C";
		}

		//check if a precompiled page exists; if it exists and is newer than
		//the .cppsp, then simply hardlink to it
		timespec modif_txt, modif_so, modif_cppsp;

		string binPath = loadedPage_getBinPath(this);
		string txt1 = binPath + ".txt";
		string dll1 = binPath + ".so";
		struct stat st;
		{
			TO_C_STR(path.data(), path.length(), s1);
			if (stat(s1, &st) < 0) goto do_comp;
			modif_cppsp = st.st_mtim;
		}
		if (stat(txt1.c_str(), &st) < 0) goto do_comp;
		modif_txt = st.st_mtim;
		if (stat(dll1.c_str(), &st) < 0) goto do_comp;
		modif_so = st.st_mtim;
		if (tsCompare(modif_cppsp, modif_txt) <= 0 && tsCompare(modif_cppsp, modif_so) <= 0) {
			if (link(txt1.c_str(), txtPath.c_str()) < 0) goto do_comp;
			if (link(dll1.c_str(), dllPath.c_str()) < 0) goto do_comp;
			afterCompile(true, true);
			return true;
		}
		do_comp: deleteTmpfiles();
		CP::File* f;
		ms.clear();
		try {
			if (this->tmpDir.length() == 0) {
				f = (CP::File*) checkError(
						compilePage(wd, path, cPath, txtPath, dllPath, cxxopts, compilerPID, tmp));
			} else {
				auto opts = cxxopts;
				opts.push_back("-iquote");
				string path1(path.data(), path.length());
				opts.push_back(dirname((char*) path1.c_str()));
				f = (CP::File*) checkError(
						compilePage(wd, path, cPath, txtPath, dllPath, opts, compilerPID, tmp));
			}
		} catch (...) {
			deleteTmpfiles();
			throw;
		}
		tmp += "\n(in ";
		tmp += wd;
		tmp += ")";
		tmp += "\n";
		ms.write(tmp.data(), tmp.length());
		p.add(*f);
		compile_fd = f;
		f->release();
		beginRead();
		compiling = true;
		return false;
	}
	void loadedPage::doLoad() {
		ScopeLock sl(dlMutex);
		//printf("doLoad(\"%s\");\n",path.c_str());
		struct stat st;
		checkError(stat(txtPath.c_str(), &st), txtPath);
		stringTableLen = st.st_size;
		stringTableFD = checkError(open(txtPath.c_str(), O_RDONLY | O_CLOEXEC), txtPath);
		stringTable = (const uint8_t*) checkError(
				mmap(NULL, stringTableLen, PROT_READ, MAP_SHARED, stringTableFD, 0));
		dlHandle = dlopen(dllPath.c_str(), RTLD_LOCAL | RTLD_LAZY);
		if (dlHandle == NULL) throw runtime_error(dlerror());
		getObjectSize = (getObjectSize_t) checkDLError(dlsym(dlHandle, "getObjectSize"));
		createObject = (createObject_t) checkDLError(dlsym(dlHandle, "createObject"));
		createObject1 = (createObject1_t) checkDLError(dlsym(dlHandle, "createObject1"));
		initModule = (initModule_t) dlsym(dlHandle, "initModule");
		deinitModule = (deinitModule_t) dlsym(dlHandle, "deinitModule");
		getModuleInfo = (getModuleInfo_t) dlsym(dlHandle, "getModuleInfo");
		if (getModuleInfo != NULL) getModuleInfo(info);
		loaded = true;
		clock_gettime(CLOCK_REALTIME, &lastLoad);
		//printf("loaded: dlHandle=%p; createObject=%p\n",dlHandle,(void*)createObject);
	}
	void loadedPage::doUnload() {
		//printf("doUnload(\"%s\");\n",path.c_str());
		loaded = false;
		info.reset();
		if (stringTable != NULL) munmap((void*) stringTable, stringTableLen);
		if (stringTableFD != -1) close(stringTableFD);
		if (dlHandle != NULL) {
			deinitModule_t deinitModule = (deinitModule_t) dlsym(dlHandle, "deinitModule");
			if (deinitModule != NULL) {
				fprintf(stderr, "module %s unloaded\n", path.c_str());
			}
			ScopeLock sl(dlMutex);
			checkError(dlclose(dlHandle));
			//void* tmp=dlopen((path + ".so").c_str(), RTLD_LOCAL | RTLD_LAZY|RTLD_NOLOAD);
			//if(tmp!=NULL) throw runtime_error("unable to unload library");
		}
		dlHandle = NULL;
		stringTable = NULL;
		stringTableFD = -1;
		//printf("unloaded\n");
	}
	Page* loadedPage::doCreate(RGC::Allocator* a) {
		Page* tmp = createObject1(a);
		checkError(tmp);
		tmp->__stringTable = stringTable;
		tmp->filePath = {path.data(),(int)path.length()};
		return tmp;
	}
	loadedPage::loadedPage() :
			dlHandle(NULL), stringTable(NULL), stringTableFD(-1), moduleCount(0) {
		//printf("loadedPage()\n");
		compiling = false;
		doUnload();
	}
	loadedPage::~loadedPage() {
		//printf("~loadedPage()\n");
		doUnload();
	}
	//returns: 0: no-op; 2: should reload/recompile
	int loadedPage::shouldCompile() {
		struct stat st;
		{
			TO_C_STR(path.data(), path.length(), s1);
			checkError(stat(s1, &st), path);
			if (S_ISDIR(st.st_mode) || S_ISSOCK(st.st_mode)) pageErr_isDir();
		}
		if (!loaded) return 2;
		if (moduleCount > 0) return 0;
		timespec modif_cppsp = st.st_mtim;
		/*{
		 CONCAT_TO(path.data(), path.length(), s1, ".txt");
		 if (stat(s1, &st) < 0) {
		 if (errno == ENOENT) return 2;
		 else checkError(-1);
		 }
		 }
		 timespec modif_txt = st.st_mtim;
		 {
		 CONCAT_TO(path.data(), path.length(), s1, ".so");
		 if (stat(s1, &st) < 0) {
		 if (errno == ENOENT) return 2;
		 else checkError(-1);
		 }
		 }
		 timespec modif_so = st.st_mtim;*/
		int i = 0;
		if (tsCompare(lastLoad, modif_cppsp) < 0) i = 2;
		//if(tsCompare(lastLoad, modif_txt)< 0 || tsCompare(lastLoad, modif_so) <0) i=1;
		//if(tsCompare(modif_cppsp, modif_txt)> 0 || tsCompare(modif_cppsp, modif_so) >0) i=2;
		//printf("shouldCompile(\"%s\") = %i\n",path.c_str(),i);
		return i;
	}
	void staticPage::_loadFD() {
		fd = checkError(open(path.c_str(), O_RDONLY | O_CLOEXEC), path);
	}
	void staticPage::_loadMap() {
		data.d = (char*) checkError(mmap(NULL, data.len, PROT_READ, MAP_SHARED, fd, 0));
	}
	void staticPage::doLoad(bool keepFD, bool map) {
		struct stat st;
		checkError(stat(path.c_str(), &st), path);
		data.len = int32_t(fileLen = (int64_t) st.st_size);
		_loadFD();
		if (map) _loadMap();
		if (!keepFD) {
			close(fd);
			fd = -1;
		}
		loaded = true;
		clock_gettime(CLOCK_REALTIME, &lastLoad);
	}
	void staticPage::doUnload() {
		loaded = false;
		if (data.d != NULL) munmap((void*) data.d, data.len);
		data = nullptr;
		if (fd >= 0) close(fd);
		fd = -1;
	}
	bool staticPage::shouldReload() {
		struct stat st;
		{
			TO_C_STR(path.data(), path.length(), s1);
			checkError(stat(s1, &st), path);
			if (S_ISDIR(st.st_mode) || S_ISSOCK(st.st_mode))
				throw ParserException("requested path is a directory or socket");
		}
		if (!loaded) return true;
		timespec modif_cppsp = st.st_mtim;
		return (tsCompare(lastLoad, modif_cppsp) < 0);
	}
	staticPage::staticPage() :
			fd(-1) {
		loaded = false;
	}
	staticPage::~staticPage() {
		doUnload();
	}
	static inline void precheckPage(String path) {
		struct stat st;
		TO_C_STR(path.data(), path.length(), s1);
		checkError(stat(s1, &st), path);
		if (S_ISDIR(st.st_mode) || S_ISSOCK(st.st_mode)) pageErr_isDir();
	}
	cppspManager::cppspManager() :
			threadID(0), debug(false) {
	}
	staticPage* cppspManager::loadStaticPage(String path, bool fd, bool map) {
		staticPage* lp1;
		auto it = staticCache.find(path);
		if (it == staticCache.end()) {
			precheckPage(path);
			lp1 = new staticPage();
			lp1->path = path.toSTDString();
			int i = path.lastIndexOf('.');
			if (i >= 0) {
				string ext = path.subString(i + 1).toSTDString();
				auto it = mimeTypes.find(ext);
				if (it != mimeTypes.end()) {
					lp1->mime = (*it).second;
				}
			}
			staticCache.insert( { lp1->path, lp1 });
		} else lp1 = (*it).second;
		staticPage& lp(*lp1);
		if (likely(lp.loaded & !shouldCheck(lp))) {
			return &lp;
		}
		if (lp.shouldReload()) {
			lp.doUnload();
		}
		if (!lp.loaded) lp.doLoad(fd, map);
		if (fd && lp.fd < 0)
			lp._loadFD();
		else if (map && lp.data.d == nullptr) lp._loadMap();
		return &lp;
	}
	bool cppspManager::shouldCheck(loadedPage& p) {
		timespec tmp1 = curTime;
		tmp1.tv_sec -= 2;
		if (tsCompare(p.lastCheck, tmp1) < 0) {
			p.lastCheck = curTime;
			return true;
		} else return false;
	}
	bool cppspManager::shouldCheck(staticPage& p) {
		timespec tmp1 = curTime;
		tmp1.tv_sec -= 2;
		if (tsCompare(p.lastCheck, tmp1) < 0) {
			p.lastCheck = curTime;
			return true;
		} else return false;
	}

	AsyncValue<loadedPage*> cppspManager::loadPage(Poll& p, String wd, String path) {
		loadedPage* lp1;
		auto it = cache.find(path);
		if (unlikely(int(it == cache.end()))) {
			precheckPage(path);
			lp1 = new loadedPage();
			lp1->tmpDir = this->tmpDir;
			lp1->path = path.toSTDString();
			lp1->manager = this;
			cache.insert( { lp1->path, lp1 });
		} else lp1 = (*it).second;
		loadedPage& lp(*lp1);
		int c = 0;
		if (unlikely(lp1->compiling)) {
			lp.loadCB.push_back( { nullptr });
			return Future<loadedPage*>(&lp.loadCB[lp.loadCB.size() - 1].cb);
		}
		if (likely(lp1->loaded & !shouldCheck(*lp1))) {
			xxx: return lp1;
		}
		c = lp.shouldCompile();
		if (likely(lp1->loaded && c==0)) goto xxx;
		if (c >= 2) {
			try {
				if (lp.doCompile(p, wd.toSTDString(), cxxopts)) return lp1;
			} catch (exception& ex) {
				throw;
			}
			if (lp.compiling) {
				lp.loadCB.push_back( { nullptr });
				return Future<loadedPage*>(&lp.loadCB[lp.loadCB.size() - 1].cb);
			}
			return &lp;
		}
		if (c >= 1) {
			lp.doUnload();
		}
		lp.doLoad();
		goto xxx;
	}
	bool cppspManager::cleanCache(int minAge) {
		timespec tmp1 = curTime;
		tmp1.tv_sec -= minAge;
		int del = 0;
		bool ret = false;
		{
			auto it = cache.begin();
			while (it != cache.end()) {
				if ((*it).second->refCount <= 1 && tsCompare((*it).second->lastCheck, tmp1) <= 0) {
					delete (*it).second;
					auto tmp = it;
					it++;
					cache.erase(tmp);
					del++;
					continue;
				} else if((*it).second->moduleCount == 0) ret = true;
				it++;
			}
		}
		{
			auto it = staticCache.begin();
			while (it != staticCache.end()) {
				if ((*it).second->refCount <= 1 && tsCompare((*it).second->lastCheck, tmp1) <= 0) {
					delete (*it).second;
					auto tmp = it;
					it++;
					staticCache.erase(tmp);
					del++;
					continue;
				} else ret = true;
				it++;
			}
		}
		if (del > 0) printf("%i file cache entries purged\n", del);
		return ret;
	}
	void cppspManager::loadMimeDB(CP::StreamReader& in) {
		string s;
		while (true) {
			s = in.readLine();
			if (s.length() <= 0) {
				if (in.eof)
					break;
				else continue;
			}
			String tmp = s;
			int i = tmp.indexOf(':');
			if (i < 0) continue;
			String ext = tmp.subString(i + 1);
			if (ext.len < 3) continue;
			if (!(ext[0] == '*' && ext[1] == '.')) continue;
			ext.clip(2);
			mimeTypes.insert( { ext.toSTDString(), tmp.subString(0, i).toSTDString() });
		}
	}
	void handleError(exception* ex, cppsp::Response& resp, String path) {
		HTTPException* ex1 = dynamic_cast<HTTPException*>(ex);

		resp.clear();
		if (ex1 != nullptr) {
			resp.statusCode = ex1->code;
			resp.statusName = ex1->what();
		} else {
			FileNotFoundException* ex2 = dynamic_cast<FileNotFoundException*>(ex);
			if (ex2 != nullptr) {
				resp.statusCode = 404;
				resp.statusName = ex2->what();
			} else {
				resp.statusCode = 500;
				resp.statusName = "Internal server error";
			}
		}
		resp.headers["Content-Type"] = "text/html; charset=UTF-8";
		resp.output.write("<html><head><title>Server error</title></head>\n");
		resp.output.write("<body><h1 style=\"color: #aa1111\">Server error in ");
		htmlEscape(path, resp.output);
		resp.output.write("</h1><hr />"
				"<h2 style=\"color: #444\">");
		htmlEscape(ex->what(), resp.output);
		resp.output.write("</h2>");
		cppsp::CompileException* ce = dynamic_cast<cppsp::CompileException*>(ex);
		if (ce != NULL) {
			resp.output.write("<pre style=\"color: #000; background: #ffc; padding: 8px;\">");
			htmlEscape(String(ce->compilerOutput), resp.output);
			resp.output.write("</pre>");
		}
		resp.output.writeF("</body></html>");
	}
}

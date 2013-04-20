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

using namespace CP;
using namespace std;
namespace cppsp
{
	ParserException::ParserException() :
			message(strerror(errno)), number(errno) {
	}
	ParserException::ParserException(int32_t number) :
			message(strerror(number)), number(number) {
	}
	ParserException::ParserException(string message, int32_t number) :
			message(message), number(number) {
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

	static void split(const char* s, int len, char delim,
			const function<void(const char*, int)>& cb) {
		if (len == -1) len = strlen(s);
		const char* s1 = s;
		const char* end = s + len;
		while (true) {
			s = (const char*) memchr(s, delim, end - s);
			if (s == NULL) break;
			cb(s1, s - s1);
			s1 = ++s;
		}
		cb(s1, end - s1);
	}

	// "/"		+ "aaaaa"	=> "/aaaaa"
	// "/asdf/"	+ "zzz"		=> "/asdf/zzz"
	// "/asdf/"	+ "zzz/"		=> "/asdf/zzz/"
	// "/asdf"	+ "zzz"		=> "/zzz"
	// "/asdf/"	+ "../zzz"	=> "/zzz"
	// "/asdf/"	+ "a/../x"	=> "/asdf/x"
	// "/asdf/"	+ "/zzz"		=> "/zzz"
	//the size of buf should be at least strlen(p1)+strlen(p2)
	//returns the length of the string written to buf; does NOT write null byte
	int combinePath(const char* p1, int l1, const char* p2, int l2, char* buf) {
		if (l2 > 0 && p2[0] == '/') {
			memcpy(buf, p2, l2);
			return l2;
		}
		int i = l1;
		memcpy(buf, p1, i);
		if (l2 > 0) {
			i--;
			while (i >= 0 && buf[i] != '/')
				i--;
			if (i < 0) i = 0;
			split(p2, l2, '/', [&](const char* s, int l) {
				if(l==2 && memcmp(s,"..",2)==0) {
					i--;
					while(i>=0 && buf[i]!='/')i--;
					if(i<0) i=0;
				} else if(l==1 && *s=='.') {
					buf[i]='/';
					i++;
				}
				else {
					//while(i>=0 && buf[i]!='/')i--;
					buf[i]='/';
					i++;
					memcpy(buf+i,s,l);
					i+=l;
				}
			});
			//if (l2 > 0 && i > 0 && p2[l2 - 1] != '/' && buf[i - 1] == '/')
		}
		if (i < 0) i = 0;
		return i;
	}
	int combinePath(const char* p1, const char* p2, char* buf) {
		return combinePath(p1, strlen(p1), p2, strlen(p2), buf);
	}
	//p1 is the "root" directory
	//guarantees that the resulting path won't be outside of p1
	int combinePathChroot(const char* p1, int l1, const char* p2, int l2, char* buf) {
		int i = l1;
		memcpy(buf, p1, i);
		if (l2 > 0) {
			bool first(true);
			split(p2, l2, '/', [&](const char* s, int l) {
				if(first) {
					first=false;
					if(l==0) return;
				}
				if(l==2 && memcmp(s,"..",2)==0) {
					i--;
					while(i>=0 && buf[i]!='/')i--;
					if(i<l1) i=l1;
				} else if(l==1 && *s=='.') {
					if(!(i>0 && buf[i-1]=='/')) {
						buf[i]='/';
						i++;
					}
				}
				else {
					//while(i>=0 && buf[i]!='/')i--;
					if(!(i>0 && buf[i-1]=='/')) {
						buf[i]='/';
						i++;
					}
					memcpy(buf+i,s,l);
					i+=l;
				}
			});
			//if (l2 > 0 && i > 0 && p2[l2 - 1] != '/' && buf[i - 1] == '/')
		}
		if (i < l1) i = l1;
		return i;
	}
	int combinePathChroot(const char* p1, const char* p2, char* buf) {
		return combinePathChroot(p1, strlen(p1), p2, strlen(p2), buf);
	}

	string combinePath(const char* p1, const char* p2) {
		char tmp[strlen(p1) + strlen(p2)];
		int l = combinePath(p1, p2, tmp);
		return string(tmp, l);
	}
	string combinePathChroot(const char* p1, const char* p2) {
		char tmp[strlen(p1) + strlen(p2)];
		int l = combinePathChroot(p1, p2, tmp);
		return string(tmp, l);
	}
	//parses a cppsp page and generates code (to out) and string table (to st_out)
	void doParse(const char* name, const char* in, int inLen, Stream& out, Stream& st_out,
			vector<string>& c_opts) {
		const char* s = in;
		const char* end = s + inLen;
		string inherits = "Page";
		string classname = (name == NULL ? "__cppsp_unnamed_page" : name);
		int st_pos = 0;
		//int out_initlen=out.length();
		Stream& ms1(out); //declarations outside of the class
		MemoryStream ms2; //declarations outside of the render() function
		MemoryStream ms3; //code inside render()
		StreamWriter sw1(ms1);
		StreamWriter sw2(ms2);
		StreamWriter sw3(ms3);
		sw1.write("#include <cppsp/page.H>\n#include <cpoll/cpoll.H>\n");
		sw1.write("#include <cppsp/common.H>\n#include <cppsp/stringutils.H>\n");
		sw1.write("using namespace cppsp;\nusing namespace CP;\n");

		while (true) {
			if (s >= end) break;
			const char* old_s = s;
			s = (const char*) memmem(s, end - s, "<%", 2);
			int st_pos0 = st_pos;
			if (s > old_s) {
				st_out.write(old_s, s - old_s);
				st_pos += (s - old_s);
				sw3.writeF("__writeStringTable(%i,%i);\n", st_pos0, st_pos - st_pos0);
			} else if (s == NULL) {
				st_out.write(old_s, end - old_s);
				st_pos += (end - old_s);
				sw3.writeF("__writeStringTable(%i,%i);\n", st_pos0, st_pos - st_pos0);
				break;
			}
			s += 2;
			if (s >= end) throw ParserException("reached EOF when looking past \"<%\"");
			const char* s1 = (const char*) memmem(s, end - s, "%>", 2);
			if (s1 == NULL) throw ParserException("reached EOF when looking for matching \"%>\"");
			switch (*s) {
				case '!':
				{ //compiler option
					c_opts.push_back(string(s + 1, s1 - s - 1));
					break;
				}
				case '@':
				{ //cppsp options
					int nextopt = 0;
					split(s + 1, s1 - s - 1, ' ', [&](const char* s1, int l1) {
						switch(nextopt) {
							case 0:
							{
								if(l1==8 && memcmp(s1,"inherits",8)==0) nextopt=1;
								else if(l1==5 && memcmp(s1,"class",5)==0) nextopt=2;
								return;
							}
							case 1:
							{
								inherits=string(s1,l1);
								break;
							}
							case 2:
							{
								if(name==NULL) classname=string(s1,l1);
								break;
							}
						}
						nextopt=0;
					});
					break;
				}
				case '#':
				{ //declarations outside of the class
					sw1.write(s + 1, s1 - s - 1);
					break;
				}
				case '$':
				{ //declarations outside of the render() function
					sw2.write(s + 1, s1 - s - 1);
					break;
				}
				case '=':
				{
					sw3.write("output.write(");
					sw3.write(s + 1, s1 - s - 1);
					sw3.write(");\n");
					break;
				}
				default:
				{
					sw3.write(s, s1 - s);
				}
			}
			s = s1 + 2;
		}
		sw2.flush();
		sw3.flush();
		sw1.writeF("class %s: public %s {\npublic:\n", classname.c_str(), inherits.c_str());
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
		sw1.flush();
		//out.write(ms1.data(), ms1.length());
	}
	static inline int checkError(int i) {
		if (i < 0) throw runtime_error(strerror(errno));
		return i;
	}
	static inline void* checkError(void* p) {
		if (p == NULL) throw runtime_error(strerror(errno));
		return p;
	}
	CP::File* compilePage(string wd, string path, const vector<string>& cxxopts, pid_t& pid) {
		vector<string> c_opts { "g++", "g++", "--std=c++0x", "--shared", "-o", path + ".so", path
				+ ".C" };
		c_opts.insert(c_opts.end(), cxxopts.begin(), cxxopts.end());
		{
			File inp(open(path.c_str(), O_RDONLY));
			unlink((path + ".C").c_str());
			File out_c(open((path + ".C").c_str(), O_RDWR | O_CREAT | O_TRUNC, 0666));
			File out_s(open((path + ".txt").c_str(), O_RDWR | O_CREAT | O_TRUNC, 0666));
			MemoryStream ms;
			inp.readToEnd(ms);
			ms.flush();

			cppsp::doParse(NULL, (const char*) ms.data(), ms.length(), out_c, out_s, c_opts);
		}

		const char* cmds[c_opts.size() + 1];
		for (int i = 0; i < (int) c_opts.size(); i++) {
			cmds[i] = c_opts[i].c_str();
		}
		cmds[c_opts.size()] = NULL;

		int p[2];
		checkError(pipe(p));
		unlink((path + ".so").c_str());
		pid_t tmp = fork();
		if (tmp > 0) {
			close(p[1]);
			pid = tmp;
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
		if (time1.tv_sec < time2.tv_sec) return (-1); /* Less than. */
		else if (time1.tv_sec > time2.tv_sec) return (1); /* Greater than. */
		else if (time1.tv_nsec < time2.tv_nsec) return (-1); /* Less than. */
		else if (time1.tv_nsec > time2.tv_nsec) return (1); /* Greater than. */
		else return (0); /* Equal. */
	}
	bool shouldCompile(string page) {
		struct stat st;
		checkError(stat(page.c_str(), &st));
		timespec modif_cppsp = st.st_mtim;
		if (stat((page + ".txt").c_str(), &st) < 0) {
			if (errno == ENOENT) return true;
			else checkError(-1);
		}
		timespec modif_txt = st.st_mtim;
		if (stat((page + ".so").c_str(), &st) < 0) {
			if (errno == ENOENT) return true;
			else checkError(-1);
		}
		timespec modif_so = st.st_mtim;
		return (tsCompare(modif_cppsp, modif_txt) == 1 || tsCompare(modif_cppsp, modif_so) == 1);
	}
	class loadedPage
	{
	public:
		loadedPage& operator=(const loadedPage& other)=delete;
		void* dlHandle;
		const uint8_t* stringTable;
		int stringTableLen;
		typedef int (*getObjectSize_t)();
		typedef Page* (*createObject_t)(void* mem);
		int (*getObjectSize)();
		Page* (*createObject)(void* mem);
		RGC::Ref<CP::File> compile_fd;
		MemoryStream ms;
		//Delegate<void(loadedPage&)> compileCB;
		vector<Delegate<void(Page*, exception* ex)> > loadCB;
		string path;
		int stringTableFD;
		pid_t compilerPID;
		bool loaded;
		bool compiling;
		void readCB(int r) {
			if (r <= 0) {
				compiling = false;
				int status=-1;
				waitpid(compilerPID,&status,WNOHANG);
				if(status!=0) {
					CompileException exc;
					exc.compilerOutput=string((const char*)ms.data(),ms.length());
					for (int i = 0; i < (int) loadCB.size(); i++)
					loadCB[i](nullptr, &exc);
					goto aaa;
				}
				try {
					doUnload();
					doLoad();
					for (int i = 0; i < (int) loadCB.size(); i++)
					loadCB[i](doCreate(), nullptr);
				} catch (exception& ex) {
					/*string msg(ex.what());
					 msg.append("\n\n\ncompiler output:\n");
					 msg.append((const char*)ms.data(),ms.length());
					 logic_error le(msg);*/
					for (int i = 0; i < (int) loadCB.size(); i++)
					loadCB[i](nullptr, &ex);
				}
				aaa:
				loadCB.clear();
				compile_fd=nullptr;
				//if (compileCB != nullptr) compileCB(*this);
				return;
			}
			ms.bufferPos += r;
			ms.flush();
			beginRead();
		}
		void beginRead() {
			if (ms.bufferSize - ms.bufferPos < 4096) ms.flushBuffer(4096);
			compile_fd->read(ms.buffer + ms.bufferPos, ms.bufferSize - ms.bufferPos,
					CP::Callback(&loadedPage::readCB, this));
		}
		void doCompile(Poll& p, string wd, const vector<string>& cxxopts) {
			compiling = true;
			ms.clear();
			CP::File* f = (CP::File*) checkError(compilePage(wd,path,cxxopts,compilerPID));
			p.add(*f);
			compile_fd = f;
			f->release();
			beginRead();
		}
		void doLoad() {
			struct stat st;
			checkError(stat(path.c_str(), &st));
			stringTableLen = st.st_size;
			stringTableFD = checkError(open((path + ".txt").c_str(), O_RDONLY));
			stringTable = (const uint8_t*) checkError(
					mmap(NULL, stringTableLen, PROT_READ, MAP_SHARED, stringTableFD, 0));
			dlHandle = dlopen((path + ".so").c_str(), RTLD_LOCAL | RTLD_LAZY);
			if(dlHandle==NULL) throw runtime_error(dlerror());
			getObjectSize = (getObjectSize_t) checkError(dlsym(dlHandle, "getObjectSize"));
			createObject = (createObject_t) checkError(dlsym(dlHandle, "createObject"));
			loaded = true;
			//printf("loaded: dlHandle=%p; createObject=%p\n",dlHandle,(void*)createObject);
		}
		void doUnload() {
			loaded = false;
			if (stringTable != NULL) munmap((void*) stringTable, stringTableLen);
			if (stringTableFD != -1) close(stringTableFD);
			if (dlHandle != NULL) {
				checkError(dlclose(dlHandle));
				void* tmp=dlopen((path + ".so").c_str(), RTLD_LOCAL | RTLD_LAZY|RTLD_NOLOAD);
				if(tmp!=NULL) throw runtime_error("unable to unload library");
			}
			dlHandle = NULL;
			stringTable = NULL;
			stringTableFD = -1;
			//printf("unloaded\n");
		}
		Page* doCreate() {
			Page* tmp = createObject(NULL);
			checkError(tmp);
			tmp->__stringTable = stringTable;
			tmp->filePath = path;
			return tmp;
		}
		loadedPage():dlHandle(NULL),stringTable(NULL),stringTableFD(-1) {
			//printf("loadedPage()\n");
			compiling = false;
			doUnload();
		}
		~loadedPage() {
			//printf("~loadedPage()\n");
			doUnload();
		}
	};
	class cppspManager
	{
	public:
		map<string, loadedPage*> cache;
		vector<string> cxxopts;
		/*void compileCB(loadedPage& lp) {

		 }*/
		void loadPage(Poll& p, string wd, string path, Delegate<void(Page*, exception* ex)> cb) {
			loadedPage* lp1;
			auto it = cache.find(path);
			if (it == cache.end()) {
				lp1 = new loadedPage();
				lp1->path = path;
				cache.insert( { path, lp1 });
			} else lp1 = (*it).second;
			loadedPage& lp(*lp1);

			bool c = false;
			if (lp.compiling) goto asdf;
			try {
				c = shouldCompile(path);
			} catch (exception& ex) {
				cb(nullptr, &ex);
				return;
			}
			if (c) {
				lp.loadCB.push_back(cb);
				try {
					lp.doCompile(p, wd, cxxopts);
				} catch (exception& ex) {
					lp.loadCB.resize(lp.loadCB.size() - 1);
					cb(nullptr, &ex);
				}
				return;
			}
			if (lp.loaded) {
				try {
					cb(lp.doCreate(), nullptr);
				} catch (exception& ex) {
					cb(nullptr, &ex);
				}
				return;
			} else if (lp.compiling) {
				asdf: lp.loadCB.push_back(cb);
			} else {
				try {
					lp.doLoad();
					cb(lp.doCreate(), nullptr);
				} catch (exception& ex) {
					cb(nullptr, &ex);
				}
			}
		}
	};
	cppspManager mgr;
	void loadPage(Poll& p, string wd, string path, Delegate<void(Page*, exception* ex)> cb) {
		mgr.loadPage(p, wd, path, cb);
	}
	vector<string>& CXXOpts() {
		return mgr.cxxopts;
	}

	void handleError(exception* ex, cppsp::Response& resp, string path) {
		resp.clear();
		resp.headers["Content-Type"] = "text/html; charset=UTF-8";
		//resp.writeHeaders();
		string title = "Server error in " + path;
		resp.output.writeF("<html><head><title>%s</title>"
				"<style></style></head>", title.c_str());
		resp.output.writeF("<body><h1 style=\"color: #aa1111\">%s</h1><hr />"
				"<h2 style=\"color: #444\">%s</h2>", title.c_str(), ex->what());
		cppsp::CompileException* ce = dynamic_cast<cppsp::CompileException*>(ex);
		if (ce != NULL) {
			resp.output.write("<pre style=\"color: #000; background: #ffc; padding: 8px;\">");
			resp.output.write(ce->compilerOutput);
			resp.output.write("</pre>");
		}
		resp.output.writeF("</body></html>");
	}
}

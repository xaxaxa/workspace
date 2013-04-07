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
#include "include/page.H"

using namespace CP;
using namespace std;
namespace cppsp
{
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

	static void split(const char* s, int len, char delim,
			const function<void(const char*, int)>& cb) {
		if (len == -1) len = strlen(s);
		const char* s1 = s;
		const char* end = s + len;
		while (true) {
			s = strchr(s, delim);
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
	int combinePath(const char* p1, const char* p2, char* buf) {
		int l2 = strlen(p2);
		if (l2 > 0 && p2[0] == '/') {
			memcpy(buf, p2, l2);
			return l2;
		}
		int i = strlen(p1);
		memcpy(buf, p1, i);
		if (l2 > 0) {
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
	//parses a cppsp page and generates code (to out) and string table (to st_out)
	void doParse(const char* name, const char* in, int inLen, Stream& out, Stream& st_out,
			vector<string>& c_opts) {
		const char* s = in;
		const char* end = s + inLen;
		int st_pos = 0;
		//int out_initlen=out.length();
		Stream& ms1(out); //declarations outside of the class
		MemoryStream ms2; //declarations outside of the render() function
		MemoryStream ms3; //code inside render()
		StreamWriter sw1(ms1);
		StreamWriter sw2(ms2);
		StreamWriter sw3(ms3);
		sw1.write("#include <cppsp/page.H>\n#include <cpoll/cpoll.H>\n");
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
				{ //declarations outside of the class
					sw1.write(s + 1, s1 - s - 1);
					break;
				}
				case '#':
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
		sw1.writeF("class %s: public Page {\n", name);
		sw1.write(ms2.data(), ms2.length());
		sw1.write("virtual void render(StreamWriter& output) override {\n");
		sw1.write(ms3.data(), ms3.length());
		sw1.write("}\n};\n");
		sw1.writeF("extern \"C\" int getObjectSize() {return sizeof(%s);}\n",name);
		sw1.writeF("extern \"C\" Page* createObject(void* mem) {return new (mem) %s();}\n",name);
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
	CP::File* compilePage(string path) {
		vector<string> c_opts { "g++", "g++", "--std=c++0x", "--shared", "-o", basename(
				(char*) (path + ".so").c_str()), basename((char*) (path + ".C").c_str()) };
		{
			File inp(open(path.c_str(), O_RDONLY));
			File out_c(open((path + ".C").c_str(), O_RDWR | O_CREAT | O_TRUNC, 0666));
			File out_s(open((path + ".txt").c_str(), O_RDWR | O_CREAT | O_TRUNC, 0666));
			MemoryStream ms;
			inp.readToEnd(ms);
			ms.flush();

			cppsp::doParse("__cppsp_unnamed_page", (const char*) ms.data(), ms.length(), out_c, out_s,
					c_opts);
		}

		const char* cmds[c_opts.size() + 1];
		for (int i = 0; i < (int) c_opts.size(); i++) {
			cmds[i] = c_opts[i].c_str();
		}
		cmds[c_opts.size()] = NULL;

		int p[2];
		checkError(pipe(p));
		pid_t tmp = fork();
		if (tmp > 0) {
			close(p[1]);
			return new CP::File(p[0]);
		} else if (tmp == 0) {
			close(p[0]);
			dup2(p[1], 1);
			dup2(p[1], 2);
			close(p[1]);
			chdir(dirname((char*) path.c_str()));
			execvp(cmds[0], (char**) (cmds + 1));
			_exit(1);
		} else {
			checkError(tmp);
		}
		return NULL;
	}
	class loadedPage
	{
		void* dlHandle;
		const uint8_t* stringTable;
		int stringTableLen;
		typedef int (*getObjectSize_t)();
		typedef Page* (*createObject_t)(void* mem);
		int (*getObjectSize)();
		Page* (*createObject)(void* mem);

		int stringTableFD;
		void doLoad(string page) {
			struct stat st;
			checkError(stat(page.c_str(), &st) < 0);
			stringTableLen = st.st_size;
			stringTableFD = checkError(open((page + ".txt").c_str(), O_RDONLY));
			stringTable = (const uint8_t*) checkError(
					mmap(NULL, stringTableLen, PROT_READ, MAP_SHARED, stringTableFD, 0));
			dlHandle = checkError(dlopen((page + ".so").c_str(), RTLD_LOCAL | RTLD_LAZY));
			getObjectSize = (getObjectSize_t) checkError(dlsym(dlHandle, "getObjectSize"));
			createObject = (createObject_t) checkError(dlsym(dlHandle, "createObject"));
		}
		void doUnload() {
			if (stringTable != NULL) munmap((void*) stringTable, stringTableLen);
			if (stringTableFD != -1) close(stringTableFD);
			if (dlHandle != NULL) dlclose(dlHandle);
			dlHandle = NULL;
			stringTable = NULL;
			stringTableFD = -1;
		}
		loadedPage() {
			doUnload();
		}
		~loadedPage() {
			doUnload();
		}
	};
	class cppspManager
	{

	};
}

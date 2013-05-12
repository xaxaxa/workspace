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
#include "page.H"
using namespace std;
typedef CP::String String;
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

	void doParse(const char* name, const char* in, int inLen, CP::Stream& out, CP::Stream& st_out,
			vector<string>& c_opts);
	CP::File* compilePage(string wd, string path, const vector<string>& cxxopts, pid_t& pid);
	int combinePath(const char* p1, int l1, const char* p2, int l2, char* buf);
	int combinePath(const char* p1, const char* p2, char* buf);
	int combinePathChroot(const char* p1, int l1, const char* p2, int l2, char* buf);
	int combinePathChroot(const char* p1, const char* p2, char* buf);
	String combinePath(String p1, String p2, CP::StringPool& sp);
	String combinePathChroot(String p1, String p2, CP::StringPool& sp);
	cppspManager* cppspManager_new();
	void cppspManager_delete(cppspManager* mgr);
	void loadPage(cppspManager* mgr, CP::Poll& p, String wd, String path, RGC::Allocator* a,
			Delegate<void(Page*, exception* ex)> cb);
	void loadModule(cppspManager* mgr, CP::Poll& p, Server* srv, String wd, String path,
			Delegate<void(void*, exception* ex)> cb);
	String loadStaticPage(cppspManager* mgr, String path);
	vector<string>& CXXOpts(cppspManager* mgr);
	void updateTime(cppspManager* mgr);
	void handleError(exception* ex, cppsp::Response& resp, String path);
}
#endif /* COMMON_H_ */

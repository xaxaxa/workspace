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
	String combinePath(const char* p1, const char* p2, CP::StringPool& sp);
	String combinePathChroot(const char* p1, const char* p2, CP::StringPool& sp);
	void loadPage(CP::Poll& p, String wd, String path, Delegate<void(Page*, exception* ex)> cb);
	cppspManager* cppspManager_new();
	void cppspManager_delete(cppspManager* mgr);
	void loadPage(cppspManager* mgr, CP::Poll& p, String wd, String path,
			Delegate<void(Page*, exception* ex)> cb);
	vector<string>& CXXOpts();
	vector<string>& CXXOpts(cppspManager* mgr);
	void updateTime();
	void updateTime(cppspManager* mgr);
	void handleError(exception* ex, cppsp::Response& resp, String path);
}
#endif /* COMMON_H_ */

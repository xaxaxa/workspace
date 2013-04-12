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
		CompileException(int32_t number);
		CompileException(string message);
		~CompileException() throw ();
		const char* what() const throw ();
	};

	void doParse(const char* name, const char* in, int inLen, CP::Stream& out, CP::Stream& st_out,
			vector<string>& c_opts);
	CP::File* compilePage(string path);
	int combinePath(const char* p1, const char* p2, char* buf);
	int combinePathChroot(const char* p1, const char* p2, char* buf);
	string combinePath(const char* p1, const char* p2);
	string combinePathChroot(const char* p1, const char* p2);
	void loadPage(CP::Poll& p, string wd, string path, Delegate<void(Page*, exception* ex)> cb);
	vector<string>& CXXOpts();
	void handleError(exception* ex, cppsp::Response& resp, string path);
}
#endif /* COMMON_H_ */

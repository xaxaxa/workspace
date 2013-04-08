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
	void doParse(const char* name, const char* in, int inLen, CP::Stream& out, CP::Stream& st_out,
			vector<string>& c_opts);
	CP::File* compilePage(string path);
	int combinePath(const char* p1, const char* p2, char* buf);
	int combinePathChroot(const char* p1, const char* p2, char* buf);
	void loadPage(CP::Poll& p, string wd, string path, Delegate<void(Page*, exception* ex)> cb);
	vector<string>& CXXOpts();
}
#endif /* COMMON_H_ */

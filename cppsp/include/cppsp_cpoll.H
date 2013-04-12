/*
 * cppsp_socketd.H
 *
 *  Created on: Mar 8, 2013
 *      Author: xaxaxa
 */

#ifndef CPPSP_SOCKETD_H_
#define CPPSP_SOCKETD_H_
#include "page.H"
#include <cpoll/cpoll.H>
#include <string>
using namespace std;
namespace cppsp
{

	class CPollRequest: public Request
	{
	public:
		RGC::Ref<CP::Socket> s;
		CP::MemoryStream ms;
		Delegate<void()> tmp_cb;
		bool firstLine;

		CPollRequest(CP::Socket& s);
		void readHeaders(const Delegate<void()>& cb);
		void _beginRead();
		void _readCB(int i);
		void _endRead();
		virtual ~CPollRequest();
	};
} /* namespace cppsp */

#endif /* CPPSP_SOCKETD_H_ */

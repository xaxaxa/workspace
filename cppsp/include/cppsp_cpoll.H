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
		CP::MemoryStream _lineBuffer;
		RGC::Ref<CP::Socket> s;
		Delegate<void(bool)> tmp_cb;
		int _headers_begin;
		bool firstLine;

		CPollRequest(CP::Socket& s);
		void readHeaders(const Delegate<void(bool)>& cb);
		void _beginRead();
		void _readCB(int i);
		void _endRead(bool success);
		virtual ~CPollRequest();
	};
} /* namespace cppsp */

#endif /* CPPSP_SOCKETD_H_ */

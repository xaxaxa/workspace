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
#include "httpparser.H"
using namespace std;
typedef CP::String String;
namespace cppsp
{
	class CPollRequest: public Request
	{
	public:
		HTTPParser _parser;
		RGC::Ref<CP::Socket> s;
		Delegate<void(bool)> tmp_cb;
		int _headers_begin;
		bool firstLine;

		CPollRequest(CP::Socket& s, CP::StringPool* sp);
		//returns: true: request already in buffer; false: read is in progress
		bool readRequest(const Delegate<void(bool success)>& cb);
		void readPost(Delegate<void(Request&)> cb) override {
			parsePost(_parser.content);
			cb(*this);
		}
		virtual String getBufferedData() override {
			return _parser.getBufferData();
		}
		void _beginRead();
		void _readCB(int i);
		virtual ~CPollRequest();
	};
} /* namespace cppsp */

#endif /* CPPSP_SOCKETD_H_ */

/*
 * cppsp_socketd.C
 *
 *  Created on: Mar 8, 2013
 *      Author: xaxaxa
 */
#include "include/cppsp_cpoll.H"
namespace cppsp
{
	//static int CPollRequest::bufSize=4096;
	CPollRequest::CPollRequest(CP::Socket& s) :
			s(s), sr(s) {

	}
	
	void CPollRequest::readHeaders(const Delegate<void()>& cb) {
		this->tmp_cb = cb;
		_beginRead();
	}
	
	void CPollRequest::_beginRead() {
		sr.readTo("\r\n", 2, CP::StreamReader::Callback(&CPollRequest::_readCB, this));
	}
	
	void CPollRequest::_readCB(const string& line) {
		if(line.length()==0) {
			_endRead();
			return;
		}


		if(sr.eof)
			_endRead();
	}
	
	void CPollRequest::_endRead() {
		tmp_cb();
	}

	CPollRequest::~CPollRequest() {
	}
}

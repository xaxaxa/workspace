/*
 * page.C
 *
 *  Created on: Jan 26, 2013
 *      Author: xaxaxa
 */

#include "include/page.H"

namespace cppsp
{
	
	Page::Page() {
		// TODO Auto-generated constructor stub
		
	}
	
	void Page::handleRequest(Request& req, Response& resp) {
		request = &req;
		response = &resp;
		req.read();
	}
	
	void Page::render(CP::Stream& s) {
	}

	Page::~Page() {

	}
	Request::~Request() {
	}
} /* namespace cppsp */


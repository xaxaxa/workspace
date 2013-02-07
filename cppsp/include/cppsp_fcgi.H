/*
 * fcgi.H
 *
 *  Created on: Jan 29, 2013
 *      Author: xaxaxa
 */

#ifndef CPPSP_FCGI_H_
#define CPPSP_FCGI_H_
#include <fcgiapp.h>
#include "page.H"

namespace cppsp
{
	
	class FCGIRequest:public Request
	{
	public:
		virtual void read();
		FCGIRequest(FCGX_Stream *in, FCGX_ParamArray params);
		virtual ~FCGIRequest();
	};

} /* namespace cppsp */
#endif /* CPPSP_FCGI_H_ */

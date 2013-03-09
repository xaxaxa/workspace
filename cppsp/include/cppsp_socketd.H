/*
 * cppsp_socketd.H
 *
 *  Created on: Mar 8, 2013
 *      Author: xaxaxa
 */

#ifndef CPPSP_SOCKETD_H_
#define CPPSP_SOCKETD_H_
#include "page.H"

namespace cppsp
{

	class SocketDRequest: public Request
	{
	public:
		virtual void read();
		SocketDRequest(FCGX_Stream *in, FCGX_ParamArray params);
		virtual ~SocketDRequest();
	};

} /* namespace cppsp */

#endif /* CPPSP_SOCKETD_H_ */

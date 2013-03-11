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

namespace cppsp
{

	class SocketDRequest: public Request
	{
	public:
		virtual void read();
		SocketDRequest(CP::Socket& s);
		virtual ~SocketDRequest();
	};

} /* namespace cppsp */

#endif /* CPPSP_SOCKETD_H_ */

/*
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * */
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

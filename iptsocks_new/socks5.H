/*
 * socks5.H
 *
 *  Created on: Mar 23, 2013
 *      Author: xaxaxa
 */
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

#ifndef SOCKS5_H_
#define SOCKS5_H_
#include <cpoll/cpoll.H>
#include <cpoll/delegate.H>
using namespace CP;
namespace SOCKS5
{
	//typedef boost::function<void (Stream*, void*)> Callback;
	typedef Delegate<void(Stream&, exception* ex)> Callback;

	void socks_connect(Stream& s, const char* host, uint16_t port, Callback cb, Callback sent_cb=nullptr);
	void socks_connect(Stream& s, EndPoint& ep, Callback cb, Callback sent_cb=nullptr);
};

#endif /* SOCKS5_H_ */

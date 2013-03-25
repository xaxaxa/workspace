/*
 * socks5.H
 *
 *  Created on: Mar 23, 2013
 *      Author: xaxaxa
 */

#ifndef SOCKS5_H_
#define SOCKS5_H_
#include <cpoll/cpoll.H>
#include <delegate.H>
using namespace CP;
namespace SOCKS5
{
	//typedef boost::function<void (Stream*, void*)> Callback;
	typedef Delegate<void(Stream&, exception* ex)> Callback;

	void socks_connect(Stream& s, const char* host, uint16_t port, Callback cb, Callback sent_cb=nullptr);
	void socks_connect(Stream& s, EndPoint& ep, Callback cb, Callback sent_cb=nullptr);
};

#endif /* SOCKS5_H_ */

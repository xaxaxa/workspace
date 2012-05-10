/*
 * PacketQueue.H
 *
 *  Created on: Apr 10, 2012
 *      Author: s-wang_cary
 */

#ifndef PACKETQUEUE_H_
#define PACKETQUEUE_H_
#include "defines.H"
#include <cplib/cplib.hpp>
#include <cplib/asyncsock.hpp>
#include <queue>
#include <boost/shared_ptr.hpp>
using namespace std;
using namespace boost;

namespace xaxaxa
{
	using namespace Sockets;
	class UDPQueue
	{
	public:
		struct packet
		{
			boost::shared_ptr<EndPoint> ep;
			Buffer data;
		};
		queue<packet> q;
		bool dosend,sending;
		Socket s;
		UDPQueue(Socket s);
		virtual ~UDPQueue();
		void Append(const packet& p);
		void cb1(SocketManager* m, Socket s);
		void begin_send();
		void start();
		void stop();
	};

} /* namespace xaxaxa */
#endif /* PACKETQUEUE_H_ */

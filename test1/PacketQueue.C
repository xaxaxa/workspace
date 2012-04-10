/*
 * PacketQueue.C
 *
 *  Created on: Apr 10, 2012
 *      Author: s-wang_cary
 */

#include "PacketQueue.H"

namespace xaxaxa
{
	
	UDPQueue::UDPQueue(Socket s) :
			dosend(false), sending(false), s(s)
	{
		// TODO Auto-generated constructor stub
		
	}
	
	UDPQueue::~UDPQueue()
	{
		// TODO Auto-generated destructor stub
	}

	void UDPQueue::Append(const packet & p)
	{
		q.push(p);
	}

	void UDPQueue::cb1(SocketManager* m, Socket s)
	{
		sending = false;
		m->EndSendTo(s, *q.front().ep.get());
		q.pop();
		begin_send();
	}

	void UDPQueue::begin_send()
	{
		if (q.empty() || !dosend)
		{
			sending = false;
			return;
		}
		sending = true;
		SocketManager::GetDefault()->BeginSend(s, q.front().data, SocketManager::Callback(&UDPQueue::cb1, this));
	}

	void UDPQueue::start()
	{
		dosend = true;
		if (!sending) begin_send();
	}

	void UDPQueue::stop()
	{
		dosend = false;
	}

} /* namespace xaxaxa */

/*
 * PacketQueue.H
 *
 *  Created on: Apr 10, 2012
 *      Author: s-wang_cary
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

#ifndef PACKETQUEUE_H_
#define PACKETQUEUE_H_
#include <cpoll/cpoll.H>
#include <queue>
#include <delegate.H>
using namespace std;

namespace xaxaxa
{
	using namespace CP;

	//intrusive linked list; T must contain 2 pointers named __prev and __next that are of type T*
	template<class T>
	struct linkedList
	{
		T* first;
		T* last;
		int length;
		linkedList() :
				first(NULL), last(NULL), length(0) {
		}
		//you have to allocate the object yourself
		void append(T* item) {
			if (first == NULL) first = item;
			item->__prev = last;
			item->__next = NULL;
			if (last != NULL) last->__next = item;
			last = item;
			length++;
		}
		//before can not be NULL
		void insertBefore(T* item, T* before) {
			item->__prev = before->__prev;
			item->__next = before;
			before->__prev->__next = item;
			before->__prev = item;
			if (first == before) first = item;
			length++;
		}
		void remove(T* item) {
			if (item->__prev != NULL) item->__prev->__next = item->__next;
			if (item->__next != NULL) item->__next->__prev = item->__prev;
			if (first == item) first = item->__next;
			if (last == item) last = item->__prev;
			length--;
		}
	};
	class UDPQueue
	{
	public:
		struct packet
		{
			packet* __prev;
			packet* __next;
			EndPoint* ep;
			uint8_t* data;
			int epSize;
			int datalen;
		};
		linkedList<packet> q;
		packet* curPacket;
		//queue<packet*> q;
		RGC::Ref<Socket> s;
		Delegate<void(uint8_t*)> sent;
		bool dosend, sending;
		UDPQueue(Socket& s);
		virtual ~UDPQueue();
		uint8_t* beginAppend(int len, const EndPoint& ep);
		void append(void* data, int len, const EndPoint& ep);
		void endAppend();
		void cb1(int r);
		void begin_send();
		void start();
		void stop();
	};

} /* namespace xaxaxa */
#endif /* PACKETQUEUE_H_ */

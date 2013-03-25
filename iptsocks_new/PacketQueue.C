/*
 * PacketQueue.C
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

#include "PacketQueue.H"
#include <stdexcept>
namespace xaxaxa
{
	UDPQueue::UDPQueue(Socket& s) :
			s(s), dosend(false), sending(false) {
	}
	UDPQueue::~UDPQueue() {

	}
	uint8_t* UDPQueue::beginAppend(int len, const EndPoint& ep) {
		int s = sizeof(packet) + EndPoint::getSize(ep.addressFamily) + len;
		curPacket = (packet*) malloc(s);
		if (curPacket == NULL) return NULL;
		curPacket->epSize = EndPoint::getSize(ep.addressFamily);
		curPacket->ep = EndPoint::construct(curPacket + 1, ep.addressFamily);
		ep.clone(*curPacket->ep);
		curPacket->data = ((uint8_t*) (curPacket + 1)) + curPacket->epSize;
		curPacket->datalen = len;
		return curPacket->data;
	}
	void UDPQueue::endAppend() {
		q.append(curPacket);
		curPacket = NULL;
	}
	void UDPQueue::append(void* data, int len, const EndPoint& ep) {
		int s = sizeof(packet) + EndPoint::getSize(ep.addressFamily);
		packet* tmpp = (packet*) malloc(s);
		if (tmpp == NULL) throw runtime_error(strerror(errno));
		tmpp->epSize = EndPoint::getSize(ep.addressFamily);
		tmpp->ep = EndPoint::construct(tmpp + 1, ep.addressFamily);
		ep.clone(*tmpp->ep);
		tmpp->data = ((uint8_t*) data);
		tmpp->datalen = len;
		q.append(tmpp);
	}
	void UDPQueue::cb1(int r) {
		sending = false;
		//m->EndSendTo(s, *q.front().ep.get());
		q.first->ep->~EndPoint();
		packet* tmpp = q.first;
		if (sent != nullptr) sent(tmpp->data);
		q.remove(tmpp);
		free(tmpp);
		begin_send();
	}
	void UDPQueue::begin_send() {
		if (q.length == 0 || !dosend) {
			sending = false;
			return;
		}
		sending = true;
		s->sendTo(q.first->data, q.first->datalen, 0, *q.first->ep, Callback(&UDPQueue::cb1, this));
	}
	void UDPQueue::start() {
		dosend = true;
		if (!sending) begin_send();
	}
	void UDPQueue::stop() {
		dosend = false;
	}
} /* namespace xaxaxa */


/*
 * client.h
 *
 *  Created on: 2011-09-16
 *      Author: xaxaxa
 */

#ifndef CLIENT_H_
#define CLIENT_H_
#include <list>
namespace p2pc {

class client {
public:
	client();
	virtual ~client();
	const int clientlimit = 50;//start rejecting clients at
	const int maxclient = 30;//wont sent out connection requests after this
	const int minclient = 20;//start sending requests if lower
	list<peer_connection> p;//linked list
	void check_peers();//enforce connection limits and send requests
	void add_peer(peer_ep* ep);
};

}

#endif /* CLIENT_H_ */

namespace p2pc
{
	class client
	{
		const int clientlimit=50;//start rejecting clients at
		const int maxclient=30;//wont sent out connection requests after this
		const int minclient=20;//start sending requests if lower
		peer_connection[] p;//linked list
		void check_peers();//enforce connection limits and send requests
		void add_peer(peer_ep* ep);
	};
	/*class peer//encapsulates a peer_ep and a peer_connection(if connected)
	{
		peer_ep* ep;
		peer_connection* con;
		void connect();
		void disconnect();
		bool connected;
	};*/
	class peer_ep//info about location of a peer
	{
		delegate void callback(peer_connection* ep/*NULL if unsuccessful*/);
		void connect(callback cb);
		operator ==();
		operator <();
		operator >();
	};
	class peer_connection//an actual connection to a peer(tcp,udp,sctp)
	{
		Stream* s;
		peer_ep* ep;
		void disconnect();
	};
	class peer_source
	{
		peer_ep* get(bool mark/*mark as in use so as to not to return it again later*/);
		//return NULL if no more unused peers;
		void ret(peer_ep* ep,bool markAsDown);//unmark it as in use; mark the peer as down; tells it to not return the same peer in a certain amount of time;
		//if a peer has been down for a long time(~3 month), then delete it
		
	};
}

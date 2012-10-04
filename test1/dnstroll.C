#define WARNLEVEL 8
#include "defines.H"
#include <iostream>
#include <cplib/cplib.hpp>
#include <cplib/asyncsock.hpp>
#include <cplib/asyncfile.hpp>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "JoinStream.h"
#include <boost/shared_ptr.hpp>
#include "DNSServer.H"
#include <map>
#include <list>
#include <tuple>
using namespace boost;
using namespace std;
using namespace xaxaxa;
using namespace Sockets;

int dnstroll_main(int argc, char** argv) {
	if (argc < 3) {
		cout << "usage: " << argv[0] << " listen_ip response_ip" << endl;
		return 1;
	}
	signal(SIGPIPE, SIG_IGN );
	struct sigaction sa;
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART; /* Restart functions if
	 interrupted by handler */
	sigaction(SIGHUP, &sa, NULL);
	sa.sa_handler = SIG_DFL;
	//cout<<sigaction(SIGSTOP, &sa, NULL)<<endl;
	//cout<<errno<<endl;
	sigaction(SIGCONT, &sa, NULL);
	sigaction(SIGTSTP, &sa, NULL);
	sigaction(SIGTTIN, &sa, NULL);
	sigaction(SIGTTOU, &sa, NULL);
	IPAddress resp_ip(argv[2]);
	auto answer_request = [&](DNSServer& srv, const EndPoint& ep, const DNSServer::dnsreq& req)
	{
		WARN(6,"RECEIVED DNS PACKET");
		/*auto tmp=pool.get();
		 if(tmp.second)ip=tmp.first;
		 else ip=IPAddress("127.0.0.1");*/

		DNSServer::dnsreq resp(req.create_answer());
		for(int i=0;i<(int)resp.queries.size();i++)
		{
			Buffer tmpb((Byte*)&resp_ip.a, sizeof(resp_ip.a));
			DNSServer::answer a
			{	i,resp.queries[i].type,resp.queries[i].cls,100000000,tmpb};
			resp.answers.push_back(a);
		}
		srv.sendreply(ep, resp);
	};
	DNSServer* srv;
	srv = new DNSServer(IPEndPoint(IPAddress(argv[1]), 53), answer_request);
	srv->start();
	SocketManager::GetDefault()->EventLoop();
}


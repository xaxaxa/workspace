/*
 * config.hpp
 *
 *  Created on: 2011-05-20
 *      Author: xaxaxa
 */

#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include <cpoll.H>
#include <rgc.H>
#include <string>
#include <map>
#include <vector>
#include "socketd.H"
using namespace std;

namespace socketd
{
	struct binding;
	struct vhost;
	struct listen
	{
	public:
		//user supplied fields
		RGC::Ref<CP::EndPoint> ep;
		int id;
		int backlog;
		//internal fields
		RGC::Ref<CP::Socket> sock;
		listen(CP::EndPoint* ep, int id, int backlog = 32) :
				ep(ep), id(id), backlog(backlog) {
		}
	};
	struct binding
	{
	public:
		//user supplied fields
		string httpPath; //path must not have / at the end!!!!!
		string httpHost;
		int listenID;

		//bitmap:
		//0: match listenID
		//1: match httpPath
		//2: match httpHost
		int matchLevel;
		enum
		{
			match_listenID = 1, match_httpPath = 2, match_httpHost = 4
		};
		binding() {
		}
		binding(int listenID, string httpPath, string httpHost, int matchLevel) :
				httpPath(httpPath), httpHost(httpHost), listenID(listenID), matchLevel(matchLevel) {
		}
		//internal fields
		vhost* vh;
	};
	struct appConnection: public RGC::Object
	{
		vhost* vh;
		//						sock		success
		typedef function<void(bool)> passConnCB;
		appConnection(vhost* vh);
		virtual void shutDown()=0;

		//return values: 0: success; 1: failed; 2: in progress (passConnCB() will be called later)
		virtual int passConnection(CP::Socket* s, void* buffer, int buflen, const passConnCB& cb)=0;
		virtual ~appConnection();
	};
	struct vhost: public RGC::Object
	{
	public:
		//user supplied fields
		vector<binding> bindings;
		string name;
		string exepath; //leave blank (length==0) to disable execing and just wait for attachment

		/*int requestsPerProcess;
		 int maxRequestsPerProcess;
		 int maxProcesses;*/

		//for attaching to a running vhost; arbitrary string; leave blank (length==0) to
		//disable attachments
		string authCookie;
		vhost() {
		}
		vhost(const vector<binding>& bindings, string name, string exepath, string authCookie) :
				bindings(bindings), name(name), exepath(exepath), authCookie(authCookie) {
		}

		//internal fields
		vector<RGC::Ref<appConnection>> conns;//indexed by thread
		RGC::Ref<appConnection> attachmentConn;//readonly by threads
		bool hasAttachments;
	};
	class socketd
	{
	public:
		//user supplied fields
		vector<listen> listens;
		vector<vhost> vhosts;
		void run();

		//internal fields
		vector<binding*> bindings;
	};

}

#endif /* CONFIG_HPP_ */

/*
 * main.C
 *
 *  Created on: Feb 16, 2012
 *      Author: xaxaxa
 */
#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WEnvironment>
#include <Wt/WPushButton>
#include <Wt/WServer>
#include <Wt/WText>
#include <Wt/WResource>
#include <Wt/Http/Request>
#include <Wt/Http/Response>
#include "App.H"
#include "Server.H"
#include "bw3chatserver.H"
#include "SharedObjManager.H"

using namespace Wt;
using namespace chat;

SharedObjManager<long, GenericServer> servers;

class p_notify: public WResource
{
public:
	int x;
	p_notify(Wt::WObject *parent = 0) :
			WResource(parent), x(0)
	{
		//suggestFileName("data.txt");
	}
	virtual ~p_notify()
	{
		beingDeleted();
	}

	void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response)
	{
		response.setMimeType("text/plain");
		auto p = request.getParameterValues("id");
		if (p.size() <= 0)
		{
			response.out() << "server not created";
			return;
		}
		long id = atol((*(p.begin())).c_str());
		auto ptr = servers.Get(id);
		if (!ptr.Valid())
		{
			response.out() << "server not created";
			return;
		}
		bw3_chatserver* s = dynamic_cast<bw3_chatserver*>(ptr.Get());
		if(s==NULL)
			response.out() << "not a bw3_chatserver";
		else
		{
			s->Update();
			response.out() << "S";
		}
	}
};

WApplication* newapp(const WEnvironment& env, Wt::WServer& srv)
{
	auto p = env.getParameterValues("id");
	if (p.size() <= 0)
		return NULL;
	//bw3_chatserver* s;
	long id = atol((*(p.begin())).c_str());

	recursive_mutex::scoped_lock lock(servers.mutex);
	auto ptr = servers.Get(id);
	if (!ptr.Valid())
	{
		bw3_chatserver* s = new bw3_chatserver(srv, id);
		ptr = servers.Add(id, s);
	}
	return new App(env, ptr);
}
WApplication* newapp2(const WEnvironment& env)
{
	return NULL;
}
int main(int argc, char **argv)
{
	/*SharedObjManager<int, counterObj> asdf;
	 auto a1 = asdf.Add(3, new counterObj());
	 auto a2 = asdf.Add(5, new counterObj());
	 cout << "a2.Get() = " << a2.Get() << endl;
	 auto a3 = asdf.Add(6, new counterObj());

	 SharedObjPtr<int, counterObj> ptr1 = a2;
	 SharedObjPtr<int, counterObj> ptr2(ptr1);

	 auto tmp1 = asdf.Get(5);
	 cout << "tmp1.Get() = " << tmp1.Get() << endl;

	 a1.Release();
	 a2.Release();
	 a3.Release();
	 cout << "a1, a2, a3 released" << endl;
	 return 0;*/
	Wt::WServer server(argv[0]);
	server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);
	server.addEntryPoint(Wt::Application, boost::bind(newapp, _1, boost::ref(server)),
			"/index.htm");
	//server.addEntryPoint(Wt::StaticResource, &newapp2, "/notify.htm");
	p_notify n;
	server.addResource(&n, "/notify.htm");
	if (server.start())
	{
		int sig = Wt::WServer::waitForShutdown();
		std::cerr << "Shutting down: (signal = " << sig << ")" << std::endl;
		server.stop();
		return 0;
	}
	return 1;
}

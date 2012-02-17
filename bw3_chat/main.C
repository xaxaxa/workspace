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
#include "App.H"
#include "Server.H"
#include "bw3chatserver.H"
using namespace Wt;
using namespace chat;

WApplication* newapp(const WEnvironment& env, GenericServer* s)
{
	return new App(env, *s);
}
int main(int argc, char **argv)
{
	Wt::WServer server(argv[0]);
	bw3_chatserver s(server);
	//return 0;
	server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);
	server.addEntryPoint(Wt::Application, boost::bind(newapp,_1,&s), "/index.xxx");
	if (server.start())
	{
		int sig = Wt::WServer::waitForShutdown();
		std::cerr << "Shutting down: (signal = " << sig << ")" << std::endl;
		server.stop();
		return 0;
	}
	return 1;
}

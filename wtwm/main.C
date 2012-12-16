#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WEnvironment>
#include <Wt/WPushButton>
#include <Wt/WServer>
#include <Wt/WText>
#include <Wt/WResource>
#include <Wt/Http/Request>
#include <Wt/Http/Response>
//class JSWindow: public Wt::WContainerWidget
class WMWidget: public Wt::WContainerWidget
{
	
};
class WTWM: public WApplication
{
	WTWM(const WEnvironment& env): WApplication(env) {
		
	}
};

int main(int argc, char **argv)
{
	Wt::WServer server(argv[0]);
	server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);
	server.addEntryPoint(Wt::Application, boost::bind(newapp, _1, boost::ref(server)),
			"/index.htm");
	//server.addEntryPoint(Wt::StaticResource, &newapp2, "/notify.htm");
	//p_notify n;
	//server.addResource(&n, "/notify.htm");
	if (server.start())
	{
		int sig = Wt::WServer::waitForShutdown();
		std::cerr << "Shutting down: (signal = " << sig << ")" << std::endl;
		server.stop();
		return 0;
	}
	return 1;
}

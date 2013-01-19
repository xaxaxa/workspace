#include <generic_gui/control.H>
#include <generic_gui/gtkviewport.H>
#include <generic_gui/wtviewport.H>
#include <Wt/WServer>
GenericGUI::Controls::Window* wnd;
GenericGUI::Controls::Button* b;
Wt::WApplication* newapp(const Wt::WEnvironment& env, Wt::WServer& srv)
{
	GenericGUI::Viewports::WTApplication* app=new GenericGUI::Viewports::WTApplication(env);
	b->implement(&app->viewport);
	return app;
}

int main(int argc, char *argv[])
{
	GenericGUI::Viewports::GTKViewport vp(argc, argv);
	//GenericGUI::Viewports::WTViewport vp(argc, argv);
	//GenericGUI::Viewports::GTKViewport vp2(argc, argv);
	//GenericGUI::Viewports::GTKViewport vp3(argc, argv);
	
	GenericGUI::Controls::Window w;
	wnd=&w;
	w.setGeometry({{-1,-1,-1,-1,500,300}});
	w.setTitle("aaaaa");
	auto impl=w.implement(&vp);
	
	GenericGUI::Controls::Table cont;
	cont.setGeometry({{0,100,100,0,-1,-1}});
	cont.setSize({{2,2}});
	w.addChild(&cont);
	
	b=new GenericGUI::Controls::Button();
	cont.setChild(0,0,b,1,1);
	b->setText("zxcv");
	b->setGeometry({{-1,-1,-1,-1,-1,100}});
	b->release();
	b=new GenericGUI::Controls::Button();
	cont.setChild(0,1,b,1,1);
	b->setText("gbasdjhvdjshbjhsa");
	b->setGeometry({{-1,-1,-1,-1,-1,100}});
	b->release();
	//b->setGeometry({{-1,0,0,-1,100,100}});
	
	GenericGUI::Controls::Button b1;
	cont.setChild(1,0,&b1,1,2);
	b1.setText("gfsdhjfgjhsd");
	b1.setPosition(tuple<int,int>(-1,-1));
	b1.click += [&]() {
		b1.setText("clicked");
	};
	
	
	GenericGUI::Controls::Container cont1;
	cont1.setGeometry({{-1,0,0,0,-1,100}});
	printf("zxcvbnm\n");
	w.addChild(&cont1);
	GenericGUI::Controls::Button b2;
	b2.setGeometry({{0,-1,-1,0,10,10}});
	printf("aaaaa\n");
	cont1.addChild(&b2);
	[](){return [](){return [](){};}();}()();
	
	//auto impl=w.implement(&vp);
	//auto impl2=w.implement(&vp2);
	//auto impl3=w.implement(&vp3);
	
	
	vp.run();
	
	/*Wt::WServer server(argv[0]);
	server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);
	server.addEntryPoint(Wt::Application, boost::bind(newapp, _1, boost::ref(server)),
			"/index.htm");
	if (server.start())
	{
		int sig = Wt::WServer::waitForShutdown();
		std::cerr << "Shutting down: (signal = " << sig << ")" << std::endl;
		server.stop();
		return 0;
	}*/
	return 1;
}

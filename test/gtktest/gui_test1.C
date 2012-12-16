#include <generic_gui/control.H>
#include <generic_gui/gtkviewport.H>

int main(int argc, char *argv[])
{
	GenericGUI::Viewports::GTKViewport vp(argc, argv);
	GenericGUI::Viewports::GTKViewport vp2(argc, argv);
	//GenericGUI::Viewports::GTKViewport vp3(argc, argv);
	
	GenericGUI::Controls::Window w;
	w.setTitle("aaaaa");
	//auto impl=w.implement(&vp);
	
	GenericGUI::Controls::Table cont;
	cont.setGeometry({{0,0,0,0,-1,-1}});
	cont.setSize({{2,2}});
	w.addChild(&cont);
	
	GenericGUI::Controls::Button* b=new GenericGUI::Controls::Button();
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
	
	auto impl=w.implement(&vp);
	//auto impl2=w.implement(&vp2);
	//auto impl3=w.implement(&vp3);
	
	vp.run();
}

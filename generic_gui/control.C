/*
 * control.C
 *
 *  Created on: Dec 8, 2012
 *      Author: xaxaxa
 */
#include "include/control.H"
#include "include/gtkviewport.H"
#include <gtkmm.h>
namespace GenericGUI
{
	void Element::propertyChange(int offset) {
		ImplEventData ed(ImplEventData::t_propertyChange);
		ed.propertyChange.offset = offset;
		ImplEvent(ed);
	}
	Element::Element() :
			firstChild(NULL), lastChild(NULL), prev(NULL), next(NULL), parent(NULL) {

	}
	Element::~Element() {
		clearChildren();
	}
	void Element::addChild(Element* e, Element* before) {
		if (e->parent != NULL) e->parent->removeChild(e);
		e->retain();
		e->parent = this;

		e->next = before;
		if (before == NULL) {
			e->prev = e->parent->lastChild;
			if (lastChild != NULL) {
				lastChild->next = e;
			}
			lastChild = e;
		} else if (before == firstChild) {
			firstChild->prev = e;
			firstChild = e;
		} else {
			before->prev->next = e;
			e->prev = before->prev;
			before->prev = e;
		}
		if (firstChild == NULL) firstChild = e;
		if (lastChild == NULL) lastChild = e;

		//addRemoveChild(false, e);
		ImplEventData ed(ImplEventData::t_addRemoveChild);
		ed.addRemoveChild.isRemove = false;
		ed.addRemoveChild.element = e;
		ImplEvent(ed);
	}
	void Element::removeChild(Element* e) {
		{
			ImplEventData ed(ImplEventData::t_addRemoveChild);
			ed.addRemoveChild.isRemove = true;
			ed.addRemoveChild.element = e;
			ImplEvent(ed);
		}
		if (e->parent != this) return;
		if (firstChild == e) firstChild = e->next;
		if (lastChild == e) lastChild = e->prev;
		if (e->prev != NULL) e->prev->next = e->next;
		if (e->next != NULL) e->next->prev = e->prev;
		e->next = e->prev = e->parent = NULL;
		e->release();
	}
	void Element::clearChildren() {
		{
			ImplEventData ed(ImplEventData::t_addRemoveChild);
			ed.addRemoveChild.isRemove = true;
			ed.addRemoveChild.element = NULL;
			ImplEvent(ed);
		}
		Element* e;
		for (e = firstChild; e != NULL; e = e->next) {
			e->prev = e->next = e->parent = NULL;
			e->release();
		}
		firstChild = lastChild = NULL;
	}

	ImplMap::ImplMap() {
		memset(&items, 0, sizeof(items));
	}
	bool ImplMap::add(Viewport* vp, Implementation* impl) {
		int i;
		for (i = 0; i < MAX_IMPLEMENTATIONS_PER_CONTROL; i++) {
			if (items[i].vp == NULL) break;
			if (items[i].vp == vp) return false;
		}
		if (likely(i < MAX_IMPLEMENTATIONS_PER_CONTROL)) {
			items[i].vp = vp;
			items[i].impl = impl;
			return true;
		}
		return false;
	}
	Implementation* ImplMap::find(Viewport* vp) {
		for (int i = 0; i < MAX_IMPLEMENTATIONS_PER_CONTROL; i++) {
			if (items[i].vp == vp) return items[i].impl;
		}
		return NULL;
	}
	void ImplMap::remove(Viewport* vp) {
		for (int i = 0; i < MAX_IMPLEMENTATIONS_PER_CONTROL; i++) {
			if (items[i].vp == vp) {
				for (i++; i < MAX_IMPLEMENTATIONS_PER_CONTROL; i++)
					items[i - 1] = items[i];
				return;
			}
		}
	}

	Implementation::Implementation(Control* control, Viewport* viewport) :
			control(control), viewport(viewport) {

	}
	Implementation::~Implementation() {
		//clearChildren();
		control->implementations.remove(viewport);
	}
	void Implementation::initialize() {

	}
	/*void Implementation::addChild(Implementation* impl) {
	 if (impl->parent == this) return;
	 impl->retain();
	 if (impl->parent != NULL) impl->parent->removeChild(impl);
	 impl->parent = this;
	 impl->next = NULL;
	 impl->prev = impl->parent->lastChild;
	 if (lastChild != NULL) {
	 lastChild->next = impl;
	 }
	 lastChild = impl;

	 if (firstChild == NULL) firstChild = impl;
	 //if (lastChild == NULL) lastChild = impl;
	 }
	 void Implementation::removeChild(Implementation* impl) {
	 if (impl->parent != this) return;
	 if (firstChild == impl) firstChild = impl->next;
	 if (lastChild == impl) lastChild = impl->prev;
	 if (impl->prev != NULL) impl->prev->next = impl->next;
	 if (impl->next != NULL) impl->next->prev = impl->prev;
	 impl->next = impl->prev = impl->parent = NULL;
	 impl->release();
	 }
	 void Implementation::clearChildren() {
	 Implementation* impl;
	 for (impl = firstChild; impl != NULL; impl = impl->next) {
	 impl->prev = impl->next = impl->parent = NULL;
	 impl->release();
	 }
	 firstChild = lastChild = NULL;
	 }*/
	Control::Control() :
			geometry( { { 0, -1, -1, 0, -1, -1 } }), visible(true) {

	}
	void Control::setVisible(bool b) {
		if (visible == b) return;
		visible = b;
		propertyChange(offsetof(Control, visible));
	}
	bool Control::getVisible() {
		return visible;
	}
	void Control::setGeometry(const Geometry& g) {
		this->geometry = g;
		propertyChange(offsetof(Control, geometry));
	}
	void Control::setPosition(const tuple<int, int>& pos) {
		//position = pos;
		//propertyChange(offsetof(Control, position));
		//auto g = getGeometry();
		Geometry& g(geometry);
		setGeometry(
				Geometry(
						{ { get < 1 > (pos), get < 1 > (g), get < 2 > (g), get < 0 > (pos), get < 4
								> (g), get < 5 > (g) } }));
	}
	Control::Geometry Control::getGeometry() {
		return geometry;
	}
	tuple<int, int> Control::getPosition() {
		return tuple<int, int>(get < 3 > (geometry), get < 0 > (geometry));
	}
	RGC::Ref<Implementation> Control::implement(Viewport* vp) {
		auto tmp = implementations[vp];
		if (tmp != NULL) return tmp;
		ImplementationCtor* func = findImplementation(typeid(*this), typeid(*vp));
		if (func == NULL) throw runtime_error(
				"could not find implementation for control " + string(typeid(*this).name())
						+ " with viewport " + string(typeid(*vp).name()));
		Implementation* impl = (*func)(this, vp);
		impl->initialize();
		RGC::Ref<Implementation> ref = impl;
		impl->release();
		if (likely(implementations.add(vp, impl))) return ref;
		else throw runtime_error("exceeded the maximum # of implementations per control");
	}
	struct implInfo
	{
		const type_info* ctrl;
		const type_info* viewport;
	};
	bool operator<(const implInfo& a, const implInfo& b) {
		if (a.ctrl == b.ctrl) return a.viewport < b.viewport;
		return a.ctrl < b.ctrl;
	}
	map<implInfo, ImplementationCtor> implementations;
	bool baseImpl_registered = false;
	void registerBaseImpl();
	ImplementationCtor* findImplementation(const type_info& ctrl, const type_info& viewport) {
		if (!baseImpl_registered) {
			baseImpl_registered = true;
			registerBaseImpl();
		}
		auto it = implementations.find( { &ctrl, &viewport });
		if (it == implementations.end()) return NULL;
		return &((*it).second);
	}
	void registerImplementation(const type_info& ctrl, const type_info& viewport,
			const ImplementationCtor& ctor) {
		implementations.insert( { { &ctrl, &viewport }, ctor });
	}
	namespace Controls
	{
		class GTKControlImpl: public Viewports::GTKImplementation
		{
		public:
			int i_event;
			int last_width, last_height;
			int last_set_width, last_set_height;
			sigc::connection c_size_allocate;
			//actual onscreen size, not the user-specified geometry
			virtual void childAllocationChange(Control* c) {

			}
			virtual void allocationChange(int new_width, int new_height) {
				if (this->control->parent != NULL) {
					Control* ctrl = dynamic_cast<Control*>(this->control->parent);
					if (ctrl == NULL) return;
					auto tmp = ctrl->implementations[this->viewport];
					if (tmp == NULL) return;
					GTKControlImpl* impl = dynamic_cast<GTKControlImpl*>(tmp);
					//printf("childSizeChange: %s\n", typeid(*impl).name());
					impl->childAllocationChange(control);
				}
				last_width = new_width;
				last_height = new_height;
			}
			virtual void childGeometryChange(Control* c) {

			}
			virtual void initialize() {
				Viewports::GTKImplementation::initialize();
				auto wid = this->getWidget();
				auto alloc = wid->get_allocation();
				last_width = alloc.get_width();
				last_height = alloc.get_height();
				c_size_allocate = wid->signal_size_allocate().connect([this](Gtk::Allocation& a) {
					if(a.get_width()==last_width && a.get_height()==last_height) return;
					//printf("signal_size_allocate\n");
						allocationChange(a.get_width(), a.get_height());
					});
				if (control->visible) wid->show();
				i_event = control->ImplEvent += [this](const ImplEventData& ed) {
					if(ed.type==ImplEventData::t_propertyChange)
					switch(ed.propertyChange.offset) {
						case offsetof(Control,visible):
						{
							if(control->getVisible()) this->getWidget()->show();
							else this->getWidget()->hide();
						}
						break;
						case offsetof(Control,geometry):
						{
							Control* ctrl = dynamic_cast<Control*>(this->control->parent);
							if (ctrl == NULL) return;
							auto tmp = ctrl->implementations[this->viewport];
							if (tmp == NULL) return;
							GTKControlImpl* impl = dynamic_cast<GTKControlImpl*>(tmp);
							impl->childGeometryChange(control);
						}
						break;
					}
				};
			}
			GTKControlImpl(Control* control, Viewport* viewport) :
					Viewports::GTKImplementation(control, viewport), last_width(-1), last_height(-1) {

				/*
				 i_event = control->ImplEvent += [this](const ImplEventData& ed) {
				 if(ed.type!=ImplEventData::t_propertyChange) return;
				 if(ed.propertyChange.offset==offsetof(Control,geometry)) {

				 }
				 };*/
			}
			~GTKControlImpl() {
				//control->ImplEvent -= i_event;
				c_size_allocate.disconnect();
				control->ImplEvent -= i_event;
				this->getWidget()->hide();
			}
		};
		class GTKFixed1: public Gtk::Fixed
		{
			virtual void on_check_resize() override
			{
				printf("on_check_resize");
			}
			virtual void on_parent_changed(Widget* previous_parent) override
			{
			}
		};
		class GTKButton1: public Gtk::Button
		{
			virtual void on_check_resize() override
			{
				printf("GTKButton1::on_check_resize");
			}
			virtual void on_parent_changed(Widget* previous_parent) override
			{
			}
		};
		class GTKContainerImpl: public GTKControlImpl
		{
		public:
			//Gtk::Fixed f;
			int i_event;
			virtual void createWidget() {
				widget = new GTKFixed1();
				//Gtk::Fixed& f(*(Gtk::Fixed*) widget);
				//f.set_resize_mode(Gtk::RESIZE_PARENT);
			}
			void repositionChild(Control* c, int width, int height) {
				Gtk::Fixed& f(*(Gtk::Fixed*) getWidget());
				auto g = c->getGeometry();
				GTKControlImpl* impl = static_cast<GTKControlImpl*>(c->implementations[viewport]);
				auto wid = impl->getWidget();
				int x, y, w, h;

				int T, R, B, L, W, H;
				T = get < 0 > (g);
				R = get < 1 > (g);
				B = get < 2 > (g);
				L = get < 3 > (g);
				W = get < 4 > (g);
				H = get < 5 > (g);

				//width
				if (L >= 0 && R >= 0) w = width - R - L;
				else w = W;
				//height
				if (T >= 0 && B >= 0) h = height - B - T;
				else h = H;
				wid->set_size_request(w, h);
				auto alloc = wid->get_allocation();
				W = alloc.get_width();
				H = alloc.get_height();

				//top
				if (T < 0) {
					if (B < 0) y = height / 2 - H / 2;
					else y = height - B - H;
				} else y = T;
				//left
				if (L < 0) {
					if (R < 0) x = width / 2 - W / 2;
					else x = width - R - W;
				} else x = L;

				printf("x: %i y: %i w: %i h: %i\n", x, y, W, H);

				f.move(*wid, x, y);
			}
			virtual void childAllocationChange(Control* c) {
				//printf("childSizeChange()\n");
				//auto pos = c->getPosition();
				Gtk::Fixed& f(*(Gtk::Fixed*) getWidget());
				//GTKControlImpl* impl = static_cast<GTKControlImpl*>(c->implementations[viewport]);
				auto alloc = f.get_allocation();
				int width = last_width = alloc.get_width();
				int height = last_height = alloc.get_height();
				repositionChild(static_cast<Control*>(c), width, height);
			}
			virtual void childGeometryChange(Control* c) {
				Gtk::Fixed& f(*(Gtk::Fixed*) getWidget());
				auto alloc = f.get_allocation();
				int width = last_width = alloc.get_width();
				int height = last_height = alloc.get_height();
				repositionChild(static_cast<Control*>(c), width, height);
			}
			virtual void allocationChange(int nw, int nh) {
				//printf("reallocate\n");
				GTKControlImpl::allocationChange(nw, nh);
				for (Element* e = this->control->firstChild; e != NULL; e = e->next) {
					repositionChild(static_cast<Control*>(e), nw, nh);
				}
			}
			void processChild(Element* e, bool reposition = true) {
				Gtk::Fixed& f(*(Gtk::Fixed*) getWidget());
				Control* c = dynamic_cast<Control*>(e);
				auto impl = c->implement(viewport).staticCast<GTKControlImpl>();
				auto wid = impl->getWidget();
				auto pos = c->getPosition();
				f.add(*wid);
				f.move(*wid, get < 0 > (pos), get < 1 > (pos));
				impl.get()->retain();
				if (reposition) {
					auto alloc = f.get_allocation();
					repositionChild(c, alloc.get_width(), alloc.get_height());
				}
			}
			virtual void initialize() {
				GTKControlImpl::initialize();
				Gtk::Fixed& f(*(Gtk::Fixed*) getWidget());
				i_event =
						control->ImplEvent +=
								[this](const ImplEventData& ed) {
									if(ed.type!=ImplEventData::t_addRemoveChild)return;

									Gtk::Fixed& f(*(Gtk::Fixed*) getWidget());
									if(ed.addRemoveChild.isRemove) {
										Control* c = dynamic_cast<Control*>(ed.addRemoveChild.element);
										auto tmp=dynamic_cast<GTKImplementation*>(c->implementations[this->viewport]);
										f.remove(*(tmp->getWidget()));
										if(tmp!=NULL) tmp->release();
									} else {
										processChild(ed.addRemoveChild.element);
									}
								};
				auto alloc = f.get_allocation();
				int width = last_width = alloc.get_width();
				int height = last_height = alloc.get_height();
				Element* e;
				for (e = control->firstChild; e != NULL; e = e->next) {
					processChild(e, false);
					repositionChild(static_cast<Control*>(e), width, height);
				}
			}
			GTKContainerImpl(Control* control, Viewport* viewport) :
					GTKControlImpl(control, viewport) {

			}
			~GTKContainerImpl() {
				control->ImplEvent -= i_event;
				Element* e;
				for (e = control->firstChild; e != NULL; e = e->next) {
					Control* c = dynamic_cast<Control*>(e);
					auto tmp = c->implementations[viewport];
					if (tmp != NULL) tmp->release();
				}
			}
		};
		class GTKWindowImpl: public GTKControlImpl
		{
		public:
			//Gtk::Window wnd;
			GTKContainerImpl cont;
			int i_event;
			virtual void childAllocationChange(Control* c) {
				cont.childAllocationChange(c);
			}
			virtual void createWidget() {
				widget = new Gtk::Window();
			}
			virtual void initialize() {
				GTKControlImpl::initialize();
				cont.initialize();
				Gtk::Window& wnd(*(Gtk::Window*) getWidget());
				Window* ctrl = static_cast<Window*>(control);
				wnd.set_title(ctrl->getTitle());
				wnd.add(*(cont.getWidget()));
				i_event = control->ImplEvent += [this](const ImplEventData& ed) {
					if(ed.type!=ImplEventData::t_propertyChange) return;
					Gtk::Window& wnd(*(Gtk::Window*) getWidget());
					Window* ctrl = static_cast<Window*>(this->control);
					if(ed.propertyChange.offset==offsetof(Window, title)) {
						wnd.set_title(ctrl->getTitle());
					}
				};
			}
			GTKWindowImpl(Control* control, Viewport* viewport) :
					GTKControlImpl(control, viewport), cont((Container*) control, viewport) {

			}
			virtual ~GTKWindowImpl() {
				control->ImplEvent -= i_event;
			}
		};
		class GTKButtonImpl: public GTKControlImpl
		{
		public:
			sigc::connection c_clicked;
			int i_event;
			//Gtk::Button b;
			virtual void createWidget() {
				widget = new GTKButton1();
			}
			virtual void initialize() {
				GTKControlImpl::initialize();
				Gtk::Button& b(*(Gtk::Button*) getWidget());
				Button* ctrl = static_cast<Button*>(control);
				b.set_label(ctrl->getText());
				i_event = control->ImplEvent += [this](const ImplEventData& ed) {
					if(ed.type!=ImplEventData::t_propertyChange) return;
					Gtk::Button& b(*(Gtk::Button*) getWidget());
					Button* ctrl = static_cast<Button*>(this->control);
					if(ed.propertyChange.offset==offsetof(Button,visible)) {
						if(this->control->visible)b.show();
						else b.hide();
					} else if(ed.propertyChange.offset==offsetof(Button,text)) {
						b.set_label(ctrl->getText());
					}
				};
				c_clicked = b.signal_clicked().connect([this]() {
					Button* ctrl = static_cast<Button*>(this->control);
					ctrl->click();
				});
			}
			GTKButtonImpl(Control* control, Viewport* viewport) :
					GTKControlImpl(control, viewport) {

			}
			~GTKButtonImpl() {
				control->ImplEvent -= i_event;
				c_clicked.disconnect();
			}
		};
		class GTKTableImpl: public GTKControlImpl
		{
		public:
			int i_event;
			virtual void createWidget() {
				widget = new Gtk::Table();
			}
			void processChild(Control* c) {
				Table* ctrl = static_cast<Table*>(control);
				Gtk::Table& t(*(Gtk::Table*) getWidget());
				auto impl = c->implement(viewport).staticCast<GTKControlImpl>();
				impl.get()->retain();
				auto g = c->getGeometry();
				auto pos = ctrl->itemsMap[c];
				Table::Cell& cell = ctrl->items[pos[0]][pos[1]];
				auto alloc = t.get_allocation();

				//t.add(*(tmp->getWidget()));
				t.attach(*(impl->getWidget()), pos[1], pos[1] + cell.colspan, pos[0],
						pos[0] + cell.rowspan,
						 Gtk::AttachOptions(Gtk::FILL | (g[4] == -1 ? Gtk::EXPAND : 0)),
						 Gtk::AttachOptions(Gtk::FILL | (g[5] == -1 ? Gtk::EXPAND : 0)));
				impl->getWidget()->set_size_request(g[4], g[5]);
				ctrl->propertyChange(offsetof(Table,geometry));
				printf(
						"GTKTableImpl::processChild: row: %i col: %i rs: %i cs: %i allocation: %i %i rows: %i cols: %i\n",
						pos[0], pos[1], cell.rowspan, cell.colspan, alloc.get_width(),
						alloc.get_height(), t.property_n_rows().get_value(),
						t.property_n_columns().get_value());
				//tmp->getWidget()->show();
				t.show_all();
			}
			virtual void initialize() {
				GTKControlImpl::initialize();
				Gtk::Table& t(*(Gtk::Table*) getWidget());
				Table* ctrl = static_cast<Table*>(control);
				t.property_n_rows().set_value(ctrl->items.size());
				t.property_n_columns().set_value(ctrl->cols);
				printf("table rows: %i cols: %i\n", (int) ctrl->items.size(), ctrl->cols);
				//Gtk::Table& t(*(Gtk::Table*) getWidget());
				i_event =
						control->ImplEvent +=
								[this](const ImplEventData& ed) {
									Gtk::Table& t(*(Gtk::Table*) getWidget());
									Table* ctrl = static_cast<Table*>(control);
									switch(ed.type) {
										case ImplEventData::t_addRemoveChild:
										{
											if(ed.addRemoveChild.isRemove) {
												Control* c = dynamic_cast<Control*>(ed.addRemoveChild.element);
												auto tmp=dynamic_cast<GTKImplementation*>(c->implementations[this->viewport]);
												t.remove(*(tmp->getWidget()));
												if(tmp!=NULL) tmp->release();
											} else {
												processChild(dynamic_cast<Control*>(ed.addRemoveChild.element));
											}
										}
										break;
										case ImplEventData::t_propertyChange:
										switch(ed.propertyChange.offset)
										{
											case offsetof(Table,cols):
											{
												t.property_n_rows().set_value(ctrl->items.size());
												t.property_n_columns().set_value(ctrl->cols);
											}
											break;
										}
										break;
										default:
										break;
									}

								};
				Element* e;
				for (e = control->firstChild; e != NULL; e = e->next) {
					processChild(static_cast<Control*>(e));
				}
			}
			GTKTableImpl(Control* control, Viewport* viewport) :
					GTKControlImpl(control, viewport) {

			}
			~GTKTableImpl() {
				control->ImplEvent -= i_event;
			}
		};

		void Window::setTitle(string s) {
			title = s;
			propertyChange(offsetof(Window,title));
		}
		string Window::getTitle() {
			return title;
		}
		Button::Button() {
			INIT_CONTROL_EVENT(Button, click);
		}
		void Button::setText(string s) {
			text = s;
			propertyChange(offsetof(Button,text));
		}
		string Button::getText() {
			return text;
		}

		Table::Cell::Cell() :
				c(NULL), colspan(0), rowspan(0) {

		}
		Table::Cell::~Cell() {
			//if (c != NULL) c->release();
		}
		void Table_do_removeChild(Table* t, Control* c) {
			t->removeChild(c);
			t->itemsMap.erase(c);
		}
		array<int, 2> Table::getSize() {
			return { {(int)items.size(),cols}};
		}
		void Table::setSize(array<int, 2> s) {
			if ((int) items.size() > s[0]) {
				for (int i = s[0]; i < (int) items.size(); i++)
					for (int ii = 0; ii < (int) items[i].size(); ii++)
						if (items[i][ii].c != NULL) Table_do_removeChild(this, items[i][ii].c);
			}
			items.resize(s[0]);
			for (int i = 0; i < (int) items.size(); i++) {
				if ((int) items[i].size() > s[1]) {
					for (int ii = s[1]; ii < (int) items[i].size(); ii++)
						if (items[i][ii].c != NULL) Table_do_removeChild(this, items[i][ii].c);
				}
				items[i].resize(s[1]);
			}
			cols = s[1];
			propertyChange(offsetof(Table,cols));
		}
		Control* Table::getChild(int y, int x) {
			return items[y][x].c;
		}
		void Table::setChild(int y, int x, Control* c, int rowspan, int colspan) {
			if (items[y][x].c != NULL) {
				for (int i = y; i < (y + rowspan); i++)
					for (int ii = x; ii < (x + colspan); ii++)
						items[i][ii].c = NULL;
				Table_do_removeChild(this, items[y][x].c);
			}
			items[y][x].c = c;
			items[y][x].colspan = colspan;
			items[y][x].rowspan = rowspan;
			if (c != NULL) {
				itemsMap.insert( { c, { { y, x } } });
				addChild(c, NULL);
			}
		}
		Table::Table() :
				cols(0) {

		}
	}
#define IMPLEMENTATION_CTOR(x) [](Control* c,Viewport* v){return new x(c, v);}
	void registerBaseImpl() {
		registerImplementation(typeid(Controls::Container), typeid(Viewports::GTKViewport),
				IMPLEMENTATION_CTOR(Controls::GTKContainerImpl));
		registerImplementation(typeid(Controls::Window), typeid(Viewports::GTKViewport),
				IMPLEMENTATION_CTOR(Controls::GTKWindowImpl));
		registerImplementation(typeid(Controls::Button), typeid(Viewports::GTKViewport),
				IMPLEMENTATION_CTOR(Controls::GTKButtonImpl));
		registerImplementation(typeid(Controls::Table), typeid(Viewports::GTKViewport),
				IMPLEMENTATION_CTOR(Controls::GTKTableImpl));
	}

}


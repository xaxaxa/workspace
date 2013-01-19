/*
 * control.C
 *
 *  Created on: Dec 8, 2012
 *      Author: xaxaxa
 */
#include "include/control.H"
#include "include/gtkviewport.H"
#include "include/wtviewport.H"
#include <gtkmm.h>
#include <Wt/WPushButton>
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
			control(control), viewport(viewport), nodetach(false) {
		printf("Implementation::Implementation(%p, %p, %p)\n", this, control, viewport);
		if (unlikely(!control->implementations.add(viewport, this))) {
			if (control->implementations[viewport] != NULL) nodetach = true;
			else throw runtime_error("exceeded the maximum # of implementations per control");
		}

		eventHandlerID = control->ImplEvent += [this](const ImplEventData& ed) {
			processEvent(ed);
		};
	}
	Implementation::~Implementation() {
		//clearChildren();
		control->ImplEvent -= eventHandlerID;
		if (!nodetach) control->implementations.remove(viewport);
	}
	void Implementation::propertyChange(int offset) {

	}
	void Implementation::methodCall(const char* name) {

	}
	void Implementation::enableDisableEvent(int offset, bool disable) {

	}
	void Implementation::addRemoveChild(Element* element, bool remove) {

	}

	void Implementation::processEvent(const ImplEventData& data) {
		switch (data.type) {
			case ImplEventData::t_addRemoveChild:
				addRemoveChild(data.addRemoveChild.element, data.addRemoveChild.isRemove);
				break;
			case ImplEventData::t_enableEvent:
				enableDisableEvent(data.enableEvent.offset, false);
				break;
			case ImplEventData::t_disableEvent:
				enableDisableEvent(data.disableEvent.offset, true);
				break;
			case ImplEventData::t_methodCall:
				methodCall(data.methodCall.name);
				break;
			case ImplEventData::t_propertyChange:
				propertyChange(data.propertyChange.offset);
				break;
		}
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
		printf("Control::implement(%p, %p)\n", this, vp);
		Implementation* impl = (*func)(this, vp);
		printf("Control::implement 2(%p, %p)\n", this, vp);
		RGC::Ref<Implementation> ref = impl;
		impl->release();
		return ref;
	}
	RGC::Ref<Implementation> Control::implement(Viewport* vp, const ImplementationCtor& implCtor) {
		auto tmp = implementations[vp];
		if (tmp != NULL) return tmp;
		Implementation* impl = implCtor(this, vp);
		RGC::Ref<Implementation> ref = impl;
		impl->release();
		return ref;
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
			virtual void propertyChange(int offset) override {
				Viewports::GTKImplementation::propertyChange(offset);
				switch (offset) {
					case offsetof(Control,visible):
					{
						if(this->control->getVisible()) this->getWidget()->show();
						else this->getWidget()->hide();
					}
					break;
					case offsetof(Control,geometry):
					{
						Control* ctrl = dynamic_cast<Control*>(this->control->parent);
						if (ctrl == NULL) {
							this->getWidget()->set_size_request(this->control->geometry[4],this->control->geometry[5]);
							return;
						}
						auto tmp = ctrl->implementations[this->viewport];
						if (tmp == NULL) return;
						GTKControlImpl* impl = dynamic_cast<GTKControlImpl*>(tmp);
						impl->childGeometryChange(this->control);
					}
					break;
				}
			}
			GTKControlImpl(Control* control, Viewport* viewport, Gtk::Widget* widg) :
			Viewports::GTKImplementation(control, viewport, widg), last_width(-1),
			last_height(-1) {
				if (this->control->parent == NULL) {
					this->getWidget()->set_size_request(control->geometry[4], control->geometry[5]);
				}
				auto wid = this->getWidget();
				auto alloc = wid->get_allocation();
				last_width = alloc.get_width();
				last_height = alloc.get_height();
				printf("initial size of %s: %i %i\n", typeid(*control).name(), alloc.get_width(), alloc.get_height());

				c_size_allocate = wid->signal_size_allocate().connect([this](Gtk::Allocation& a) {
							if(a.get_width()==last_width && a.get_height()==last_height) return;
							//printf("signal_size_allocate: %s %i %i; lastSize: %i %i\n", typeid(*this->control).name(), a.get_width(), a.get_height(), last_width, last_height);
							allocationChange(a.get_width(), a.get_height());
						});
				if (control->visible) wid->show();
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
				this->getWidget()->hide();
			}
		};
		class WTControlImpl: public Viewports::WTImplementation
		{
		public:
			virtual void parentChange(Control* oldparent) {
				Viewports::WTViewport* vp = static_cast<Viewports::WTViewport*>(viewport);
				if (control->parent == NULL) {
					vp->app->addChild(getWidget());
				}
			}
			virtual void initialize() {

			}
			virtual void addRemoveChild(Element* element, bool remove) override {
				Viewports::WTImplementation::addRemoveChild(element, remove);
				if (remove) {
					auto ctrl = dynamic_cast<Control*>(element);
					if (!ctrl) return;
					auto impl = ctrl->implementations[this->viewport];
					if (!impl) return;
					auto impl1 = dynamic_cast<WTControlImpl*>(impl);
					impl1->parentChange(this->control);
				}
			}
			virtual void propertyChange(int offset) override {
				Viewports::WTImplementation::propertyChange(offset);
				switch (offset) {
					case offsetof(Control, visible):
					{
						if (this->control->visible) this->getWidget()->show();
						else getWidget()->hide();
					}
				}
			}

			WTControlImpl(Control* control, Viewport* viewport, Wt::WWidget* widg) :
			Viewports::WTImplementation(control, viewport, widg) {
				Viewports::WTViewport* vp = static_cast<Viewports::WTViewport*>(viewport);
				if (control->visible) getWidget()->show();
				else getWidget()->hide();
				printf("initializing wtcontrol; parent=%p visible=%i\n", control->parent,
						(int) control->visible);
				if (control->parent == NULL) {
					vp->app->addChild(getWidget());
					printf("adding child to root wt widget\n");
				}
			}
			~WTControlImpl() {
			}
		};
		class WTButtonImpl: public WTControlImpl
		{
		public:
			WTButtonImpl(Control* control, Viewport* viewport, Wt::WWidget* w = NULL) :
					WTControlImpl(control, viewport, (w == NULL ? new Wt::WPushButton() : w)) {

			}
			~WTButtonImpl() {

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

				//printf("repositioned %s: x: %i y: %i w: %i h: %i parentW: %i parentH: %i\n",
				//		typeid(*c).name(), x, y, W, H, width, height);

				f.move(*wid, x, y);
			}
			virtual void childAllocationChange(Control* c) override {
				//printf("childAllocationChange()\n");
				GTKControlImpl::childAllocationChange(c);
				//auto pos = c->getPosition();
				Gtk::Fixed& f(*(Gtk::Fixed*) getWidget());
				//GTKControlImpl* impl = static_cast<GTKControlImpl*>(c->implementations[viewport]);
				auto alloc = f.get_allocation();
				int width = alloc.get_width();
				int height = alloc.get_height();
				repositionChild(static_cast<Control*>(c), width, height);
			}
			virtual void childGeometryChange(Control* c) override {
				GTKControlImpl::childGeometryChange(c);
				Gtk::Fixed& f(*(Gtk::Fixed*) getWidget());
				auto alloc = f.get_allocation();
				int width = alloc.get_width();
				int height = alloc.get_height();
				repositionChild(static_cast<Control*>(c), width, height);
			}
			virtual void allocationChange(int nw, int nh) override {
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
			virtual void addRemoveChild(Element* element, bool remove) override {
				printf("addRemoveChild: element: %s\n", typeid(*element).name());
				GTKControlImpl::addRemoveChild(element, remove);
				Gtk::Fixed& f(*(Gtk::Fixed*) getWidget());
				if (remove) {
					Control* c = dynamic_cast<Control*>(element);
					auto tmp = dynamic_cast<GTKImplementation*>(c->implementations[this->viewport]);
					f.remove(*(tmp->getWidget()));
					if (tmp != NULL) tmp->release();
				} else {
					processChild(element);
				}
			}
			GTKContainerImpl(Control* control, Viewport* viewport, Gtk::Widget* widg = NULL) :
					GTKControlImpl(control, viewport, (widg == NULL ? new GTKFixed1() : widg)) {
				Gtk::Fixed& f(*(Gtk::Fixed*) getWidget());
				auto alloc = f.get_allocation();
				int width = alloc.get_width();
				int height = alloc.get_height();
				Element* e;
				for (e = control->firstChild; e != NULL; e = e->next) {
					processChild(e, false);
					repositionChild(static_cast<Control*>(e), width, height);
				}
			}
			~GTKContainerImpl() {
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
			GTKContainerImpl cont;

			virtual void childAllocationChange(Control* c) override {
				cont.childAllocationChange(c);
			}
			virtual void allocationChange(int nw, int nh) override {
				GTKControlImpl::allocationChange(nw, nh);
			}
			virtual void propertyChange(int offset) override {
				GTKControlImpl::propertyChange(offset);
				Gtk::Window& wnd(*(Gtk::Window*) getWidget());
				Window* ctrl = static_cast<Window*>(this->control);
				if (offset == offsetof(Window, title)) {
					wnd.set_title(ctrl->getTitle());
				}
			}
			GTKWindowImpl(Control* control, Viewport* viewport, Gtk::Widget* widg = NULL) :
			GTKControlImpl(control, viewport, (widg == NULL ? new Gtk::Window() : widg)),
			cont((Container*) control, viewport) {
				((Gtk::Window*) widget)->property_allow_shrink().set_value(true);
				Gtk::Window& wnd(*(Gtk::Window*) getWidget());
				Window* ctrl = static_cast<Window*>(control);
				wnd.set_title(ctrl->getTitle());
				wnd.add(*(cont.getWidget()));
			}
			virtual ~GTKWindowImpl() {
			}
		};
		class GTKButtonImpl: public GTKControlImpl
		{
		public:
			sigc::connection c_clicked;
			virtual void propertyChange(int offset) override {
				GTKControlImpl::propertyChange(offset);
				Gtk::Button& b(*(Gtk::Button*) getWidget());
				Button* ctrl = static_cast<Button*>(this->control);
				if (offset == offsetof(Button,visible)) {
					if(this->control->visible)b.show();
					else b.hide();
				} else if(offset==offsetof(Button,text)) {
					b.set_label(ctrl->getText());
				}
			}
			GTKButtonImpl(Control* control, Viewport* viewport, Gtk::Widget* widg = NULL) :
			GTKControlImpl(control, viewport, widg == NULL ? new GTKButton1() : widg) {
				Gtk::Button& b(*(Gtk::Button*) getWidget());
				Button* ctrl = static_cast<Button*>(control);
				b.set_label(ctrl->getText());
				c_clicked = b.signal_clicked().connect([this]() {
							Button* ctrl = static_cast<Button*>(this->control);
							ctrl->click();
						});
			}
			~GTKButtonImpl() {
				c_clicked.disconnect();
			}
		};
		class GTKTableImpl: public GTKControlImpl
		{
		public:
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
			virtual void addRemoveChild(Element* element, bool remove) override {
				GTKControlImpl::addRemoveChild(element, remove);
				Gtk::Table& t(*(Gtk::Table*) getWidget());
				if (remove) {
					Control* c = dynamic_cast<Control*>(element);
					auto tmp = dynamic_cast<GTKImplementation*>(c->implementations[this->viewport]);
					t.remove(*(tmp->getWidget()));
					if (tmp != NULL) tmp->release();
				} else {
					processChild(dynamic_cast<Control*>(element));
				}
			}
			virtual void propertyChange(int offset) override {
				GTKControlImpl::propertyChange(offset);
				Table* ctrl = static_cast<Table*>(this->control);
				Gtk::Table& t(*(Gtk::Table*) getWidget());
				switch (offset) {
					case offsetof(Table,cols):
					{
						t.property_n_rows().set_value(ctrl->items.size());
						t.property_n_columns().set_value(ctrl->cols);
					}
					break;
				}
			}
			GTKTableImpl(Control* control, Viewport* viewport, Gtk::Widget* widg = NULL) :
			GTKControlImpl(control, viewport, widg == NULL ? new Gtk::Table() : widg) {
				Gtk::Table& t(*(Gtk::Table*) getWidget());
				Table* ctrl = static_cast<Table*>(control);
				t.property_n_rows().set_value(ctrl->items.size());
				t.property_n_columns().set_value(ctrl->cols);
				printf("table rows: %i cols: %i\n", (int) ctrl->items.size(), ctrl->cols);
				//Gtk::Table& t(*(Gtk::Table*) getWidget());
				Element* e;
				for (e = control->firstChild; e != NULL; e = e->next) {
					processChild(static_cast<Control*>(e));
				}
			}
			~GTKTableImpl() {
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
		registerImplementation(typeid(Controls::Button), typeid(Viewports::WTViewport),
				IMPLEMENTATION_CTOR(Controls::WTButtonImpl));
	}

}


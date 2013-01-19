#ifndef CONTROL_H
#define CONTROL_H
#include <rgc.H>
#include <list>
#include <functional>
#include <string>
#include <vector>
#include <tuple>
#include <stddef.h>
#include <sys/types.h>
#include <array>
#include <map>
//maximum # of simultaneous implementations a control may have
//note that implementing a control automatically implements their children as well
//increasing this will use more ram
#ifndef MAX_IMPLEMENTATIONS_PER_CONTROL
#define MAX_IMPLEMENTATIONS_PER_CONTROL 5
#endif

#define INIT_CONTROL_EVENT(t, field) field.init(offsetof(t, field))

#ifndef likely
#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)
#endif
using namespace std;
/*
 * USAGE: controls may be extended by creating classes derived from it,
 * but you must register your implementation for that specific child class;
 * otherwise your control is non-implementable even if its base class has
 * an implementation registered; MULTIPLE/VIRTUAL INHERITANCE MUST NOT BE USED
 * ANYWHERE IN THE INHERITANCE TREE OF THE CONTROLS; otherwise you will get segfault
 *
 * GenericGUI basic concepts:
 * * DOM - document object model; the application interacts directly with the DOM
 * 		 - always exists in the same process as the application
 * * Control - an abstract view of widget
 * * Element - an element in the UI; not necessarily visible
 * * Viewport - a physical view of the controls
 * 			  - abstracted away from the applications
 * 			  - for example, there may be a GTK viewport, a web viewport, etc
 * * Implementation - an implementation of a certain control for a certain viewport
 *
 * * all controls are responsible for rendering its child controls
 *
 *
 * */
namespace GenericGUI
{
	template<class ... A>
	class Event
	{
	public:
		struct Handler
		{
			function<void(A...)> f;
			int id;
		};
		vector<Handler> h;
		int max_id;
		Event() :
				max_id(1) {
		}
		int operator+=(const function<void(A...)>& func);
		void operator-=(int id);
		void operator()(A ... a...);
	};

		//NOT polymorphic
	template<class ... A>
	class ManagedEvent: public Event<A...>
	{
	public:
		Event<bool> callback;
		int operator+=(const function<void(A...)>& func);
		void operator-=(int id);
	};
	template<class ... A> int Event<A...>::operator+=(const function<void(A...)>& func) {
		size_t tmp = h.size();
		h.resize(tmp + 1);
		h[tmp].f = func;
		h[tmp].id = ++max_id;
		return (int) h[tmp].id;
	}
	template<class ... A> void Event<A...>::operator-=(int id) {
		for (size_t i = 0; i < h.size(); i++) {
			if (h[i].id == id) {
				h.erase(h.begin() + i);
				return;
			}
		}
	}
	template<class ... A> void Event<A...>::operator()(A ... a...) {
		for(size_t i=0;i<h.size();i++) {
			//h[i].f(std::forward<A>(a)...);
			h[i].f(a...);
		}
	}
	template<class ... A> int ManagedEvent<A...>::operator+=(const function<void(A...)>& func) {
		bool b = this->h.size() != 0;
		int tmp = Event<A...>::operator +=(func);
		if (!b) callback(true);
		return tmp;
	}
	template<class ... A> void ManagedEvent<A...>::operator-=(int id) {
		Event<A...>::operator -=(id);
		if (this->h.size() == 0) callback(false);
	}
	class Element;
	// messages from control to implementation
	struct ImplEventData
	{
		enum Types
		{
			none = 0, t_propertyChange, t_methodCall, t_enableEvent, //when there is at least one listener on the event
			t_disableEvent, //when all listeners have disappeared
			t_addRemoveChild
		} type;
		union
		{
			struct
			{
				//const char* name;
				int offset; //offsetof(field)
			} propertyChange;
			struct
			{
				const char* name;
				//int offset;
			} methodCall;
			struct
			{
				//const char* name;
				int offset;
			} enableEvent, disableEvent;
			struct
			{
				bool isRemove;
				Element* element;
			} addRemoveChild;
		};
		void* extraData;
		ImplEventData() :
				type(none), extraData(NULL) {
		}
		ImplEventData(Types t) :
				type(t), extraData(NULL) {

		}
	};
	//messages from implementation to control
	struct ImplMessageData
	{
		enum Types
		{
			none = 0, t_propertyChange, t_raiseEvent
		} type;
		union
		{
			struct
			{
				int offset;
			} propertyChange;
			struct
			{
				int offset;
			} raiseEvent;
		};
	};
	class Object: public RGC::Object
	{
	};
	class Viewport: public Object
	{
		virtual void run()=0; //run event loop
	};
	class Element: public Object
	{
	public:
		//list<RGC::Ref<Element> > children;
		Event<const ImplEventData&> ImplEvent;

		void propertyChange(int offset);
		Element* firstChild;
		Element* lastChild;
		Element* prev;
		Element* next;
		Element* parent;

		//Event<const char*> propertyChange;
		//bool isremove, Element* e
		//if e is NULL and isremove is true, clearChildren() was called
		//Event<bool, Element*> addRemoveChild;
		//adding an element to the linked list will increment its reference count
		virtual void addChild(Element* e, Element* before = NULL); //append
		virtual void removeChild(Element* e);
		virtual void clearChildren();
		Element();
		virtual ~Element();
	};
	class Implementation;
	class ImplMap
	{
	public:
		struct Item
		{
			Viewport* vp;
			Implementation* impl;
		};
		Item items[MAX_IMPLEMENTATIONS_PER_CONTROL];
		ImplMap();
		bool add(Viewport* vp, Implementation* impl);
		Implementation* find(Viewport* vp);
		inline Implementation* operator[](Viewport* vp) {
			return find(vp);
		}
		void remove(Viewport* vp);
	};
	class Control;
	class Implementation: public Object
	{
	public:

		/*Implementation* parent;
		 Implementation* firstChild;
		 Implementation* lastChild;
		 Implementation* prev;
		 Implementation* next;
		 void addChild(Implementation* impl);
		 void removeChild(Implementation* impl);
		 void clearChildren();*/

		//these 4 functions below will be called by processMessage()
		virtual void propertyChange(int offset);
		virtual void methodCall(const char* name);
		virtual void enableDisableEvent(int offset, bool disable);
		virtual void addRemoveChild(Element* element, bool remove);

		virtual void processEvent(const ImplEventData& data);

		Control* control;
		Viewport* viewport;
		int eventHandlerID;
		bool nodetach;
		Implementation(Control* control, Viewport* viewport);
		virtual ~Implementation();
	};
	typedef function<Implementation*(Control*, Viewport*)> ImplementationCtor;
	class Control: public Element
	{
	public:
		ImplMap implementations;

		//Implementation* impl;
		//if one or more of (T, R, B, L) is negative, it means that that side is not
		//fixed relative to the side of its parent
		//			  T,   R,	B,	 L,	  W,   H
		//typedef tuple<int, int, int, int, int, int> Geometry;
		//			  0		1	2	  3		4	5
		typedef array<int, 6> Geometry;
		Geometry geometry;
		bool visible;

		void setVisible(bool b);
		bool getVisible();
		void setGeometry(const Geometry& g);
		void setPosition(const tuple<int, int>& pos);
		Geometry getGeometry();
		tuple<int, int> getPosition();
		//to "unimplement", or hide a control, just release() all references
		//to the Implementation object
		virtual RGC::Ref<Implementation> implement(Viewport* vp);
		virtual RGC::Ref<Implementation> implement(Viewport* vp,
				const ImplementationCtor& implCtor);
		Control();
	};
	//NOT polymorphic
	template<class ... A>
	class ControlEvent: public Event<A...>
	{
	public:
		int offset;
		Control* getControl() {
			int8_t* i8 = (int8_t*) this;
			i8 -= offset;
			return (Control*) i8;
		}
		int operator+=(const function<void(A...)>& func) {
			bool b = this->h.size() != 0;
			int tmp = Event<A...>::operator +=(func);
			if (!b) {
				ImplEventData ed;
				ed.type = ImplEventData::t_enableEvent;
				ed.enableEvent.offset = offset;
				getControl()->ImplEvent(ed);
			}
			return tmp;
		}
		void operator-=(int id) {
			Event<A...>::operator -=(id);
			if (this->h.size() == 0) {
				ImplEventData ed;
				ed.type = ImplEventData::t_disableEvent;
				ed.disableEvent.offset = offset;
				getControl()->ImplEvent(ed);
			}
		}
		void init(int offset) {
			this->offset = offset;
		}
	};

	ImplementationCtor* findImplementation(const type_info& ctrl, const type_info& viewport);
	void registerImplementation(const type_info& ctrl, const type_info& viewport,
			const ImplementationCtor& ctor);
	namespace Viewports
	{

	}
	namespace Controls
	{
		class Container: public Control
		{
			//Event<Control*, int, int> childPositionChange;
			//virtual void setChildPosition(Control* c, int x, int y);
		};
		class Window: public Container
		{
		public:
			string title;
			void setTitle(string s);
			string getTitle();
		};
		class Button: public Control
		{
		public:
			string text;
			Button();
			//ManagedEvent<> click;
			ControlEvent<> click;
			void setText(string s);
			string getText();
		};
		class Table: public Container
		{
		public:
			struct Cell
			{
				Control* c;
				int colspan, rowspan;
				Cell();
				~Cell();
			};
			vector<vector<Cell> > items; //rows, cols
			map<Control*, array<int, 2> > itemsMap;
			int cols;
			array<int, 2> getSize(); //rows, cols
			void setSize(array<int, 2> s);
			Control* getChild(int y, int x);
			void setChild(int y, int x, Control* c, int rowspan = 1, int colspan = 1);
			Table();
		};
	}
}
#endif


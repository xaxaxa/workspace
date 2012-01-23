/*
 * untitled.cxx
 * 
 * Copyright 2012  <xaxaxa@xaxaxa-mac>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */
#include <cplib/cplib.hpp>
#include <gtkmm.h>
#include <gdk/gdk.h>
#include <iostream>
#include <math.h>
using namespace std;
using namespace Gdk;
using namespace Gtk;
//using namespace ::Cairo;
namespace xaxaxa
{
	class EQControl: public Gtk::DrawingArea
	{
	public:
		double* data;
		UInt datalen;
		Int last_i;
		double last_v;
		bool d;
		DELEGATE(void,ChangeDelegate,UInt,UInt);
		DELEGATE(void,MouseDelegate,double);
		EVENT(ChangeDelegate) Change;
		EVENT(MouseDelegate) MouseMove;
		inline double GetPoint(double pos)
		{
			if(pos>=datalen)return data[datalen-1];
			double p1, p2;
			p1=floor(pos);
			p2=p1+1;
			if(p2>=datalen)return data[(UInt)p1];
			return data[(UInt)p1]*(p2-pos)+data[(UInt)p2]*(pos-p1);
		}
		virtual bool on_draw(const ::Cairo::RefPtr< ::Cairo::Context>& gc)
		{
			Gtk::DrawingArea::on_draw(gc);
			
			UInt w=get_allocation().get_width();
			UInt h=get_allocation().get_height();
			double x1,y1,x2,y2;
			gc->get_clip_extents(x1,y1,x2,y2);
			
			for(UInt x=x1;x<=x2;x++)
			{
				gc->move_to(x,h-1);
				double y=h-round(GetPoint((double)x*datalen/w)*h);
				if(y>h)y=h;
				else if(y<0)y=0;
				gc->line_to(x,y);
				gc->set_source_rgb(0,0,0);
				gc->stroke();
				
				gc->move_to(x,y);
				gc->line_to(x,0);
				gc->set_source_rgb(1,1,1);
				gc->stroke();
				//gc->line_to(x,123);
				//gc->stroke();
			}
			//gc->rectangle(5,5,100,100);
			
		//ret:
			return true;
		}
		virtual bool on_motion_notify_event(GdkEventMotion* event)
		{
			if(event->y<0)event->y=0;
			if(event->x<0)event->x=0;
			UInt w=get_allocation().get_width();
			RAISEEVENT(MouseMove,(double)event->x*datalen/w);
			if(!d)return true;
			
			//cout << "on_motion_notify_event" << endl;
			
			UInt h=get_allocation().get_height();
			
			double v=(double)(h-event->y)/h;
			UInt i1,i2;
			i1=round((double)event->x*datalen/w);
			i2=round((double)(event->x+1)*datalen/w);
			if(i1>=datalen)i1=datalen-1;
			if(i2>datalen)i2=datalen;
			Int tmp_i=last_i;
			double tmp_v=last_v;
			last_v=v;
			last_i=i1+((i2-i1)/2);
			if(i2>datalen)i2=datalen;
			if(i2<=i1)i2=i1+1;
			if(tmp_i>=0 && (tmp_i<(Int)i1 || tmp_i>(Int)i2))
			{
				i1=tmp_i;
				i2=last_i;
				if(i2<i1)
				{	//swap
					UInt tmp=i1;
					i1=i2;
					i2=tmp;
					double tmp_d=tmp_v;
					tmp_v=v;
					v=tmp_d;
				}
				for(UInt i=i1;i<i2;i++)
					data[i]=v*(i-i1)/(i2-i1)+tmp_v*(i2-i)/(i2-i1);
			}
			else for(UInt i=i1;i<i2;i++)
				data[i]=v;
			UInt x1=i1*w/datalen;
			UInt x2=i2*w/datalen;
			if(x2<=x1)x2=x1+2;
			//cout << x2-x1 << endl;
			queue_draw_area(x1-(x2-x1),0,(x2-x1)*2,h);
			RAISEEVENT(Change,i1,i2);
			return true;
		}
		virtual bool on_button_press_event(GdkEventButton* event)
		{
			d=true;
			last_i=-1;
			return true;
		}
		virtual bool on_button_release_event(GdkEventButton* event)
		{
			d=false;
			return true;
		}
		EQControl(UInt datalen): Gtk::DrawingArea(), datalen(datalen), last_i(-1), last_v(-1.0), d(false)
		{
			//signal_draw().connect(sigc::mem_fun(this,&EQControl::onDraw));
			//signal_motion_notify_event().connect(sigc::mem_fun(this,&EQControl::on_motion_notify_event));
			data=new double[datalen];
			for(UInt i=0;i<datalen;i++)
				data[i] = 0.5;
			set_app_paintable(true);
			set_double_buffered(false);
			set_redraw_on_allocate(true);
			set_events(get_events()|POINTER_MOTION_MASK|BUTTON_MOTION_MASK|BUTTON_PRESS_MASK|BUTTON_RELEASE_MASK);
		}
	};
}


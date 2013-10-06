/*
 * histcontrol.C
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
#include <gtkmm/drawingarea.h>
#include <gdk/gdk.h>
#include <iostream>
#include <math.h>
using namespace std;
using namespace Gdk;
using namespace Gtk;
//using namespace ::Cairo;
namespace xaxaxa
{
	class HistoryControl: public Gtk::DrawingArea
	{
	public:
		/*data format:
		[frame1, frame2, ... , frameN]
		frame:
		[sample1, sample2, ..., sampleN]
		*/
		double* data;
		int frameLen;
		int frames;
		int curFrame;
		::Cairo::RefPtr< ::Cairo::ImageSurface > img;
		::Cairo::RefPtr< ::Cairo::SurfacePattern > sp;
		
		inline double getPoint(double* frame, double pos, int frameLen)
		{
			if(pos>=(frameLen-1))return frame[frameLen-1];
			double p1, p2;
			p1=floor(pos);
			p2=p1+1;
			return double(frame[(int)p1])*(p2-pos)+double(data[(int)p2])*(pos-p1);
		}
		double* getFrame(int i) {
			return data+(i*frameLen);
		}
		//data is in FORMAT_ARGB32 format
		void do_draw(void* imgdata, int w, int h)
		{
			unsigned int* img=(unsigned int*)imgdata;
			int s=w-frames;
			if(s<0)s=0;
			for(int i=0;i<s;i++) {
				for(int y=0;y<h;y++) img[y*w+i]=255<<24 | 255;
			}
			for(int i=s;i<w;i++) {
				int fIndex=curFrame-(w-1-i);
				if(fIndex<0)fIndex+=frames;
				else fIndex=fIndex%frames;
				double* frame=getFrame(fIndex);
				double pointsPerPx=double(frameLen)/h;
				for(int y=0;y<h;y++) {
					double v;
					if(pointsPerPx<2)v=getPoint(frame,(h-y)*frameLen/h,frameLen);
					else {
						v=0;
						int start=(h-y)*frameLen/h;
						int end=(int)ceil(double(h-y+1)*frameLen/h);
						for(int i=start;i<=end;i++) if(frame[i]>v)v=frame[i];
					}
					if(v>1)v=1;
					int V=(int)(unsigned char)(v*255);
					img[y*w+i]=255<<24 | V<<16 | V<<8 | V;
				}
			}
		}
		void do_draw(::Cairo::RefPtr< ::Cairo::Context> ctx) {
			int w=(int)get_allocation().get_width();
			int h=(int)get_allocation().get_height();
			if(!img || img->get_width()!=w || img->get_height()!=h) {
				img=::Cairo::ImageSurface::create(::Cairo::FORMAT_ARGB32,w,h);
				sp=::Cairo::SurfacePattern::create(img);
			}
			do_draw(img->get_data(),w,h);
			img->mark_dirty();
			ctx->set_source(sp);
			ctx->paint();
		}
		void do_draw(GdkEventExpose* evt=NULL)
		{
			Glib::RefPtr<Gdk::Window> window = get_window();
			if(window)
			{
				::Cairo::RefPtr< ::Cairo::Context> ctx = window->create_cairo_context();
				if(evt)
				{
					ctx->rectangle(evt->area.x, evt->area.y, evt->area.width, evt->area.height);
					ctx->clip();
				}
				do_draw(ctx);
			}
		}
		virtual bool on_expose_event(GdkEventExpose* evt)
		{
			do_draw(evt);
			return true;
		}
		void allocBuffer(int dataLen) {
			data=new double[dataLen];
			for(int i=0;i<dataLen;i++)
				data[i] = 0;
		}
		void addFrames(double* d, int framecnt) {
			if(framecnt>frames) {
				d+=(framecnt-frames);
				framecnt=frames;
			}
			int cnt=framecnt<(frames-curFrame)?framecnt:(frames-curFrame);
			memcpy(data+curFrame*frameLen,d,cnt*frameLen*sizeof(double));
			if(cnt<framecnt)
				memcpy(data,d+cnt*frameLen,(framecnt-cnt)*frameLen*sizeof(double));
			curFrame=(curFrame+framecnt)%frames;
		}
		HistoryControl(int frames, int frameLen): frames(frames), frameLen(frameLen), curFrame(0)
		{
			allocBuffer(frames*frameLen);
			set_app_paintable(true);
			set_double_buffered(true);
			set_redraw_on_allocate(true);
			set_events(get_events()|POINTER_MOTION_MASK|BUTTON_MOTION_MASK|BUTTON_PRESS_MASK|BUTTON_RELEASE_MASK);
		}
		~HistoryControl() {
			delete[] data;
		}
		void resize(int frames, int frameLen) {
			this->frames=frames;
			this->frameLen=frameLen;
			delete[] data;
			allocBuffer(frames*frameLen);
		}
	};
}


// JScript File

function rspanel(e,wnd)
{
	this.type="rspanel";
	this.ready=false;
	if(typeof wnd.obj == "object")
	{
		window.obj[window.obj.length]=this;
		this.ind=window.obj.length-1;
	}
	else
	{
		window.obj=[this];
		this.ind=0;
	}
	this.e=e;
	this.wnd=wnd;
	this.rsf=[];
	this.rsfd=[];
	this.rsfb=[];
	this.mov=wnd.document.createElement("iframe");
	this.mov.style.cssText="border: #000000 1px solid; width: 13px; position: absolute; height: 13px; z-index:5;padding:0px 0px 0px 0px;";
	this.mov.src="about:blank";
	this.mov.frameBorder="0";
	this.mov.scrolling="no";
	this.mov.style.backgroundImage="url(move.gif)";
	wnd.document.body.appendChild(this.mov);
	
	this.is_ie=window.is_ie;//(window.navigator.appName.toLowerCase().indexOf("microsoft")>-1);
	var i;
	for(i=0;i<8;i++)
	{
		var f=wnd.document.createElement("iframe");
		this.rsf[i]=f;
		f.style.cssText="border-right: #000000 1px solid; border-top: #000000 1px solid; border-left: #000000 1px solid; width: 6px; border-bottom: #000000 1px solid; position: absolute; height: 6px; z-index:5;";
		f.src="about:blank";
		f.frameBorder="0";
		f.scrolling="no";
		wnd.document.body.appendChild(f);
	}
	this.is_movable=function(e)
	{
		var tn;
		if(e.tagName)tn=e.tagName.toLowerCase();
		else return false;
		return ((!e.dock)||e.dock==DOCK_NONE||e.dock==DOCK_CENTERH||e.dock==DOCK_CENTERV)&&tn!="td"&&tn!="tr";
	};
	this.find_movable_element=function(e)
	{
		var e_old=e;
		while(e && !this.is_movable(e))
			e=e.parentNode;
		if(e) return e;
		else return e_old;
	};
	this.is_resizable=function(e)
	{
		var tn;
		if(e.tagName)tn=e.tagName.toLowerCase();
		else return false;
		return ((!e.dock)&&(e.dock!=DOCK_FILL))&&tn!="td"&&tn!="tr";
	};
	this.find_resizable_element=function(e)
	{
		var e_old=e;
		while(e && !this.is_resizable(e))
			e=e.parentNode;
		if(e) return e;
		else return e_old;
	};
	this.calc_points=function(e)
	{
		var th=this;
		if(e==null)return;
		var trtd=false;
		/*if(e.tagName.toLowerCase()=="tr")
		{
			e=e.parentNode.parentNode;
			trtd=true;
		}
		else if(e.tagName.toLowerCase()=="td")
		{
			e=e.parentNode.parentNode.parentNode;
			trtd=true;
		}
		else if(e.__td!=null)
		{
			e=e.__td.parentNode.parentNode.parentNode;
			trtd=true;
		}*/
		var e_old=e;
		e=th.find_movable_element(e);
		//move
		switch(e.dock)
		{
			case DOCK_FILL:
				th.mov.style.display="none";
				break;
			case DOCK_TOP:
				th.mov.style.display="none";
				break;
			case DOCK_BOTTOM:
				th.mov.style.display="none";
				break;
			case DOCK_LEFT:
				th.mov.style.display="none";
				break;
			case DOCK_RIGHT:
				th.mov.style.display="none";
				break;
			case DOCK_CENTER:
				th.mov.style.display="none";
				break;
			case DOCK_TOPLEFT:
				th.mov.style.display="none";
				break;
			case DOCK_TOPRIGHT:
				th.mov.style.display="none";
				break;
			case DOCK_BOTTOMLEFT:
				th.mov.style.display="none";
				break;
			case DOCK_BOTTOMRIGHT:
				th.mov.style.display="none";
				break;
			default:
				th.mov.style.display="";
		}
		
		e=th.find_resizable_element(e_old);
		//resize
		var i;
		switch(e.dock)
		{
			case DOCK_FILL:
				for(i=0;i<th.rsf.length;i++)
				{
					th.rsf[i].style.display="none";
				}
				break;
			case DOCK_TOP:
				for(i=0;i<th.rsf.length;i++)
				{
					th.rsf[i].style.display="none";
				}
				th.rsf[5].style.display="";
				break;
			case DOCK_BOTTOM:
				for(i=0;i<th.rsf.length;i++)
				{
					th.rsf[i].style.display="none";
				}
				th.rsf[1].style.display="";
				break;
			case DOCK_LEFT:
				for(i=0;i<th.rsf.length;i++)
				{
					th.rsf[i].style.display="none";
				}
				th.rsf[3].style.display="";
				break;
			case DOCK_RIGHT:
				for(i=0;i<th.rsf.length;i++)
				{
					th.rsf[i].style.display="none";
				}
				th.rsf[7].style.display="";
				break;
			case DOCK_CENTER:
				for(i=0;i<th.rsf.length;i++)
				{
					th.rsf[i].style.display="";
				}
				break;
			case DOCK_TOPLEFT:
				for(i=0;i<th.rsf.length;i++)
				{
					th.rsf[i].style.display="none";
				}
				th.rsf[3].style.display="";
				th.rsf[4].style.display="";
				th.rsf[5].style.display="";
				break;
			case DOCK_TOPRIGHT:
				for(i=0;i<th.rsf.length;i++)
				{
					th.rsf[i].style.display="none";
				}
				th.rsf[5].style.display="";
				th.rsf[6].style.display="";
				th.rsf[7].style.display="";
				break;
			case DOCK_BOTTOMLEFT:
				for(i=0;i<th.rsf.length;i++)
				{
					th.rsf[i].style.display="none";
				}
				th.rsf[1].style.display="";
				th.rsf[2].style.display="";
				th.rsf[3].style.display="";
				break;
			case DOCK_BOTTOMRIGHT:
				for(i=0;i<th.rsf.length;i++)
				{
					th.rsf[i].style.display="none";
				}
				th.rsf[7].style.display="";
				th.rsf[0].style.display="";
				th.rsf[1].style.display="";
				break;
			default:
				for(i=0;i<th.rsf.length;i++)
				{
					th.rsf[i].style.display="";
				}
		}
		
		if(e.offsetWidth<10)
		{
			th.rsf[1].style.display="none";
			th.rsf[5].style.display="none";
		}
		if(e.offsetHeight<10)
		{
			th.rsf[3].style.display="none";
			th.rsf[7].style.display="none";
		}
		if(trtd)
		{
			//var i;
			th.mov.style.display="";
			/*for(i=0;i<th.rsf.length;i++)
			{
				th.rsf[i].style.display="";
			}*/
		}
		th.rsf[0].style.marginTop=offsety+get_y(e,this.e)-th.rsf[0].offsetHeight+1;
		th.rsf[0].style.marginLeft=offsetx+get_x(e,this.e)-th.rsf[0].offsetWidth+1;

		th.rsf[1].style.marginTop=offsety+get_y(e,this.e)-th.rsf[1].offsetHeight+1;
		th.rsf[1].style.marginLeft=offsetx+get_x(e,this.e)+(e.offsetWidth/2)-(th.rsf[1].offsetWidth/2);

		th.rsf[2].style.marginTop=offsety+get_y(e,this.e)-th.rsf[2].offsetHeight+1;
		th.rsf[2].style.marginLeft=offsetx+get_x(e,this.e)+e.offsetWidth-1;

		th.rsf[3].style.marginTop=offsety+get_y(e,this.e)+(e.offsetHeight/2)-(th.rsf[3].offsetHeight/2);
		th.rsf[3].style.marginLeft=offsetx+get_x(e,this.e)+e.offsetWidth-1;

		th.rsf[4].style.marginTop=offsety+get_y(e,this.e)+e.offsetHeight-1;
		th.rsf[4].style.marginLeft=offsetx+get_x(e,this.e)+e.offsetWidth-1;

		th.rsf[5].style.marginTop=offsety+get_y(e,this.e)+e.offsetHeight-1;
		th.rsf[5].style.marginLeft=offsetx+get_x(e,this.e)+(e.offsetWidth/2)-(th.rsf[5].offsetWidth/2);

		th.rsf[6].style.marginTop=offsety+get_y(e,this.e)+e.offsetHeight-1;
		th.rsf[6].style.marginLeft=offsetx+get_x(e,this.e)-th.rsf[6].offsetWidth+1;

		th.rsf[7].style.marginTop=offsety+get_y(e,this.e)+(e.offsetHeight/2)-(th.rsf[7].offsetHeight/2);
		th.rsf[7].style.marginLeft=offsetx+get_x(e,this.e)-th.rsf[7].offsetWidth+1;
		
		//if(th.rs!=9)
		//{
			th.mov.style.marginTop=offsety+get_y(e,this.e)-th.mov.offsetHeight;
			th.mov.style.marginLeft=offsetx+get_x(e,this.e);
		//}
	};
	this.ctrl_onmousedown=function(ev,param2)
	{
		var e;
		if(ev)
			e=ev;
		else
			e=window.event;
		if(e)e.cancelBubble=true;
		var th=this.rspanel;
		if(th.rs==9)
		{
			th.rs=0;
			th.calc_points(th.cur_element);
			if(th.wnd.on_endresize)
			{
				th.wnd.on_endresize(this);
			}
			if(th.onchange)th.onchange(th,th.cur_element);
		}
		else if(th.rs!=0 && th.rs!=null)
		{
			this.rspanel.calc_points(this.rspanel.cur_element);
			this.rspanel.rs=0;
			if(this.rspanel.onchange)this.rspanel.onchange(this.rspanel,this.rspanel.cur_element);
		}
		if(th.cur_element != null)
		{
			if(th.cur_element==this)
			{
				if(th.onmousedown)
					if(!param2)
						th.onmousedown(th,this);
				return;
			}
			/*if(th.cur_element.__border != null)
			{*/
			//alert(th.cur_element.__border);
			th.cur_element.style.borderTop=th.cur_element.__bordertop;
			th.cur_element.style.borderBottom=th.cur_element.__borderbottom;
			th.cur_element.style.borderLeft=th.cur_element.__borderleft;
			th.cur_element.style.borderRight=th.cur_element.__borderright;
			if(th.cur_element.__bordertop=="")
			{
				th.cur_element.style.borderTopStyle="";
				th.cur_element.style.borderTopWidth="";
				th.cur_element.style.borderTopColor="";
			}
			if(th.cur_element.__borderleft=="")
			{
				th.cur_element.style.borderLeftStyle="";
				th.cur_element.style.borderLeftWidth="";
				th.cur_element.style.borderLeftColor="";
			}
			if(th.cur_element.__borderbottom=="")
			{
				th.cur_element.style.borderBottomStyle="";
				th.cur_element.style.borderBottomWidth="";
				th.cur_element.style.borderBottomColor="";
			}
			if(th.cur_element.__borderright=="")
			{
				th.cur_element.style.borderRightStyle="";
				th.cur_element.style.borderRightWidth="";
				th.cur_element.style.borderRightColor="";
			}
			if(th.cur_element.__borderstyle!=null)
			{
				th.cur_element.style.borderStyle=th.cur_element.__borderstyle;
				th.cur_element.__borderstyle=null;
			}
			if(th.cur_element.__borderwidth!=null)
			{
				th.cur_element.style.borderWidth=th.cur_element.__borderwidth;
				th.cur_element.__borderwidth=null;
			}
			if(th.cur_element.__bordercolor!=null)
			{
				th.cur_element.style.borderColor=th.cur_element.__bordercolor;
				th.cur_element.__bordercolor=null;
			}
			//}
		}
		th.cur_element=this;
		/*th.rsf[0].style.marginTop=offsety+gety(this)-th.rsf[0].offsetHeight+1;
		th.rsf[0].style.marginLeft=offsetx+getx(this)-th.rsf[0].offsetWidth+1;

		th.rsf[1].style.marginTop=offsety+gety(this)-th.rsf[1].offsetHeight;
		th.rsf[1].style.marginLeft=offsetx+getx(this)+(this.offsetWidth/2)-(th.rsf[1].offsetWidth/2);

		th.rsf[2].style.marginTop=offsety+gety(this)-th.rsf[2].offsetHeight+1;
		th.rsf[2].style.marginLeft=offsetx+getx(this)+this.offsetWidth-1;

		th.rsf[3].style.marginTop=offsety+gety(this)+(this.offsetHeight/2)-(th.rsf[3].offsetHeight/2);
		th.rsf[3].style.marginLeft=offsetx+getx(this)+this.offsetWidth;

		th.rsf[4].style.marginTop=offsety+gety(this)+this.offsetHeight-1;
		th.rsf[4].style.marginLeft=offsetx+getx(this)+this.offsetWidth-1;

		th.rsf[5].style.marginTop=offsety+gety(this)+this.offsetHeight;
		th.rsf[5].style.marginLeft=offsetx+getx(this)+(this.offsetWidth/2)-(th.rsf[5].offsetWidth/2);

		th.rsf[6].style.marginTop=offsety+gety(this)+this.offsetHeight-1;
		th.rsf[6].style.marginLeft=offsetx+getx(this)-th.rsf[6].offsetWidth+1;

		th.rsf[7].style.marginTop=offsety+gety(this)+(this.offsetHeight/2)-(th.rsf[7].offsetHeight/2);
		th.rsf[7].style.marginLeft=offsetx+getx(this)-th.rsf[7].offsetWidth;*/
		e=this;
		//if(e.__bordertop==null)
		//{
		e.__bordertop=e.style.borderTop;
		//}
		//if(e.__borderbottom==null)
		//{
		e.__borderbottom=e.style.borderBottom;
		//}
		//if(e.__borderleft==null)
		//{
		e.__borderleft=e.style.borderLeft;
		//}
		//if(e.__borderright==null)
		//{
		e.__borderright=e.style.borderRight;
		//}
		//if(e.__borderstyle1==null)
		//{
		e.__borderstyle1=e.style.borderStyle;
		//}
		//if(e.__borderwidth1==null)
		//{
		e.__borderwidth1=e.style.borderWidth;
		//}
		//if(e.__bordercolor1==null)
		//{
		e.__bordercolor1=e.style.borderColor;
		//}
		e.style.border="#000000 1px dotted";
		th.calc_points(this);
		//if(e)
		if(th.onselect)
			th.onselect(th,this);
		if(th.onmousedown)
			if(!param2)
				th.onmousedown(th,this);
	}
	this.tmpf=function()
	{
		var i;
		var th=this;
		th.mov.contentWindow.document.body.topMargin="0";
		th.mov.contentWindow.document.body.bottomMargin="0";
		th.mov.contentWindow.document.body.leftMargin="0";
		th.mov.contentWindow.document.body.rightMargin="0";
		th.mov.contentWindow.document.body.style.cursor="move";
		th.mov.contentWindow.document.body.style.backgroundImage="url(move.gif)";
		th.mov.contentWindow.document.body.rs_img_mode=RS_IMG_MODE_BG;
		if(typeof wnd.rs_img == "object")
		{
			window.rs_img[window.rs_img.length]=th.mov.contentWindow.document.body;
		}
		else
		{
			window.rs_img=[th.mov.contentWindow.document.body];
		}
		th.mov.contentWindow.document.body.style.backgroundRepeat="no-repeat";
		th.mov.contentWindow.document.body.style.fontSize="1px";
		th.mov.contentWindow.document.rspanel=this;
		th.mov.contentWindow.document.__wnd=th.mov.contentWindow;
		th.mov.contentWindow.document.onmousedown=function(ev)
		{
			var e;
			if(ev)
				e=ev;
			else
				e=this.__wnd.event;
			var th=this.rspanel;
			if(th.cur_element)
			{
				/*if(th.cur_element.tagName.toLowerCase()=="td")
				{
					th.cur_element.parentNode.parentNode.parentNode.onmousedown(null,true);
				}
				else if(th.cur_element.__td!=null)
				{
					th.cur_element.__td.parentNode.parentNode.parentNode.onmousedown(null,true);
				}*/
				th.cur_element=th.find_movable_element(th.cur_element);
				var tmp;
				try
				{
					tmp=th.cur_element.style.marginTop;
					if(tmp==null)
					{
						tmp=0;
					}
					else
					{
						tmp=parseInt(tmp.substring(0,tmp.length-2));
					}
					if(isNaN(tmp))tmp=0;
				}
				catch(ex)
				{
					tmp=0;
				}
				th.cur_element.md_t=tmp;
				try
				{
					tmp=th.cur_element.style.marginLeft;
					if(tmp==null)
					{
						tmp=0;
					}
					else
					{
						tmp=parseInt(tmp.substring(0,tmp.length-2));
					}
					if(isNaN(tmp))tmp=0;
				}
				catch(ex2)
				{
					tmp=0;
				}
				th.cur_element.md_l=tmp;
				th.cur_element.md_b=th.cur_element.md_t+th.cur_element.offsetHeight;
				th.cur_element.md_r=th.cur_element.md_l+th.cur_element.offsetWidth;
				th.cur_element.mdx=e.clientX+get_x(th.mov);
				th.cur_element.mdy=e.clientY+get_y(th.mov);
				//alert(e.clientX);
				th.rs=9;
				if(th.wnd.on_beginresize)
				{
					th.wnd.on_beginresize(this);
				}
			}
		};
		th.mov.contentWindow.document.onmousemove=function(ev)
		{
			var e;
			if(ev)
				e=ev;
			else
				e=this.__wnd.event;
			var th=this.rspanel;
			if(th.rs==0 || th.rs==null)
				return;
			try
			{
				var diff_x=e.clientX+get_x(th.mov)-th.cur_element.mdx;
				var diff_y=e.clientY+get_y(th.mov)-th.cur_element.mdy;
				//alert("xxaaaxaxaxxa");
				//alert(th.rs);
				var nv;
				if(th.rs==9)
				{
					nv=th.cur_element.md_l+diff_x;
					if(nv<0)nv=0;
					th.cur_element.style.marginLeft=nv+"px";
					
					nv=th.cur_element.md_t+diff_y;
					if(nv<0)nv=0;
					th.cur_element.style.marginTop=nv+"px";
					
					th.calc_points(th.cur_element);
					
					if(th.onresize)th.onresize();
				}
			}
			catch(ex){}
		};
		th.mov.contentWindow.document.onmouseup=function()
		{
			var th=this.rspanel;
			th.rs=0;
			th.calc_points(th.cur_element);
			if(th.wnd.on_endresize)
			{
				th.wnd.on_endresize(this);
			}
			if(th.onchange)th.onchange(th,th.cur_element);
		};
		th.mov.style.display="none";
		for(i=0;i<this.rsf.length;i++)
		{
			this.rsfd[i]=this.rsf[i].contentWindow.document;
			this.rsfb[i]=this.rsf[i].contentWindow.document.body;
			this.rsfb[i].topMargin="0";
			this.rsfb[i].bottomMargin="0";
			this.rsfb[i].leftMargin="0";
			this.rsfb[i].rightMargin="0";
			
			this.rsfd[i].topMargin="0";
			this.rsfd[i].bottomMargin="0";
			this.rsfd[i].leftMargin="0";
			this.rsfd[i].rightMargin="0";
			
			this.rsfd[i].__wnd=this.rsf[i].contentWindow;
			this.rsfd[i].rspanel=this;
			this.rsfd[i].ind=i+1;
			this.rsf[i].ind=i+1;
			this.rsf[i].style.display="none";
			this.rsfd[i].onmousedown=function(ev)
			{
				var e;
				if(ev)
					e=ev;
				else
					e=this.__wnd.event;
				var th=this.rspanel;
				if(th.cur_element)
				{
					/*if(th.cur_element.tagName.toLowerCase()=="td")
					{
						th.cur_element.parentNode.parentNode.parentNode.onmousedown(null,true);
					}
					else if(th.cur_element.__td!=null)
					{
						th.cur_element.__td.parentNode.parentNode.parentNode.onmousedown(null,true);
					}*/
					th.cur_element=th.find_resizable_element(th.cur_element);
					var tmp;
					try
					{
						tmp=th.cur_element.style.marginTop;
						if(tmp==null)
						{
							tmp=0;
						}
						else
						{
							tmp=parseInt(tmp.substring(0,tmp.length-2));
						}
						if(isNaN(tmp))tmp=0;
					}
					catch(ex)
					{
						tmp=0;
					}
					th.cur_element.md_t=tmp;
					try
					{
						tmp=th.cur_element.style.marginLeft;
						if(tmp==null)
						{
							tmp=0;
						}
						else
						{
							tmp=parseInt(tmp.substring(0,tmp.length-2));
						}
						if(isNaN(tmp))tmp=0;
					}
					catch(ex2)
					{
						tmp=0;
					}
					th.cur_element.md_l=tmp;
					th.cur_element.md_b=th.cur_element.md_t+th.cur_element.offsetHeight;
					th.cur_element.md_r=th.cur_element.md_l+th.cur_element.offsetWidth;
					th.cur_element.mdx=e.clientX;
					th.cur_element.mdy=e.clientY;
					//alert(e.clientX);
					th.rs=this.ind;
				}
			};
			this.rsfd[i].onmousemove=function(ev)
			{
				var e;
				if(ev)
					e=ev;
				else
					e=this.__wnd.event;
				var th=this.rspanel;
				if(th.rs==0 || th.rs==null)
					return;
				var diff_x=e.clientX-th.cur_element.mdx;
				var diff_y=e.clientY-th.cur_element.mdy;
				//alert("xxaaaxaxaxxa");
				//alert(th.rs);
				var nv;
				switch(th.rs)
				{
					case 1:
						//topleft
						nv=th.cur_element.md_b-th.cur_element.md_t-diff_y;
						if(nv<1)nv=1;
						th.cur_element.style.height=nv+"px";
						
						nv=th.cur_element.md_b-th.cur_element.offsetHeight;
						if(nv<1)nv=1;
						th.cur_element.style.marginTop=nv+"px";

						nv=th.cur_element.md_r-th.cur_element.md_l-diff_x;
						if(nv<1)nv=1;
						th.cur_element.style.width=nv+"px";
						
						nv=th.cur_element.md_r-th.cur_element.offsetWidth;
						if(nv<1)nv=1;
						th.cur_element.style.marginLeft=nv+"px";
						break;
					case 2:
						//top
						nv=th.cur_element.md_b-th.cur_element.md_t-diff_y;
						if(nv<1)nv=1;
						th.cur_element.style.height=nv+"px";
						
						nv=th.cur_element.md_b-th.cur_element.offsetHeight;
						if(nv<1)nv=1;
						th.cur_element.style.marginTop=nv+"px";
						break;
					case 3:
						//topright
						nv=(th.cur_element.md_r-th.cur_element.md_l+diff_x);
						if(nv<1)nv=1;
						th.cur_element.style.width=nv+"px";
						
						nv=th.cur_element.md_b-th.cur_element.md_t-diff_y;
						if(nv<1)nv=1;
						th.cur_element.style.height=nv+"px";
						
						nv=th.cur_element.md_b-th.cur_element.offsetHeight;
						if(nv<1)nv=1;
						th.cur_element.style.marginTop=nv+"px";
						break;
					case 4:
						//right
						nv=(th.cur_element.md_r-th.cur_element.md_l+diff_x);
						if(nv<1)nv=1;
						th.cur_element.style.width=nv+"px";
						//th.calc_points(th.cur_element);
						//alert("");
						break;
					case 5:
						//bottomright
						nv=(th.cur_element.md_r-th.cur_element.md_l+diff_x);
						if(nv<1)nv=1;
						th.cur_element.style.width=nv+"px";
						
						nv=(th.cur_element.md_b-th.cur_element.md_t+diff_y);
						if(nv<1)nv=1;
						th.cur_element.style.height=nv+"px";
						break;
					case 6:
						//bottom
						nv=(th.cur_element.md_b-th.cur_element.md_t+diff_y);
						if(nv<1)nv=1;
						th.cur_element.style.height=nv+"px";
						break;
					case 7:
						//bottomleft
						nv=(th.cur_element.md_b-th.cur_element.md_t+diff_y);
						if(nv<1)nv=1;
						th.cur_element.style.height=nv+"px";
						
						nv=th.cur_element.md_r-th.cur_element.md_l-diff_x;
						if(nv<1)nv=1;
						th.cur_element.style.width=nv+"px";
						
						nv=th.cur_element.md_r-th.cur_element.offsetWidth;
						if(nv<1)nv=1;
						th.cur_element.style.marginLeft=nv+"px";
						break;
					case 8:
						//left
						nv=th.cur_element.md_r-th.cur_element.md_l-diff_x;
						if(nv<1)nv=1;
						th.cur_element.style.width=nv+"px";
						
						nv=th.cur_element.md_r-th.cur_element.offsetWidth;
						if(nv<1)nv=1;
						th.cur_element.style.marginLeft=nv+"px";
						break;
				}
				if(th.onresize)th.onresize();
			};
			this.rsfd[i].onmouseup=function()
			{
				this.rspanel.calc_points(this.rspanel.cur_element);
				this.rspanel.rs=0;
				if(this.rspanel.onchange)this.rspanel.onchange(this.rspanel,this.rspanel.cur_element);
			};
		}
		/*this.rsfd[0].style.cursor="se-resize";
		this.rsfd[1].style.cursor="s-resize";
		this.rsfd[2].style.cursor="sw-resize";
		this.rsfd[3].style.cursor="e-resize";
		this.rsfd[4].style.cursor="se-resize";
		this.rsfd[5].style.cursor="s-resize";
		this.rsfd[6].style.cursor="sw-resize";
		this.rsfd[7].style.cursor="e-resize";*/

		/*this.rsf[0].style.cursor="se-resize";
		this.rsf[1].style.cursor="s-resize";
		this.rsf[2].style.cursor="sw-resize";
		this.rsf[3].style.cursor="e-resize";
		this.rsf[4].style.cursor="se-resize";
		this.rsf[5].style.cursor="s-resize";
		this.rsf[6].style.cursor="sw-resize";
		this.rsf[7].style.cursor="e-resize";*/

		this.rsfb[0].style.cursor="se-resize";
		this.rsfb[1].style.cursor="s-resize";
		this.rsfb[2].style.cursor="sw-resize";
		this.rsfb[3].style.cursor="e-resize";
		this.rsfb[4].style.cursor="se-resize";
		this.rsfb[5].style.cursor="s-resize";
		this.rsfb[6].style.cursor="sw-resize";
		this.rsfb[7].style.cursor="e-resize";

		this.rsfb[0].innerHTML="<div style='cursor: se-resize;width:100%; height:100%;'></div>";
		this.rsfb[1].innerHTML="<div style='cursor: s-resize;width:100%; height:100%;'></div>";
		this.rsfb[2].innerHTML="<div style='cursor: sw-resize;width:100%; height:100%;'></div>";
		this.rsfb[3].innerHTML="<div style='cursor: e-resize;width:100%; height:100%;'></div>";
		this.rsfb[4].innerHTML="<div style='cursor: se-resize;width:100%; height:100%;'></div>";
		this.rsfb[5].innerHTML="<div style='cursor: s-resize;width:100%; height:100%;'></div>";
		this.rsfb[6].innerHTML="<div style='cursor: sw-resize;width:100%; height:100%;'></div>";
		this.rsfb[7].innerHTML="<div style='cursor: e-resize;width:100%; height:100%;'></div>";
	};
	this.ctrl_onkeypress=function(ev)
	{
		try
		{
			var e;
			if(ev)
				e=ev;
			else
				e=window.event;
			if(e)
			{
				e.cancelBubble=true;
				var n;
				if(is_ie)
				{
					n=e.keyCode;
				}
				else
				{
					n=e.which;
				}
				if(n==16)
				{
					var th=this.rspanel;
					var p=th.cur_element.parentNode;
					if(p!=null)
					{
						p.__tmpf=th.ctrl_onmousedown;
						p.__tmpf(null,true);
					}
				}
			}
		}
		catch(exc){}
	};
	this.addcontrol=function(ctrl,p)
	{
		if(ctrl==null)return;
		if(p==null)p=this.e;
		ctrl.onmousedown=this.ctrl_onmousedown;
		ctrl.onkeypress=this.ctrl_onkeypress;
		ctrl.rspanel=this;
		ctrl.style.position="absolute";
		p.appendChild(ctrl);
	};
	this.add=function(ctrl)
	{
		if(ctrl==null)return;
		ctrl.onmousedown=this.ctrl_onmousedown;
		ctrl.onkeypress=this.ctrl_onkeypress;
		ctrl.rspanel=this;
	};
	this.intv=wnd.setInterval(new Function("\
	var th=window.obj["+this.ind.toString()+"];\
	var i;\
	for(i=0;i<th.rsf.length;i++)\
	{\
		if(!th.rsf[i].contentWindow)\
			return;\
		if(!th.rsf[i].contentWindow.document)\
			return;\
		if(!th.rsf[i].contentWindow.document.body)\
			return;\
	}\
	if(!th.mov.contentWindow)\
		return;\
	if(!th.mov.contentWindow.document)\
		return;\
	if(!th.mov.contentWindow.document.body)\
		return;\
	th.wnd.clearInterval(th.intv);\
	th.tmpf();\
	"),100);
}

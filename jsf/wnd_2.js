// JScript File

var RS_IMG_MODE_BG=1;
var RS_IMG_MODE_IMG=2;
var RS_IMG_MODE_ALL=3;

var RESTORE_MINIMIZE=1;
var RESTORE_MAXIMIZE=2;
var RESTORE_DO_MAXIMIZE=3;
if(window.on_beginresize==null)window.on_beginresize=function(e)
{
	if(typeof window.rs_img == "object")
	{
		var i;
		for(i=0;i<window.rs_img.length;i++)
		{
			if(!window.rs_img[i].imgishidden)
			{
				if(window.rs_img[i].rs_img_mode==RS_IMG_MODE_BG)
				{
					window.rs_img[i].imgishidden=true;
					window.rs_img[i].__tmp=window.rs_img[i].style.backgroundImage;
					window.rs_img[i].style.backgroundImage="";
				}
				else if(window.rs_img[i].rs_img_mode==RS_IMG_MODE_IMG)
				{
					window.rs_img[i].imgishidden=true;
					window.rs_img[i].__tmp=window.rs_img[i].src;
					window.rs_img[i].src="";
				}
				else if(window.rs_img[i].rs_img_mode==RS_IMG_MODE_ALL)
				{
					window.rs_img[i].imgishidden=true;
					window.rs_img[i].__tmp1=window.rs_img[i].style.backgroundImage;
					window.rs_img[i].__tmp2=window.rs_img[i].src;
					window.rs_img[i].style.backgroundImage="";
					window.rs_img[i].src="";
				}
			}
		}
	}
};
if(window.on_endresize==null)window.on_endresize=function(e)
{
	if(typeof window.rs_img == "object")
	{
		var i;
		for(i=0;i<window.rs_img.length;i++)
		{
			if(window.rs_img[i].rs_img_mode==RS_IMG_MODE_BG)
			{
				window.rs_img[i].imgishidden=false;
				window.rs_img[i].style.backgroundImage=window.rs_img[i].__tmp;
			}
			else if(window.rs_img[i].rs_img_mode==RS_IMG_MODE_IMG)
			{
				window.rs_img[i].imgishidden=false;
				window.rs_img[i].src=window.rs_img[i].__tmp;
			}
			else if(window.rs_img[i].rs_img_mode==RS_IMG_MODE_ALL)
			{
				window.rs_img[i].imgishidden=false;
				window.rs_img[i].style.backgroundImage=window.rs_img[i].__tmp1;
				window.rs_img[i].src=window.rs_img[i].__tmp2;
			}
		}
	}
};
function getx(oElement)
{
	return oElement.offsetLeft;
	var iReturnValue = 0;
	while( oElement != null )
	{
		iReturnValue += oElement.offsetLeft;
		oElement = oElement.offsetParent;
	}
	return iReturnValue;
}
function gety(oElement)
{
	return oElement.offsetTop;
	var iReturnValue = 0;
	while( oElement != null )
	{
		iReturnValue += oElement.offsetTop;
		oElement = oElement.offsetParent;
	}
	return iReturnValue;
}
function get_zi()
{
	var ret=0;
	var ifr=document.getElementsByTagName("iframe");
	var i;
	for(i=0;i<ifr.length;i++)
	{
		var tmp=parseInt(ifr[i].style.zIndex);
		if((!isNaN(tmp)) && (tmp!=null))
		{
			if(tmp>ret)ret=tmp;
		}
	}
	ifr=null;
	return ret;
	
}
function sortf(a, b)
{
	return a[0] - b[0];
}
function rearrange_zi()
{
	var zi=[];
	var ifr=document.getElementsByTagName("iframe");
	var i;
	for(i=0;i<ifr.length;i++)
	{
		if(ifr[i].style.zIndex!=null)
		{
			if(!isNaN(ifr[i].style.zIndex))
			{
				zi[zi.length]=[ifr[i].style.zIndex,ifr[i]];
			}
		}
	}
	zi=zi.sort(sortf);
	for(i=0;i<zi.length;i++)
	{
		zi[i][1].style.zIndex=i;
		if(zi[i][1].wnd)
		{
			if(zi[i][1].wnd.onzindexchange)zi[i][1].wnd.onzindexchange(zi[i][1].wnd);
		}
	}
}

function wnd2(x,y,w,h,wnd,p,cb,title,controlbox)
{
	this.type="window";
	this.ready=false;
	this.selectable=true;
	this._selectable=this.selectable;
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
	this.param=new Object();
	if(x==null)x=0;
	if(y==null)y=0;
	if(w==null)w=300;
	if(h==null)h=250;
	if(wnd==null)wnd=window;
	if(title==null)title="Form"+(this.ind+1).toString();
	if(controlbox==null)
		controlbox=
		[
			["span","0","width:17px;height:18px;border:#555555 1px solid;font-size:9px;text-align:center;font-family: Webdings;overflow:hidden;",15,"minimize/restore","w.minimizerestore();"],
			["span","1","width:17px;height:18px;border:#555555 1px solid;font-size:9px;text-align:center;font-family: Webdings;overflow:hidden;",15,"maximize/restore","w.maximizerestore();"],
			["span","r","width:17px;height:18px;border:#555555 1px solid;font-size:11px;text-align:center;font-family: Webdings;overflow:hidden;",15,"close","w.close();"]
		];
	this.param.x=x;
	this.param.y=y;
	this.param.w=w;
	this.param.h=h;
	this.param.wnd=wnd;
	this.param.cb=cb;
	this.param.p=p;
	this.param.title=title;
	this.param.controlbox=controlbox;
	this.showintaskbar=true;
	this.fr=wnd.document.createElement("iframe");
	this.fr.wnd=this;
	this.fr.style.border="none";
	this.fr.style.position="absolute";
	this.fr.style.marginTop=y+"px";
	this.fr.style.marginLeft=x+"px";
	this.fr.style.width=w.toString()+"px";
	this.fr.style.height=h.toString()+"px";
	this.fr.style.display="none";
	p.appendChild(this.fr);
	this.fr.style.zIndex=get_zi();
	if(this.onzindexchange)this.onzindexchange(this);
	this.fr.src="javascript:\"<html><body topmargin=\\\"0\\\" leftmargin=\\\"0\\\" bottommargin=\\\"0\\\" rightmargin=\\\"0\\\" style=\\\"border:none;overflow:hidden\\\"></body></html>\"";
	//p.appendChild(this.fr);
	this.visible=false;
	this.maximized=false;
	this.minimized=false;
	this.show=function()
	{
		try
		{
			try
			{
				if(this.param.wnd.wnd_do_show)
				{
					if(this.param.wnd.wnd_do_show(this))
					{
						this.visible=true;
						if(this.param.wnd.wnd_on_show)
						{
							return this.param.wnd.wnd_on_show(this);
						}
						return;
					}
				}
			}
			catch(ex){}
			//this.param.p.appendChild(this.fr);
			this.fr.style.display="";
			this.fr.style.visibility="visible";
			this.visible=true;
			if(this.onshow)this.onshow(this);
			if(this.param.wnd.wnd_on_show)
			{
				return this.param.wnd.wnd_on_show(this);
			}
		}
		catch(exc){}
	};
	this.close=function()
	{
		try
		{
			try
			{
				if(this.param.wnd.wnd_do_close)
				{
					if(this.param.wnd.wnd_do_close(this))
					{
						this.visible=false;
						if(this.param.wnd.wnd_on_close)
						{
							return this.param.wnd.wnd_on_close(this);
						}
						return;
					}
				}
			}
			catch(ex){}
			this.fr.style.display="none";
			this.fr.style.visibility="hidden";
			this.visible=false;
			if(this.onclose)this.onclose(this);
			if(this.param.wnd.wnd_on_close)
			{
				return this.param.wnd.wnd_on_close(this);
			}
		}
		catch(exc){}
	};
	this.maximize=function()
	{
	    if(this.no_maximize)return;
		if(this.maximized)return;
		if(!this.minimized)
		{
			this.tmph=this.fr.offsetHeight;
			this.tmpw=this.fr.offsetWidth;
			this.tmpx=getx(this.fr);
			this.tmpy=gety(this.fr);
		}
		try
		{
			if(this.param.wnd.wnd_do_maximize)
			{
				if(this.param.wnd.wnd_do_maximize(this))
				{
					this.maximized=true;
					this.minimized=false;
					if(this.param.wnd.wnd_on_maximize)
					{
						return this.param.wnd.wnd_on_maximize(this);
					}
					return;
				}
			}
		}
		catch(exc){}
		this.fr.style.width="100%";
		this.fr.style.height="100%";
		//this.fr.dock=5;
		this.fr.style.marginTop="0px";
		this.fr.style.marginLeft="0px";
		this.maximized=true;
		this.minimized=false;
		if(this.onmaximize)this.onmaximize(this);
		if(this.param.wnd.wnd_on_maximize)
		{
			return this.param.wnd.wnd_on_maximize(this);
		}
		try{layout_document();}catch(exc){}
		//this.fr.style.cssText="position:absolute;border:none;top:0px;left:0px;width:100%;height:100%;";
		//this.param.p.appendChild(this.fr);
		//this.close();
		//this.show();
	};
	this.minimize=function()
	{
		if(this.minimized)return;
		if(!this.maximized)
		{
			this.tmph=this.fr.offsetHeight;
			this.tmpw=this.fr.offsetWidth;
			this.tmpx=getx(this.fr);
			this.tmpy=gety(this.fr);
		}
		try
		{
			if(this.param.wnd.wnd_do_minimize)
			{
				if(this.param.wnd.wnd_do_minimize(this))
				{
					this.minimized=true;
					if(this.param.wnd.wnd_on_minimize)
					{
						return this.param.wnd.wnd_on_minimize(this);
					}
					return;
				}
			}
		}
		catch(exc){}
		//this.fr.style.width="150px";
		this.fr.style.height="21px";
		//this.fr.style.marginTop=(this.param.p.offsetHeight-30).toString()+"px";
		//this.fr.style.marginLeft="0px";
		this.minimized=true;
		if(this.onminimize)this.onminimize(this);
		if(this.param.wnd.wnd_on_minimize)
		{
			return this.param.wnd.wnd_on_minimize(this);
		}
		//this.maximized=false;
	};
	this.restore=function()
	{
		var m;
		if(this.minimized)
		{
			if(this.maximized)
			{
				m=RESTORE_DO_MAXIMIZE;
				this.fr.style.width="100%";
				this.fr.style.height="100%";
				this.fr.style.marginTop="0px";
				this.fr.style.marginLeft="0px";
			}
			else
			{
				m=RESTORE_MINIMIZE;
				this.fr.style.marginTop=(this.tmpy)+"px";
				this.fr.style.marginLeft=(this.tmpx)+"px";
				this.fr.style.width=this.tmpw+"px";
				this.fr.style.height=this.tmph+"px";
			}
			this.minimized=false;
		}
		else if(this.maximized)
		{
			m=RESTORE_MAXIMIZE;
			this.fr.style.marginTop=(this.tmpy)+"px";
			this.fr.style.marginLeft=(this.tmpx)+"px";
			this.fr.style.width=this.tmpw+"px";
			this.fr.style.height=this.tmph+"px";
			this.maximized=false;
			this.minimized=false;
		}
		//this.fr.dock=0;
		if(this.onrestore)this.onrestore(this,m);
		if(this.param.wnd.wnd_on_restore)
		{
			return this.param.wnd.wnd_on_restore(this,m);
		}
	};
	this.add_dialog=function(w)
	{
	    w.dlgparent=this;
	    w.onclose=function(th)
	    {
	        if(th.dlgparent)
	        {
	            th.dlgparent.remove_dialog(th);
	            if(th.dlgparent.dlg.length==0)th.dlgparent.activate();
	        }
	    };
	    this.clientarea.disabled=true;
	    this.disable();
	    if(!this.dlg)this.dlg=[];
	    this.dlg[this.dlg.length]=w;
	};
	this.get_dlg_index=function(w)
	{
        for(i=0;i<this.dlg.length;i++)
        {
            if(this.dlg[i]==w)return i;
        }
	    return -1;
	};
	this.remove_dialog=function(w)
	{
		var i;
		if((typeof w) == "object")
		{
			i=this.get_dlg_index(w);
		}
		else
		{
			i=w;
		}
		if(i<0)return;
		var ii;
		for(ii=i+1;ii<this.dlg.length;ii++)
		{
			this.dlg[i-1]=this.dlg[i];
		}
		this.dlg.length--;
		if(this.dlg.length==0)
		{
			this.clientarea.disabled=false;
			this.enable();
		}
	};
	this.activate=function(disable_restore,_do_flash)
	{
	    //alert(this.dlg);
		if(this.param.wnd.wnd_do_activate)
		{
			if(this.param.wnd.wnd_do_activate(this))
			{
				if(this.param.wnd.wnd_on_activate)
				{
					this.param.wnd.wnd_on_activate(this);
				}
				return;
			}
		}
		if(disable_restore)
		{}
		else
		{
			if(this.minimized)this.restore();
		}
		rearrange_zi();
		this.fr.style.zIndex=get_zi()+1;
		if(this.onzindexchange)this.onzindexchange(this);
		if(this.param.wnd.wnd_on_activate)
		{
			this.param.wnd.wnd_on_activate(this);
		}
	    if(this.dlg)
	    {
	        if(this.dlg.length>0)
	        {
	            if(this.dlg[0].activate(false,true)) this.dlg[0].b.focus();
	            
	            //this.dlg[0].do_flash();
	            return;
	        }
	    }
		//this.b.focus();
		if(_do_flash) this.do_flash();
		return true;
	};
	this.disable=function()
	{
		if(!this.disableframe)return;
		this.disableframe.style.display="";
	};
	this.enable=function()
	{
		if(!this.disableframe)return;
		this.disableframe.style.display="none";
	};
	this.maximizerestore=function()
	{
		if(this.maximized)
			this.restore();
		else
			this.maximize();
	};
	this.minimizerestore=function()
	{
		if(this.minimized)
			this.restore();
		else
			this.minimize();
	};
	this.set_width=function(v)
	{
		this.fr.style.width=v.toString()+"px";
		if(this.onresize)this.onresize(this);
	};
	this.set_height=function(v)
	{
		this.fr.style.height=v.toString()+"px";
		if(this.onresize)this.onresize(this);
	};
	this.set_top=function(v)
	{
		this.fr.style.marginTop=v.toString()+"px";
		if(this.onmove)this.onmove(this);
	};
	this.set_left=function(v)
	{
		this.fr.style.marginLeft=v.toString()+"px";
		if(this.onmove)this.onmove(this);
	};
	this.get_width=function()
	{
		return this.fr.offsetWidth;
	};
	this.get_height=function()
	{
		return this.fr.offsetHeight;
	};
	this.get_top=function()
	{
		return this.fr.offsetTop;
	};
	this.get_left=function()
	{
		return this.fr.offsetLeft;
	};
	this.set_area=function(a)
	{
		/*a.style.width="100%";
		a.style.height="100%";
		a.style.display="";
		a.style.visibility="visible";*/
		this.clientarea.innerHTML=a.innerHTML;
	};
	this.tmpf=function()
	{
		var tmpf1=function(){if(this.fn){this.fn(this.wnd);}};
		var tmpf2=function(){this.wnd.setstatus(this.title);};
		var tmpf3=function(){this.wnd.setstatus("");};
		this.b=this.fr.contentWindow.document.body;
		this.d=this.fr.contentWindow.document;
		this.b.topMargin="0";
		this.b.leftMargin="0";
		this.b.bottomMargin="0";
		this.b.rightMargin="0";
		this.d.topMargin="0";
		this.d.leftMargin="0";
		this.d.bottomMargin="0";
		this.d.rightMargin="0";
		//this.b.innerHTML="";
		this.b.style.overflow="hidden";
		this.b.scroll="no";
		this.b.style.border="none";
		this.b.wnd=this;
		//this.b.onselectstart=function(){return this.wnd._selectable;};
		this.fr.contentWindow.wnd=this;
		this.fr.contentWindow.onfocus=function()
		{
			this.wnd.activate(true);
		};
		this.fr.onfocus=function()
		{
			this.wnd.activate(true);
		};
		//this.b.unSelectable="On";
		this.b.onfocus=function()
		{
			this.wnd.activate(true);
		};
		this.b.onmouseup=function()
		{
			this.wnd._selectable=this.wnd.selectable;
			if(this.wnd.resizing)
			{
				if(this.wnd.onendresize)this.wnd.onendresize(this.wnd);
			}
			if(this.wnd.mov)
			{
				if(this.wnd.onendmove)this.wnd.onendmove(this.wnd);
			}
			if(this.wnd.resizing || this.wnd.mov)this.wnd.input_resize.style.backgroundImage="url(rs.gif)";
			this.wnd.mov=false;
			this.wnd.resizing=false;
		};
		this.disableframe=this.d.createElement("div");
		this.disableframe.style.cssText="width:100%; height:100%; display:none; position:absolute; background-image:url(disable.gif);z-index:101;";
		this.b.appendChild(this.disableframe);
		this.t=this.d.createElement("table");
			this.t.style.cursor="default";
			this.t.cellSpacing="0";
			this.t.cellPadding="0";
			this.t.style.width="100%";
			this.t.style.height="100%";
			this.t.style.backgroundColor="#d4d0c8";
			this.t.style.position="absolute";
			this.t.style.marginTop="0px";
			this.t.style.marginLeft="0px";
			this.tb=this.d.createElement("tbody");
				this.tr_title=this.d.createElement("tr");
					this.td_title=this.d.createElement("td");
						this.td_title.style.height="15px";
						this.input_title=this.d.createElement("input");
							this.input_title.style.border="none";
							this.input_title.readOnly=true;
							this.input_title.style.cursor="default";
							this.input_title.style.width="100%";
							this.input_title.style.backgroundColor="#0a246a";
							this.input_title.style.color="#ffffff";
							this.input_title.style.paddingLeft="5px";
							this.input_title.value=this.param.title;
							this.input_title.wnd=this;
							this.input_title.onmousedown=function(ev)
							{
								if(this.wnd.maximized && (!this.wnd.minimized))return;
								var e;
								if(ev)
									e=ev;
								else
									e=this.wnd.fr.contentWindow.event;
								this.wnd.md_x=e.clientX-this.wnd.param.p.scrollLeft+getx(this.wnd.fr);
								this.wnd.md_y=e.clientY-this.wnd.param.p.scrollTop+gety(this.wnd.fr);
								this.wnd.d_x=getx(this.wnd.fr);
								this.wnd.d_y=gety(this.wnd.fr);
								this.wnd.mov=true;
								this.wnd.input_resize.style.backgroundImage="";
								if(this.wnd.onbeginmove)this.wnd.onbeginmove(this.wnd);
								//this.wnd._selectable=false;
							};
							this.input_title.onmousemove=function(ev)
							{
								var e;
								if(ev)
									e=ev;
								else
									e=this.wnd.fr.contentWindow.event;
								if(this.wnd.mov)
								{
									var ox=(e.clientX-this.wnd.param.p.scrollLeft+getx(this.wnd.fr))-this.wnd.md_x;
									var oy=(e.clientY-this.wnd.param.p.scrollTop+gety(this.wnd.fr))-this.wnd.md_y;
									var nx=this.wnd.d_x+ox;
									var ny=this.wnd.d_y+oy;
									if(nx<0)nx=0;
									if(ny<0)ny=0;
									//if(nx>this.wnd.param.p.offsetWidth-20)nx=this.wnd.param.p.offsetWidth-20;
									//if(ny>this.wnd.param.p.offsetHeight-20)ny=this.wnd.param.p.offsetHeight-20;
									this.wnd.fr.style.marginTop=ny+"px";
									this.wnd.fr.style.marginLeft=nx+"px";
									if(this.wnd.onmove)this.wnd.onmove(this.wnd);
								}
								/*else if(this.wnd.resizing)
								{
									var ow=e.clientX-this.wnd.dx;
									var oh=e.clientY-this.wnd.dy;
									var nw=this.wnd.dw+ow;
									var nh=this.wnd.dh+oh;
									if(nw<100)nw=100;
									if(nh<70)nh=70;
									//if(nx>this.wnd.param.p.offsetWidth-20)nx=this.wnd.param.p.offsetWidth-20;
									//if(ny>this.wnd.param.p.offsetHeight-20)ny=this.wnd.param.p.offsetHeight-20;
									this.wnd.fr.style.width=nw+"px";
									this.wnd.fr.style.height=nh+"px";
								}*/
								return true;
							};
							this.set_title=function(v)
							{
								this.input_title.value=v;
								if(this.param.wnd.wnd_onchange_title)this.param.wnd.wnd_onchange_title(this);
							};
						this.td_title.appendChild(this.input_title);
					this.tr_title.appendChild(this.td_title);
					
					var i;
					this.buttons=[];
					this.buttons_td=[];
					for(i=0;i<this.param.controlbox.length;i++)
					{
						this.buttons_td[i]=this.d.createElement("td");
							this.buttons_td[i].style.width=this.param.controlbox[i][3];
							this.buttons[i]=this.d.createElement(this.param.controlbox[i][0]);
								this.buttons[i].innerHTML=this.param.controlbox[i][1];
								this.buttons[i].style.cssText=this.param.controlbox[i][2];
								this.buttons[i].title=this.param.controlbox[i][4];
								this.buttons[i].alt=this.param.controlbox[i][4];
								this.buttons[i].fn=this.param.controlbox[i][5];
								if(typeof this.buttons[i].fn == "string")this.buttons[i].fn=new Function("w",this.buttons[i].fn);
								this.buttons[i].wnd=this;
								this.buttons[i].onmouseover=tmpf2;
								this.buttons[i].onmouseout=tmpf3;
								this.buttons[i].onclick=tmpf1;
							this.buttons_td[i].appendChild(this.buttons[i]);
						this.tr_title.appendChild(this.buttons_td[i]);
					}
				this.t_title=this.d.createElement("table");
				this.t_title.cellSpacing="0";
				this.t_title.cellPadding="0";
				this.t_title.style.width="100%";
				this.t_title.style.height="100%";
				this.tb_title=this.d.createElement("tbody");
				
				//this.td_title2.style.border="#000000 1px solid";
				//this.td_title2.colSpan="2";
				
				this.tb_title.appendChild(this.tr_title);
				this.t_title.appendChild(this.tb_title);
				this.tr_title2=this.d.createElement("tr");
				this.td_title2=this.d.createElement("td");
				this.td_title2.appendChild(this.t_title);
				this.td_title2.style.height="18px";
				this.tr_title2.appendChild(this.td_title2);
				this.tb.appendChild(this.tr_title2);
				this.tr_content=this.d.createElement("tr");
					this.td_content=this.d.createElement("td");
						//this.td_content.colSpan=this.param.controlbox.length;
						//this.td_content.innerHTML="&nbsp;";
						this.div_content=this.d.createElement("div");
							this.div_content.style.width="100%";
							this.div_content.style.height="100%";
							this.div_content.style.overflow="hidden";
						this.td_content.appendChild(this.div_content);
						this.td_content.style.width="100%";
						this.clientarea=this.div_content;
					this.tr_content.appendChild(this.td_content);
				this.tb.appendChild(this.tr_content);
				this.tr_status=this.d.createElement("tr");
					this.td_status=this.d.createElement("td");
						this.td_status.style.height="20px";
						this.td_status.style.fontSize="13px";
						this.td_status.style.fontFamily="Arial";
						//this.td_status.colSpan=this.param.controlbox.length;
						this.td_status.style.overFlow="hidden";
						this.input_status=this.d.createElement("input");
							this.input_status.style.borderTop="#808080 1px solid";
							this.input_status.style.borderLeft="#808080 1px solid";
							this.input_status.style.borderBottom="#eaeaea 1px solid";
							this.input_status.style.borderRight="#eaeaea 1px solid";
							this.input_status.style.width="100%";
							this.input_status.style.height="100%";
							this.input_status.readOnly=true;
							this.input_status.style.backgroundColor="transparent";
							this.input_status.value="";
							this.input_status.style.fontSize="13px";
							this.input_status.style.fontFamily="Arial";
							this.statusbar=this.input_status;
							this.setstatus=function(v)
							{
								this.input_status.value=v;
								if(this.onstatuschange)this.onstatuschange(this);
							};
						this.td_status.appendChild(this.input_status);
					this.tr_status.appendChild(this.td_status);
					this.td_resize=this.d.createElement("td");
						this.td_resize.colSpan="1";
						this.td_resize.style.width="15px";
						this.td_resize.style.overFlow="hidden";
						this.td_resize.wnd=this;
						//this.td_resize.style.backgroundColor="#dcdcdc";
						/*this.img_resize=this.d.createElement("img");
							this.img_resize.src="rs.gif";
							this.img_resize.style.position="absolute";
							this.img_resize.style.zIndex="0";*/
						//this.td_resize.appendChild(this.img_resize);
						this.input_resize=this.d.createElement("input");
							this.input_resize.onmousedown=function(ev)
							{
								var e;
								if(this.wnd.maximized && (!this.wnd.minimized))return;
								if(ev)e=ev; else e=this.wnd.fr.contentWindow.event;
								this.wnd.dx=e.clientX-this.wnd.param.p.scrollLeft+getx(this.wnd.fr);
								this.wnd.dy=e.clientY-this.wnd.param.p.scrollTop+gety(this.wnd.fr);
								this.wnd.dw=this.wnd.fr.offsetWidth;
								this.wnd.dh=this.wnd.fr.offsetHeight;
								this.wnd.resizing=true;
								this.wnd.input_resize.style.backgroundImage="";
								if(this.wnd.onbeginresize)this.wnd.onbeginresize(this.wnd);
							};
							this.input_resize.style.border="none";
							//this.input_resize.style.fontSize="1px";
							//this.input_resize.style.position="absolute";
							this.input_resize.style.zIndex="2";
							this.input_resize.style.width="16px";
							this.input_resize.style.height="16px";
							this.input_resize.style.cursor="se-resize";
							//this.input_resize.style.marginTop="-5px";
							this.input_resize.style.backgroundImage="url(rs.gif)";
							this.input_resize.style.backgroundColor="transparent";
							this.input_resize.rs_img_mode=RS_IMG_MODE_BG;
							if(typeof wnd.rs_img == "object")
							{
								window.rs_img[window.rs_img.length]=this.input_resize;
							}
							else
							{
								window.rs_img=[this.input_resize];
							}
							//this.input_resize.style.backgroundRepeat="no-repeat";
							this.input_resize.wnd=this;
							this.input_resize.readOnly=true;
							//this.td_resize.innerHTML='<DIV style="FONT-SIZE: 0px"><DIV style="MARGIN-TOP: 10px; MARGIN-LEFT: 0px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 9px; MARGIN-LEFT: 1px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 8px; MARGIN-LEFT: 2px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 7px; MARGIN-LEFT: 3px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 6px; MARGIN-LEFT: 4px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 5px; MARGIN-LEFT: 5px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 4px; MARGIN-LEFT: 6px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 3px; MARGIN-LEFT: 7px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 2px; MARGIN-LEFT: 8px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 1px; MARGIN-LEFT: 9px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 0px; MARGIN-LEFT: 10px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: -1px; MARGIN-LEFT: 11px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: -2px; MARGIN-LEFT: 12px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: -3px; MARGIN-LEFT: 13px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: -4px; MARGIN-LEFT: 14px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: -5px; MARGIN-LEFT: 15px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 10px; MARGIN-LEFT: 5px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 9px; MARGIN-LEFT: 6px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 8px; MARGIN-LEFT: 7px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 7px; MARGIN-LEFT: 8px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 6px; MARGIN-LEFT: 9px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 5px; MARGIN-LEFT: 10px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 4px; MARGIN-LEFT: 11px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 3px; MARGIN-LEFT: 12px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 2px; MARGIN-LEFT: 13px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 1px; MARGIN-LEFT: 14px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 0px; MARGIN-LEFT: 15px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 10px; MARGIN-LEFT: 10px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 9px; MARGIN-LEFT: 11px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 8px; MARGIN-LEFT: 12px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 7px; MARGIN-LEFT: 13px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 6px; MARGIN-LEFT: 14px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 5px; MARGIN-LEFT: 15px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 10px; MARGIN-LEFT: 15px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV></DIV>';
							this.input_resize.onmousemove=function(ev)
							{
								var e;
								if(ev)
									e=ev;
								else
									e=this.wnd.fr.contentWindow.event;
								if(this.wnd.resizing)
								{
									//return;
									//e.cancelBubble=true;
									//this.style.cursor="se-resize";
									//this.wnd.d.selection.empty();
									var ow=e.clientX-this.wnd.param.p.scrollLeft+getx(this.wnd.fr)-this.wnd.dx;
									var oh=e.clientY-this.wnd.param.p.scrollTop+gety(this.wnd.fr)-this.wnd.dy;
									var nw=this.wnd.dw+ow;
									var nh=this.wnd.dh+oh;
									if(nw<150)nw=150;
									if(nh<70)nh=70;
									//if(nx>this.wnd.param.p.offsetWidth-20)nx=this.wnd.param.p.offsetWidth-20;
									//if(ny>this.wnd.param.p.offsetHeight-20)ny=this.wnd.param.p.offsetHeight-20;
									this.wnd.fr.style.width=nw+"px";
									this.wnd.fr.style.height=nh+"px";
									if(this.wnd.onresize)this.wnd.onresize(this.wnd);
								}
								//return false;
							};
							//this.input_resize.style.position="absolute";
							//this.input_resize.ondrag=function(){return false;};
							//this.input_resize.src="rs.gif";
							this.resizehandle=this.input_resize;
						this.td_resize.appendChild(this.input_resize);
					this.tr_status.appendChild(this.td_resize);
				//this.tb.appendChild(this.tr_status);
				this.t_status=this.d.createElement("table");
				this.t_status.cellSpacing="0";
				this.t_status.cellPadding="0";
				this.t_status.style.width="100%";
				this.t_status.style.height="100%";
				this.tb_status=this.d.createElement("tbody");
				
				//this.td_status2.style.border="#000000 1px solid";
				//this.td_status2.colSpan="2";
				
				this.tb_status.appendChild(this.tr_status);
				this.t_status.appendChild(this.tb_status);
				this.tr_status2=this.d.createElement("tr");
				this.td_status2=this.d.createElement("td");
				this.td_status2.appendChild(this.t_status);
				this.td_status2.style.height="18px";
				this.tr_status2.appendChild(this.td_status2);
				this.tb.appendChild(this.tr_status2);

			this.t.appendChild(this.tb);
		this.b.appendChild(this.t);
		this.t.style.borderLeft="#eaeaea 2px solid";
		this.t.style.borderTop="#eaeaea 2px solid";
		this.t.style.borderRight="#808080 2px solid";
		this.t.style.borderBottom="#808080 2px solid";
		//this.t.style.border="#000000 1px solid";
		//this.b.innerHTML+="aaaaaaaaaaaaa";
		this.do_flash=function()
		{
		    if(this.f_intv)return;
		    this.f_ind=0;
		    this.f_t_c=this.input_title.style.backgroundColor;
		    this.__tmp_t_d=this.tr_title2.style.display;
		    this.__tmp_s_d=this.tr_status2.style.display;
		    var f=new Function("\
		    var o=window.obj["+this.ind.toString()+"];\
		    o.f_ind++;\
		    if(o.f_ind>5){o.param.wnd.clearInterval(o.f_intv);o.f_intv=null;o.tr_title2.style.display=o.__tmp_t_d;o.tr_status2.style.display=o.__tmp_s_d;return;}\
		    o.input_title.style.backgroundColor=((o.f_ind%2)==0)?'#999999':'#0a246a';\
		    ");
		    this.tr_title2.style.display="";
		    this.tr_status2.style.display="";
		    this.f_intv=this.param.wnd.setInterval(f,60);
		    f();
		};
		this.ready=true;
		this.activate();
		this.b.focus();
		if(this.param.cb)this.param.cb(this);
	};
	this.intvf=new Function
	("\
		var o=window.obj["+this.ind.toString()+"];\
		/*try\
		{*/\
			if(o.fr)\
			{\
				if(o.fr.contentWindow)\
				{\
					if(o.fr.contentWindow.document)\
					{\
						if(o.fr.contentWindow.document.body)\
						{\
							o.param.wnd.clearInterval(o.intv);\
							o.tmpf();\
						}\
					}\
				}\
			}\
		/*}\
		catch(exc)\
		{alert(exc.message);}*/\
	");
	this.intv=wnd.setInterval(this.intvf,100);
	
};
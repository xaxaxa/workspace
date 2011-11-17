

var RS_IMG_MODE_BG=1;

var RS_IMG_MODE_IMG=2;

var RS_IMG_MODE_ALL=3;

// JScript File

function getx(oElement)

{

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

	var ifr=document.getElementsByTagName("div");

	var i;

	for(i=0;i<ifr.length;i++)

	{

		if(ifr[i].wnd!=null)

		{

			var tmp=parseInt(ifr[i].style.zIndex);

			if((!isNaN(tmp)) && (tmp!=null))

			{

				if(tmp>ret)ret=tmp;

			}

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

	}

}

function wnd2_helper(wnd)

{

	this.param=new Object();

	this.param.wnd=wnd;

	wnd.wnd2_helper_i=this;

	wnd.document.body.onmousemove=function(ev)

	{

		var e;

		if(ev)

			e=ev;

		else

			e=window.event;

		if(window.wnd2_moving!=null)

		{

			var w=window.wnd2_moving;

			var ox=e.clientX-w.md_x;

			var oy=e.clientY-w.md_y;

			var nx=w.d_x+ox;

			var ny=w.d_y+oy;

			if(nx<0)nx=0;

			if(ny<0)ny=0;

			if(nx>w.param.p.offsetWidth-20)nx=w.param.p.offsetWidth-20;

			if(ny>w.param.p.offsetHeight-20)ny=w.param.p.offsetHeight-20;

			w.fr.style.top=ny+"px";

			w.fr.style.left=nx+"px";

		}else if(window.wnd2_resizing!=null)

		{

			var w=window.wnd2_resizing;

			var ow=e.clientX-w.param.p.scrollLeft+getx(w.fr)-w.dx;

			var oh=e.clientY-w.param.p.scrollTop+gety(w.fr)-w.dy;

			var nw=w.dw+ow;

			var nh=w.dh+oh;

			if(nw<150)nw=150;

			if(nh<70)nh=70;

			//if(nx>this.wnd.param.p.offsetWidth-20)nx=this.wnd.param.p.offsetWidth-20;

			//if(ny>this.wnd.param.p.offsetHeight-20)ny=this.wnd.param.p.offsetHeight-20;

			w.fr.style.width=nw+"px";

			w.fr.style.height=nh+"px";

			if(w.onresize)w.onresize(w);

		}

	};

	wnd.document.body.onmouseup=function(){window.wnd2_moving=null;window.wnd2_resizing=null;};

	wnd.document.body.onselectstart=function(){return (window.wnd2_moving==null && window.wnd2_resizing==null);};

	wnd.document.addEventListener("touchmove",function(ev)

	{

		var e;

		if(ev)

			e=ev;

		else

			e=window.event;
e.preventDefault();
		if(window.wnd2_moving!=null)

		{

			if(e.touches.length>1)return;

			

			var w=window.wnd2_moving;

			var ox=e.touches[0].pageX-w.md_x;

			var oy=e.touches[0].pageY-w.md_y;

			var nx=w.d_x+ox;

			var ny=w.d_y+oy;

			if(nx<0)nx=0;

			if(ny<0)ny=0;

			if(nx>w.param.p.offsetWidth-20)nx=w.param.p.offsetWidth-20;

			if(ny>w.param.p.offsetHeight-20)ny=w.param.p.offsetHeight-20;

			w.fr.style.top=ny+"px";

			w.fr.style.left=nx+"px";

		}else if(window.wnd2_resizing!=null)

		{

			var w=window.wnd2_resizing;

			var ow=e.touches[0].pageX-w.param.p.scrollLeft+getx(w.fr)-w.dx;

			var oh=e.touches[0].pageY-w.param.p.scrollTop+gety(w.fr)-w.dy;

			var nw=w.dw+ow;

			var nh=w.dh+oh;

			if(nw<150)nw=150;

			if(nh<70)nh=70;

			//if(nx>this.wnd.param.p.offsetWidth-20)nx=this.wnd.param.p.offsetWidth-20;

			//if(ny>this.wnd.param.p.offsetHeight-20)ny=this.wnd.param.p.offsetHeight-20;

			w.fr.style.width=nw+"px";

			w.fr.style.height=nh+"px";

			if(w.onresize)w.onresize(w);

		}

	},false);

	wnd.document.addEventListener("touchend",function(ev){window.wnd2_moving=null;window.wnd2_resizing=null;},false);

}

function wnd2(x,y,w,h,wnd,p,cb,title,controlbox)

{

	this.type="window";

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

	this.param=new Object();

	if(x==null)x=0;

	if(y==null)y=0;

	if(w==null)w=300;

	if(h==null)h=250;

	if(wnd==null)wnd=window;

	if(title==null)title="Form"+(this.ind+1).toString();

	var btn_style="width:20px;height:20px;display:table-cell;border:solid 1px #999;background-position: center center;background-repeat:no-repeat;";

	if(controlbox==null)

		controlbox=

		[

			["span","",btn_style+"background-image:url(\"data:image/png,%89PNG%0D%0A%1A%0A%00%00%00%0DIHDR%00%00%00%0A%00%00%00%0B%08%04%00%00%00%ECg%D4%93%00%00%00%01sRGB%00%AE%CE%1C%E9%00%00%00%02bKGD%00%FF%87%8F%CC%BF%00%00%00%09pHYs%00%00%0E%C4%00%00%0E%C4%01%95%2B%0E%1B%00%00%00%07tIME%07%DB%03%0D%0A%0E%12aJ%3DQ%00%00%00%16IDAT%08%D7c%60%18%C2%80%91%81%81%E1%3F%BA%18%136%95%00'%2B%01%04%8F%12%E1%23%00%00%00%00IEND%AEB%60%82\")",20,"minimize/restore","w.minimizerestore();"],

			["span","",btn_style+"background-image:url(\"data:image/png,%89PNG%0D%0A%1A%0A%00%00%00%0DIHDR%00%00%00%0C%00%00%00%0B%08%06%00%00%00Kpl_%00%00%00%01sRGB%00%AE%CE%1C%E9%00%00%00%06bKGD%00%FF%00%FF%00%FF%A0%BD%A7%93%00%00%00%09pHYs%00%00%0E%C4%00%00%0E%C4%01%95%2B%0E%1B%00%00%00%07tIME%07%DB%03%0D%0A%11-%1Av%1E%F2%00%00%00%2CIDAT(%CF%ED%91%B1%0D%000%0C%C2L%D4%FF_v%5E%08%7B%3D%83%2CA%00)%18J%1E%80%9AK8%89%B5%E1%17N%D3%B6O%2F%00x%06%151%9E%B1%A6%00%00%00%00IEND%AEB%60%82\")",20,"maximize/restore","w.maximizerestore();"],

			["span","",btn_style+"background-image:url(\"data:image/png,%89PNG%0D%0A%1A%0A%00%00%00%0DIHDR%00%00%00%0A%00%00%00%0B%08%04%00%00%00%ECg%D4%93%00%00%00%01sRGB%00%AE%CE%1C%E9%00%00%00%02bKGD%00%FF%87%8F%CC%BF%00%00%00%09pHYs%00%00%0E%C4%00%00%0E%C4%01%95%2B%0E%1B%00%00%00%07tIME%07%DB%03%0D%0A%043%D7%CC%C5%85%00%00%00%3EIDAT%08%D7u%8D%C9%11%00%20%0C%02%97%F4%DF3%3E%14%A33%CA%8B%23%01a%B0%D8%90%A1B%DAZfDB%F9%B8%9B%B0%EAn%9C%AAB%DA%FA%0F%DD%9D2%08%F7c%E2%3A%BB%D2%FD%1C%1A%B8%C9%19%11%D6%D1%F0%94%00%00%00%00IEND%AEB%60%82\")",20,"close","w.close();"]

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

	this.fr=wnd.document.createElement("div");

	this.fr.wnd=this;

	this.fr.style.border="none";

	this.fr.style.position="fixed";

	this.fr.style.top=y+"px";

	this.fr.style.left=x+"px";

	this.fr.style.width=w+"px";

	this.fr.style.height=h+"px";

	this.fr.style.zIndex=get_zi();

	this.fr.addEventListener("touchstart",function(ev)

	{

		this.wnd.activate();

	},false);

	//this.fr.src="about:blank";

	//p.appendChild(this.fr);

	this.marginTop=0;

	this.marginRight=0;

	this.marginBottom=0;

	this.marginLeft=0;

	this.visible=false;

	this.maximized=false;

	this.minimized=false;

	this.show=function()

	{

		if(this.minimized)this.restore();

		if(this.visible)return;

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

			this.param.p.appendChild(this.fr);

			this.visible=true;

			if(this.param.wnd.wnd_on_show)

			{

				return this.param.wnd.wnd_on_show(this);

			}

		}

		catch(exc){}

	};

	this.close=function()

	{

		if(!this.visible)return;

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

			this.param.p.removeChild(this.fr);

			this.visible=false;

			if(this.param.wnd.wnd_on_close)

			{

				return this.param.wnd.wnd_on_close(this);

			}

		}

		catch(exc){}

	};

	this.maximize=function()

	{

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

		this.fr.style.width="";

		this.fr.style.height="";

		this.fr.style.top=this.marginTop.toString()+"px";

		this.fr.style.left=this.marginLeft.toString()+"px";

		this.fr.style.right=this.marginRight.toString()+"px";

		this.fr.style.bottom=this.marginBottom.toString()+"px";

		this.maximized=true;

		this.minimized=false;

		if(this.param.wnd.wnd_on_maximize)

		{

			return this.param.wnd.wnd_on_maximize(this);

		}

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

		this.fr.style.width="150px";

		this.fr.style.height="20px";

		this.fr.style.top=(this.param.p.offsetHeight-30).toString()+"px";

		this.fr.style.left="0px";

		this.minimized=true;

		if(this.param.wnd.wnd_on_minimize)

		{

			return this.param.wnd.wnd_on_minimize(this);

		}

		//this.maximized=false;

	};

	this.restore=function()

	{

		if(this.param.wnd.wnd_before_restore)

		{

			if(this.param.wnd.wnd_before_restore(this))return false;

		}

		if(this.minimized)

		{

			if(this.maximized)

			{

				this.fr.style.width="";

				this.fr.style.height="";

				this.fr.style.top=this.marginTop.toString()+"px";

				this.fr.style.left=this.marginLeft.toString()+"px";

				this.fr.style.right=this.marginRight.toString()+"px";

				this.fr.style.bottom=this.marginBottom.toString()+"px";

			}

			else

			{

				this.fr.style.top=(this.tmpy-3)+"px";

				this.fr.style.left=(this.tmpx-3)+"px";

				this.fr.style.right="";

				this.fr.style.bottom="";

				this.fr.style.width=this.tmpw+"px";

				this.fr.style.height=this.tmph+"px";

			}

			this.minimized=false;

		}

		else if(this.maximized)

		{

			this.fr.style.top=(this.tmpy-3)+"px";

			this.fr.style.left=(this.tmpx-3)+"px";

			this.fr.style.right="";

			this.fr.style.bottom="";

			this.fr.style.width=this.tmpw+"px";

			this.fr.style.height=this.tmph+"px";

			this.maximized=false;

			this.minimized=false;

		}

		if(this.param.wnd.wnd_on_restore)

		{

			return this.param.wnd.wnd_on_restore(this);

		}

	};

	this.activate=function()

	{

		if(this.param.wnd.wnd_do_activate)

		{

			if(this.param.wnd.wnd_do_activate(this))

			{

				if(this.param.wnd.wnd_on_activate)

				{

					return this.param.wnd.wnd_on_activate(this);

				}

				return;

			}

		}

		if(this.minimized)this.restore();

		rearrange_zi();

		this.fr.style.zIndex=get_zi()+1;

		if(this.param.wnd.wnd_on_activate)

		{

			return this.param.wnd.wnd_on_activate(this);

		}

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

	this.tmpf=function()

	{

		var tmpf1=function(){if(this.fn){this.fn(this.wnd);}};

		this.b=this.fr;//.contentWindow.document.body;

		this.d=this.param.wnd.document;//this.fr.contentWindow.document;

		/*this.b.topMargin="0";

		this.b.leftMargin="0";

		this.b.bottomMargin="0";

		this.b.rightMargin="0";*/

		//this.b.innerHTML="";

		this.b.style.overflow="hidden";

		//this.b.scroll="no";

		this.b.style.border="none";

		this.b.wnd=this;

		//this.fr.contentWindow.wnd=this;

		/*this.fr.contentWindow.onfocus=function()

		{

			this.wnd.activate();

		};

		this.fr.onfocus=function()

		{

			this.wnd.activate();

		};*/

		this.b.onmousedown=function()

		{

			this.wnd.activate();

		};

		this.b.onmouseup=function()

		{

			this.wnd.mov=false;

		};

		this.b.onmousemove=function(ev)

		{

			if(this.wnd.param.wnd.wnd2_helper_i!=null)return;

			var e;

			if(ev)

				e=ev;

			else

				e=this.wnd.param.wnd.event;

			if(this.wnd.mov)

			{

				var ox=e.clientX-this.wnd.md_x;

				var oy=e.clientY-this.wnd.md_y;

				var nx=this.wnd.d_x+ox;

				var ny=this.wnd.d_y+oy;

				if(nx<0)nx=0;

				if(ny<0)ny=0;

				if(nx>this.wnd.param.p.offsetWidth-20)nx=this.wnd.param.p.offsetWidth-20;

				if(ny>this.wnd.param.p.offsetHeight-20)ny=this.wnd.param.p.offsetHeight-20;

				this.wnd.fr.style.top=ny+"px";

				this.wnd.fr.style.left=nx+"px";

			}

		};

		this.t=this.d.createElement("table");

			this.t.style.cursor="default";

			this.t.cellSpacing="0";

			this.t.cellPadding="0";

			this.t.style.borderSpacing="0px";

			//this.t.cellPadding="0";

			this.t.style.width="100%";

			this.t.style.height="100%";

			this.t.style.backgroundColor="#d4d0c8";

			this.t.style.position="absolute";

			this.t.style.top="0px";

			this.t.style.left="0px";

			this.tb=this.d.createElement("tbody");

				this.tr_title=this.d.createElement("tr");

					this.td_title=this.d.createElement("td");

						this.td_title.style.height="20px";

						this.td_title.style.backgroundColor="#808080";

						this.td_title.style.color="#ffffff";

						this.td_title.style.paddingLeft="5px";

						this.td_title.innerHTML=this.param.title;

						this.td_title.wnd=this;

						this.td_title.onselectstart=function(){return false;};

						this.td_title.onmousedown=function(ev)

						{

							if(this.wnd.maximized && (!this.wnd.minimized))return;

							var e;

							if(ev)

								e=ev;

							else

								e=this.wnd.param.wnd.event;

							if(e.button!=0)return;

							this.wnd.md_x=e.clientX;

							this.wnd.md_y=e.clientY;

							this.wnd.d_x=getx(this.wnd.fr);

							this.wnd.d_y=gety(this.wnd.fr);

							this.wnd.mov=true;

							this.wnd.param.wnd.wnd2_moving=this.wnd;

						};

						this.td_title.addEventListener("touchstart",function(ev)

						{

							if(this.wnd.maximized && (!this.wnd.minimized))return;

							var e;

							if(ev)

								e=ev;

							else

								e=this.wnd.param.wnd.event;

							if(e.touches.length>1)return;

							this.wnd.activate();

							e.preventDefault();

							this.wnd.md_x=e.touches[0].pageX;

							this.wnd.md_y=e.touches[0].pageY;

							this.wnd.d_x=getx(this.wnd.fr);

							this.wnd.d_y=gety(this.wnd.fr);

							this.wnd.mov=true;

							this.wnd.param.wnd.wnd2_moving=this.wnd;

						},false);

						

						this.settitle=function(v)

						{

							this.td_title.textContent=v;

							if(this.ontitlechange)this.ontitlechange(this);

							if(this.param.wnd.wnd_on_titlechange)this.param.wnd.wnd_on_titlechange(this);

						};

					this.tr_title.appendChild(this.td_title);

					

					var i;

					this.buttons=[];

					this.buttons_td=[];

					for(i=0;i<this.param.controlbox.length;i++)

					{

						this.buttons_td[i]=this.d.createElement("td");

							this.buttons_td[i].style.width=this.param.controlbox[i][3]+"px";

							this.buttons[i]=this.d.createElement(this.param.controlbox[i][0]);

								this.buttons[i].innerHTML=this.param.controlbox[i][1];

								this.buttons[i].style.cssText=this.param.controlbox[i][2];

								this.buttons[i].title=this.param.controlbox[i][4];

								this.buttons[i].alt=this.param.controlbox[i][4];

								this.buttons[i].fn=this.param.controlbox[i][5];

								if(typeof this.buttons[i].fn == "string")this.buttons[i].fn=new Function("w",this.buttons[i].fn);

								this.buttons[i].wnd=this;

								this.buttons[i].onclick=tmpf1;

							this.buttons_td[i].appendChild(this.buttons[i]);

						this.tr_title.appendChild(this.buttons_td[i]);

					}

				this.tb.appendChild(this.tr_title);

				this.tr_content=this.d.createElement("tr");

					this.td_content=this.d.createElement("td");

						this.td_content.colSpan=this.param.controlbox.length+1;

						this.td_content.vAlign="top";

							this.div_content=this.d.createElement("div");

								this.div_content.style.cssText="position:absolute;top: 22px; left:1px; right:1px; bottom:20px";

								//this.div_content.innerHTML="<button style=\"position:absolute; bottom: 0px;\"></button>";

							this.td_content.appendChild(this.div_content);

						//this.td_content.innerHTML=" ";

					this.tr_content.appendChild(this.td_content);

				this.tb.appendChild(this.tr_content);

				this.tr_status=this.d.createElement("tr");

					this.td_status=this.d.createElement("td");

						this.td_status.style.height="18px";

						this.td_status.style.fontSize="13px";

						this.td_status.style.fontFamily="Arial";

						//this.td_status.colSpan=this.param.controlbox.length;

						this.td_status.style.overFlow="hidden";

						//this.td_status.colSpan="4";

						/*this.input_status=this.d.createElement("span");

							//this.input_status.style.borderTop="#808080 1px solid";

							//this.input_status.style.borderLeft="#808080 1px solid";

							//this.input_status.style.borderBottom="#eaeaea 1px solid";

							//this.input_status.style.borderRight="#eaeaea 1px solid";

							//this.input_status.style.border="none";

							this.input_status.style.borderTop="solid 1px #666";

							this.input_status.style.borderLeft="none";

							this.input_status.style.borderBottom="none";

							this.input_status.style.borderRight="solid 1px #666";

							this.input_status.style.width="100%";

							this.input_status.style.height="100%";

							this.input_status.style.display="block";

							this.input_status.readOnly=true;

							this.input_status.style.backgroundColor="transparent";

							this.input_status.value="";

							this.input_status.style.fontSize="13px";

							this.input_status.style.fontFamily="Arial";

							this.statusbar=this.input_status;

							this.setstatus=function(v)

							{

								this.input_status.textContent=v;

								if(this.onstatuschange)this.onstatuschange(this);

							};

						this.td_status.appendChild(this.input_status);*/

						this.td_status.style.borderTop="solid 1px #666";

						this.td_status.style.borderLeft="none";

						this.td_status.style.borderBottom="none";

						this.td_status.style.borderRight="solid 1px #666";

						this.setstatus=function(v)

						{

							this.td_status.textContent=v;

							if(this.onstatuschange)this.onstatuschange(this);

						};

					this.tr_status.appendChild(this.td_status);

					this.td_resize=this.d.createElement("td");

						//this.td_resize.colSpan="1";

						this.td_resize.style.width="18px";

						this.td_resize.style.overFlow="hidden";

						this.td_resize.wnd=this;

						this.td_resize.onselectstart=function(){return false;};

						//this.td_resize.style.backgroundColor="#dcdcdc";

						/*this.img_resize=this.d.createElement("img");

							this.img_resize.src="rs.gif";

							this.img_resize.style.position="absolute";

							this.img_resize.style.zIndex="0";*/

						//this.td_resize.appendChild(this.img_resize);

						this.input_resize=this.d.createElement("span");

							this.input_resize.onmousedown=function(ev)

							{

								var e;

								if(this.wnd.maximized && (!this.wnd.minimized))return;

								if(ev)e=ev; else e=this.wnd.fr.contentWindow.event;

								if(e.button!=0)return;

								this.wnd.dx=e.clientX-this.wnd.param.p.scrollLeft+getx(this.wnd.fr);

								this.wnd.dy=e.clientY-this.wnd.param.p.scrollTop+gety(this.wnd.fr);

								this.wnd.dw=this.wnd.fr.offsetWidth;

								this.wnd.dh=this.wnd.fr.offsetHeight;

								this.wnd.resizing=true;

								this.wnd.param.wnd.wnd2_resizing=this.wnd;

								//this.wnd.input_resize.style.backgroundImage="";

								if(this.wnd.onbeginresize)this.wnd.onbeginresize(this.wnd);

							};

							this.input_resize.addEventListener("touchstart",function(ev)

							{

								if(this.wnd.maximized && (!this.wnd.minimized))return;

								var e;

								if(ev)

									e=ev;

								else

									e=this.wnd.param.wnd.event;

								if(e.touches.length>1)return;

								this.wnd.activate();

								e.preventDefault();

								this.wnd.dx=e.touches[0].pageX-this.wnd.param.p.scrollLeft+getx(this.wnd.fr);

								this.wnd.dy=e.touches[0].pageY-this.wnd.param.p.scrollTop+gety(this.wnd.fr);

								this.wnd.dw=this.wnd.fr.offsetWidth;

								this.wnd.dh=this.wnd.fr.offsetHeight;

								this.wnd.resizing=true;

								this.wnd.param.wnd.wnd2_resizing=this.wnd;

								//this.wnd.input_resize.style.backgroundImage="";

								if(this.wnd.onbeginresize)this.wnd.onbeginresize(this.wnd);

							},false);

							this.input_resize.style.border="none";

							this.input_resize.style.display="table-cell";

							//this.input_resize.style.fontSize="1px";

							//this.input_resize.style.position="absolute";

							//this.input_resize.style.zIndex="2";

							this.input_resize.style.width="18px";

							this.input_resize.style.height="20px";

							this.input_resize.style.cursor="se-resize";

							this.input_resize.style.backgroundPosition="right bottom";

							this.input_resize.style.backgroundRepeat="no-repeat";

							this.input_resize.style.backgroundImage="url(\"data:image/png,%89PNG%0D%0A%1A%0A%00%00%00%0DIHDR%00%00%00%0C%00%00%00%0C%08%04%00%00%00%FC%7C%94l%00%00%00%01sRGB%00%AE%CE%1C%E9%00%00%00%02bKGD%00%FF%87%8F%CC%BF%00%00%00%09pHYs%00%00%0E%C3%00%00%0E%C3%01%C7o%A8d%00%00%00%07tIME%07%DB%03%0D%12%24%13k%133'%00%00%00AIDAT%18%D3m%CD%BB%01%00%20%08%03%D1%C0B%AE%C2Jl%E8FX%F8%03M%CA%97%E2%A4%83%AD%85rv(g%FC%C7d%7B%8F%CD%AD%1E%97K%23sjT%3E%8D%97W%E3g%87r%06%24(%1B%06%DFu!X%FD%FEwV%00%00%00%00IEND%AEB%60%82\")";

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

							//this.input_resize.readOnly=true;

							this.input_resize.onselectstart=function(){return false;};

							//this.td_resize.innerHTML='<DIV style="FONT-SIZE: 0px"><DIV style="MARGIN-TOP: 10px; MARGIN-LEFT: 0px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 9px; MARGIN-LEFT: 1px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 8px; MARGIN-LEFT: 2px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 7px; MARGIN-LEFT: 3px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 6px; MARGIN-LEFT: 4px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 5px; MARGIN-LEFT: 5px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 4px; MARGIN-LEFT: 6px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 3px; MARGIN-LEFT: 7px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 2px; MARGIN-LEFT: 8px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 1px; MARGIN-LEFT: 9px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 0px; MARGIN-LEFT: 10px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: -1px; MARGIN-LEFT: 11px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: -2px; MARGIN-LEFT: 12px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: -3px; MARGIN-LEFT: 13px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: -4px; MARGIN-LEFT: 14px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: -5px; MARGIN-LEFT: 15px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 10px; MARGIN-LEFT: 5px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 9px; MARGIN-LEFT: 6px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 8px; MARGIN-LEFT: 7px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 7px; MARGIN-LEFT: 8px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 6px; MARGIN-LEFT: 9px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 5px; MARGIN-LEFT: 10px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 4px; MARGIN-LEFT: 11px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 3px; MARGIN-LEFT: 12px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 2px; MARGIN-LEFT: 13px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 1px; MARGIN-LEFT: 14px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 0px; MARGIN-LEFT: 15px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 10px; MARGIN-LEFT: 10px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 9px; MARGIN-LEFT: 11px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 8px; MARGIN-LEFT: 12px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 7px; MARGIN-LEFT: 13px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 6px; MARGIN-LEFT: 14px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 5px; MARGIN-LEFT: 15px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV><DIV style="MARGIN-TOP: 10px; MARGIN-LEFT: 15px; OVERFLOW: hidden; WIDTH: 1px; POSITION: absolute; HEIGHT: 1px; BACKGROUND-COLOR: #000000"></DIV></DIV>';

							this.input_resize.onmousemove=function(ev)

							{

								if(this.wnd.param.wnd.wnd2_helper_i!=null)return;

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

				this.t_status.style.borderSpacing="0px";

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

				this.td_status2.colSpan=this.param.controlbox.length+1;

				this.tb.appendChild(this.tr_status2);

			this.t.appendChild(this.tb);

		this.b.appendChild(this.t);

		/*this.t.style.borderLeft="#eaeaea 2px solid";

		this.t.style.borderTop="#eaeaea 2px solid";

		this.t.style.borderRight="#808080 2px solid";

		this.t.style.borderBottom="#808080 2px solid";*/

		this.t.style.border="solid 1px #000";

		//this.t.style.border="#000000 1px solid";

		//this.b.innerHTML+="aaaaaaaaaaaaa";

		this.ready=true;

		this.activate();

		if(this.param.cb)this.param.cb(this);

	};

	/*this.intvf=new Function

	("\

		var o=window.obj["+this.ind.toString()+"];\

		try\

		{\

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

		}\

		catch(exc)\

		{alert(exc.message);}\

	");

	this.intv=wnd.setInterval(this.intvf,10);*/

	this.tmpf();

};



function browser(w)

{

	this.w=w;

	w.settitle("Browser");

	this.toolbar=w.param.wnd.document.createElement("div");

	this.toolbar.style.cssText="position:absolute;top:0px;left:0px;right:0px;height:23px;border-bottom:solid 1px #666;padding:0px;";

	this.div=w.param.wnd.document.createElement("div");

	this.div.style.cssText="position:absolute;bottom:0px;left:0px;right:0px;top:24px;";

	this.ifr=w.param.wnd.document.createElement("iframe");

	this.ifr.style.cssText="width:100%;height:100%;border:none;margin:0px;padding:0px;";

	this.ifr.browser=this;

	this.ifr.onload=function()

	{

		this.browser.w.settitle(this.contentWindow.title);

	};

	

	this.inp_addr=w.param.wnd.document.createElement("input");

	this.inp_addr.style.cssText="position:absolute;top:0px;left:0px;right:0px;height:18px;border:solid 1px #999;margin:0px;padding:1px;";

	this.inp_addr.browser=this;

	this.inp_addr.onkeydown=function(ev)

	{

		var e;

		if(ev)

			e=ev;

		else

			e=window.event;

		if(e.keyCode==13)

		{

			this.browser.loadpage(this.value);

			return false;

		}

	};

	//this.btn_go=w.param.wnd.document.createElement("span");

	//this.btn_go.style.cssText="position:absolute;top:0px;width:38px;right:0px;height:18px;border:solid 1px #999;margin:0px;padding:1px;";

	

	this.toolbar.appendChild(this.inp_addr);

	//this.toolbar.appendChild(this.btn_go);

	

	this.div.appendChild(this.ifr);

	w.div_content.appendChild(this.toolbar);

	w.div_content.appendChild(this.div);

	this.loadpage=function(url)

	{

		this.ifr.src=url;

	};

}
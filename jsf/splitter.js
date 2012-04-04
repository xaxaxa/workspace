// JScript File

if(__FILE_PANEL_JS==null)
{
	alert("\"panel.js\" is required.");
}
function splitter(e,rs_e,inv,vertical,dock,wnd,min)
{
	if(wnd==null)wnd=window;
	if(dock==null)
	{
		if(vertical)
			dock=DOCK_LEFT;
		else
			dock=DOCK_TOP;
	}
	if(min==null)min=0;
	this.minimum=min;
	this.e=e;
	this.rs_e=rs_e;
	this.inv=inv;
	this.wnd=wnd;
	this.vertical=vertical;
	this.element=wnd.document.createElement("input");
	this.element.type="text";
	this.element.style.backgroundColor="transparent";
	this.element.style.fontSize="1px";
	this.element.style.position="absolute";
	this.element.dock=dock;
	this.element.readOnly=true;
	this.element.value="";
	this.element.style.border="none";
	this.element.style.cursor=vertical?"e-resize":"s-resize";
	this.element.s=this;
	this.element.onmousedown=function(ev)
	{
		var e;
		if(ev)
			e=ev;
		else
			e=this.s.wnd.event;
		this.s.dx=e.clientX;
		this.s.dy=e.clientY;
		this.s.dw=this.s.rs_e.offsetWidth;
		this.s.dh=this.s.rs_e.offsetHeight;
		this.s.rs=true;
		if(this.s.wnd.on_beginresize)
		{
			this.s.wnd.on_beginresize(this);
		}
	};
	this.element.onmousemove=function(ev)
	{
		if(!this.s.rs)return;
		var e;
		if(ev)
			e=ev;
		else
			e=this.s.wnd.event;
		var dif_x=e.clientX-this.s.dx;
		var dif_y=e.clientY-this.s.dy;
		var p=getp(this.s.e);
		var mt,mb,ml,mr;
		mt=((this.s.e.margintop==null) ? 0 : this.s.e.margintop);
		mb=((this.s.e.marginbottom==null) ? 0 : this.s.e.marginbottom);
		ml=((this.s.e.marginleft==null) ? 0 : this.s.e.marginleft);
		mr=((this.s.e.marginright==null) ? 0 : this.s.e.marginright);
		if(this.s.e.marginh!=null)
		{
			ml=this.s.e.marginh;
			mr=this.s.e.marginh;
		}
		if(this.s.e.marginv!=null)
		{
			mt=this.s.e.marginv;
			mb=this.s.e.marginv;
		}
		if(this.s.e.margin!=null)
		{
			mt=this.s.e.margin;
			mb=this.s.e.margin;
			ml=this.s.e.margin;
			mr=this.s.e.margin;
		}
		if(this.s.inv)
		{
			dif_x=-dif_x;
			dif_y=-dif_y;
		}
		var nv;
		if(this.s.vertical)
		{
			nv=this.s.dw+dif_x;
			if(nv<this.s.minimum)nv=this.s.minimum;
			/*if(this.s.inv)
			{
				var r=this.s.e.offsetLeft+this.s.e.offsetWidth;
				if(nv>r-ml-6)nv=r-ml-6;
			}
			else
			{
				if(nv>p.offsetWidth-this.s.e.offsetLeft-mr-6)nv=p.offsetWidth-this.s.e.offsetLeft-mr-6;
			}*/
			this.s.rs_e.style.width=nv+"px";
			//alert(nv);
		}
		else
		{
			nv=this.s.dh+dif_y;
			if(nv<this.s.minimum)nv=this.s.minimum;
			/*if(this.s.inv)
			{
				var r=this.s.e.offsetTop+this.s.e.offsetHeight;
				if(nv>r-mt-6)nv=r-mt-6;
			}
			else
			{
				if(nv>p.offsetHeight-this.s.e.offsetTop-mb-6)nv=p.offsetHeight-this.s.e.offsetTop-mb-6;
			}*/
			this.s.rs_e.style.height=nv+"px";
		}
		if(this.s.onresize)this.s.onresize(this.s);
	};
	this.element.onmouseup=function()
	{
		this.s.rs=false;
		if(this.s.wnd.on_endresize)
		{
			this.s.wnd.on_endresize(this);
		}
	};
	if(vertical)
	{
		this.element.style.width="8px";
	}
	else
	{
		this.element.style.height="8px";
	}
	this.e.appendChild(this.element);
	//layout_element(this.element,this.element.dock);
}

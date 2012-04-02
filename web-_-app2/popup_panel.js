// JScript File
//resources: transparent.gif
function popup_panel(e,w,h,wnd)
{
	this.clientarea=wnd.document.createElement("input");
	this.clientarea.style.display="none";
	this.clientarea.style.position="absolute";
	this.clientarea.style.zIndex="999";
	if(w!=null) this.clientarea.style.width=w.toString()+"px";
	if(h!=null) this.clientarea.style.height=h.toString()+"px";
	this.clientarea.style.background="url(transparent.gif)";
	this.clientarea.readOnly=true;
	this.clientarea.style.border="none";
	this.clientarea.style.fontSize="1px";
	this.clientarea.style.cursor="default";
	
	this.displayarea=wnd.document.createElement("div");
	this.displayarea.style.display="none";
	this.displayarea.style.position="absolute";
	this.displayarea.style.zIndex="998";
	if(w!=null) this.displayarea.style.width=w.toString()+"px";
	if(h!=null) this.displayarea.style.height=h.toString()+"px";
	insertAdjacentElement(e,"afterBegin",this.clientarea);
	insertAdjacentElement(e,"afterBegin",this.displayarea);
	this.show=function(x,y)
	{
		this.clientarea.style.marginTop=y.toString()+"px";
		this.clientarea.style.marginLeft=x.toString()+"px";
		
		this.displayarea.style.marginTop=y.toString()+"px";
		this.displayarea.style.marginLeft=x.toString()+"px";
		
		this.displayarea.style.display="";
		
		this.clientarea.style.width=this.displayarea.offsetWidth.toString()+"px";
		this.clientarea.style.height=this.displayarea.offsetHeight.toString()+"px";
		
		this.clientarea.style.display="";
		this.clientarea.focus();
	};
	this.hide=function()
	{
		this.displayarea.style.display="none";
		this.clientarea.style.display="none";
	};
	this.get_element=function(x,y)
	{
		var i;
		var l=this.displayarea.childNodes;
		var _t;
		var _l;
		for(i=0;i<l.length;i++)
		{
			_t=l[i].offsetTop;
			if(l===null){}
			else
			{
				_l=l[i].offsetLeft;
				if(x>=_l && y>=_t && x<(_l+l[i].offsetWidth) && y<(_t+l[i].offsetHeight))
				{
					return l[i];
				}
			}
		}
	};
	this.clientarea.__p=this;
	this.clientarea.onblur=function()
	{
		this.__p.hide();
	};
	this.clientarea.onclick=function(ev)
	{
		var e;
		if(ev)
			e=ev;
		else
			e=event;
		var el=this.__p.get_element(e.clientX,e.clientY);
		if(el)
		{
			if(el.onclick)
			{
				el.onclick();
			}
		}
	};
	this.clientarea.onmousedown=function(ev)
	{
		var e;
		if(ev)
			e=ev;
		else
			e=event;
		var el=this.__p.get_element(e.clientX,e.clientY);
		if(el)
		{
			if(el.onmousedown)
			{
				el.onmousedown();
			}
		}
	};
	this.clientarea.onmouseup=function(ev)
	{
		var e;
		if(ev)
			e=ev;
		else
			e=event;
		var el=this.__p.get_element(e.clientX,e.clientY);
		if(el)
		{
			if(el.onmouseup)
			{
				el.onmouseup();
			}
		}
	};
	this.capture=null;
	this.clientarea.onmousemove=function(ev)
	{
		var e;
		if(ev)
			e=ev;
		else
			e=event;
		var el=this.__p.get_element(e.clientX,e.clientY);
		if(el != this.__p.capture)
		{
			if(this.__p.capture)
			{
				if(this.__p.capture.onmouseout)
				{
					this.__p.capture.onmouseout();
				}
			}
			if(el)
			{
				if(el.onmouseover)
				{
					el.onmouseover();
				}
			}
			this.__p.capture=el;
		}
	};
	this.clientarea.onmouseover=this.clientarea.onmousemove;
	this.clientarea.onmouseout=this.clientarea.onmousemove;
}
/*	items=
	[//	text			onclick
		["xaxaxa",		"alert('xaxaxa');"],
		["aaaaaaa",		"alert('aaaaaaaaaaaaaaaaaaaaaaaa');"]
	]
*/
function popup_menu(p,items,wnd)
{
	this.panel=new popup_panel(p,100,null,wnd);
	var i;
	var tmp1;
	var tmp2;
	var tmpf1=function(){this.style.backgroundColor="#B6BDD2";this.style.border="#0A246A 1px solid";this.style.color="";};
	var tmpf2=function(){this.style.backgroundColor="";this.style.border="#F9F8F7 1px solid";this.style.color="";};
	for(i=0;i<items.length;i++)
	{
		tmp1=wnd.document.createElement("div");
		tmp1.style.width="100%";
		tmp1.style.paddingLeft="5px";
		tmp1.innerHTML=items[i][0];
		tmp1.onclick=items[i][1];
		tmp1.onmouseover=tmpf1;
		tmp1.onmouseout=tmpf2;
		tmp1.style.border="#F9F8F7 1px solid";
		tmp1.style.height="22px";
		tmp1.style.paddingTop="2px";
		this.panel.displayarea.appendChild(tmp1);
	}
	this.panel.displayarea.style.backgroundColor="#F9F8F7";
	/*this.panel.displayarea.style.borderTop="#eaeaea 2px solid";
	this.panel.displayarea.style.borderLeft="#eaeaea 2px solid";
	this.panel.displayarea.style.borderRight="#808080 2px solid";
	this.panel.displayarea.style.borderBottom="#808080 2px solid";*/
	this.panel.displayarea.style.border="#000000 1px solid";
	//this.panel.displayarea.style.backgroundColor="#d4d0c8";
	this.panel.displayarea.style.fontFamily="Arial";
	this.panel.displayarea.style.fontSize="14px";
	this.show=function(x,y)
	{
		this.panel.show(x,y);
	};
	this.hide=function()
	{
		this.panel.hide();
	};
}
function jsw(code)
{
	if(typeof window.__jsw == "object")
	{
		window.__jsw[window.__jsw.length] = this;
		this.ind = window.__jsw.length-1;
	}
	else
	{
		window.__jsw = [];
		window.__jsw[0] = this;
		this.ind=0;
	}
	this.param=new Object();
	this.param.code=code;
	this.obj=new (new Function(code));
	this.run=function(fn,p)
	{
		this.tmpf = this.obj[fn];
		this.tmpp=p;
		this.tmpstr = "(";
		var i;
		for(i = 0; i < p.length; i ++)
		{
			if(i == 0)
			{
				this.tmpstr += "t.tmpp[0]";
			}
			else
			{
				this.tmpstr += ",t.tmpp[" + i + "]";
			}
		}
		this.tmpstr += ")";
		(new Function("var t=window.__jsw[" + this.ind + "];t.tmpf" + this.tmpstr + ";"))();
	}
};
function inherit(obj,type,p)
{
	window.__tmp=this;
	this.tmpp = p;
	this.tmpt=type;
	this.tmpstr = "(";
	var i;
	for(i = 0; i < p.length; i ++)
	{
		if(i == 0)
		{
			this.tmpstr += "t.tmpp[0]";
		}
		else
		{
			this.tmpstr += ",t.tmpp[" + i + "]";
		}
	}
	this.tmpstr += ")";
	(new Function("var t=window.__tmp;t.tmp=new t.tmpt" + this.tmpstr + ";"))();
	//this.tmp=new type();
	var m;
	for(m in tmp)
	{
		obj[m]=tmp[m];
	}
};
function copy_m(o1,o2)
{
	var m;
	for(m in o1)
	{
		o2[m]=o1[m];
	}
};
function frm(p,w,h,x,y,title,doc)
{
	if(w==null)w=300;
	if(h==null)h=300;
	if(x==null)x=0;
	if(y==null)y=0;
	this.resizable=true;
	if(doc == null)doc=window.document;
	this._doc=doc;
	if(typeof window.__wnds == "object")
	{
		window.__wnds[window.__wnds.length]=this;
		this.ind=window.__wnds.length-1;
	}
	else
	{
		window.__wnds=[this];
		this.ind=0;
	}
	this.getx=function(oElement)
	{
		var iReturnValue = 0;
		while( oElement != null )
		{
			iReturnValue += oElement.offsetLeft;
			oElement = oElement.offsetParent;
		}
		return iReturnValue-doc.body.scrollLeft;
	};
	this.gety=function(oElement)
	{
		var iReturnValue = 0;
		while( oElement != null )
		{
			iReturnValue += oElement.offsetTop;
			oElement = oElement.offsetParent;
		}
		return iReturnValue-doc.body.scrollTop;
	};
	this.tb=doc.createElement("table");
	if(x != null)this.tb.style.left=x+"px";
	if(y != null)this.tb.style.top=y+"px";
	this.tb.style.width=w;
	this.tb.style.height=h;
	this.tb.width=w;
	this.tb.height=h;
	this.tb.style.border="2px outset";
	this.tb.style.position="absolute";
	this.tb.cellSpacing="0";
	this.tb.cellPadding="0";
	this.tb.style.backgroundColor="#d4d0c8";
	this.tb.border="0";
	this.tb.__t=this;
	p.appendChild(this.tb);
	var t=doc.createElement("tbody");
	this.tb.appendChild(t);
	//t.style.width=w;
	//t.style.height=h;
	var tr1=doc.createElement("tr");
	var tr2=doc.createElement("tr");
	var td1=doc.createElement("td");
	var td2=doc.createElement("td");
	this.area=td2;
	td1.style.height="20px";
	tr1.appendChild(td1);
	tr2.appendChild(td2);
	t.appendChild(tr1);
	t.appendChild(tr2);
	this.__m=doc.createElement("input");
	this.__m.value=title==null?"":title;
	this.__m.style.borderStyle="none";
	this.__m.readOnly=true;
	this.__m.style.cursor="default";
	td1.appendChild(this.__m);
	this.__m.style.width="100%";
	this.__m.style.height="100%";
	this.__m.__wnd=this.tb;
	this.__m.__t=this;
	this.__m.style.backgroundColor="#0a246a";
	this.__m.style.color="#ffffff";
	this.__m.style.fontWeight="bold";
	this.__fr=doc.createElement("input");
	this.__fr.readOnly=true;
	this.__fr.style.cursor="default";
	//this.__fr.style.display="none";
	this.__fr.__wnd=this.tb;
	this.__fr.__t=this;
	this.__fr.style.width=(w-4)+"px";
	this.__fr.style.height="20px";
	this.__fr.style.position="absolute";
	this.__fr.__fr=this.__fr;
	//this.__fr.value="aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
	this.__m.__fr=this.__fr;
	this.tb.zIndex="1";
	this.__fr.zIndex="99";
	this.__fr.style.top=this.tb.offsetTop+2;
	this.__fr.style.left=this.tb.offsetLeft+2;
	this.__fr.style.backgroundColor="transparent";
	this.__fr.style.borderStyle="none";
	this.__m.onselectstart=function()
	{
		try
		{
			this.__fr.focus();
		}
		catch(e)
		{}
	};
	p.appendChild(this.__fr);
	this.__fr.onmousemove=function(e)
	{
		if(this.__t.rs != 0)return;
		var event;
		if(e)
			event=e;
		else
			event=window.event;
		if(this.__t.__rs)
		{
			var t=this.__fr.ot+(event.clientY-this.__fr.oy);
			var l=this.__fr.ol+(event.clientX-this.__fr.ox);
			//alert(t);
			//return;
			if(t<0)t=0;
			if(l<0)l=0;
			this.__fr.style.top=t;
			this.__fr.style.left=l;
			this.focus();
		}
	};
	this.rs=0;
	this.addcontrol=function(c)
	{
		this.area.appendChild(c);
	};
	this.__fr.onmousedown=function(e)
	{
		this.style.cursor="default";
		this.__t.__fr.style.border="#808080 4px solid";//
		this.__t.__fr.style.width=this.__t.tb.offsetWidth;
		this.__t.__fr.style.height=this.__t.tb.offsetHeight;
		this.__t.__fr.style.top=this.__t.tb.offsetTop;
		this.__t.__fr.style.left=this.__t.tb.offsetLeft;
		var event;
		if(e)
			event=e;
		else
			event=window.event;
		this.__fr.ot=this.__fr.offsetTop;
		this.__fr.ol=this.__fr.offsetLeft;
		this.__fr.ox=event.clientX;
		this.__fr.oy=event.clientY;
		this.__t.__rs=true;
		this.__fr.focus();
	};
	this.__fr.onmouseup=function(e)
	{
	};
	this.tb.onmousedown=function(e)
	{
		var event;
		if(e)
			event=e;
		else
			event=window.event;
		var _x = event.clientX-this.__t.getx(this);
		var _y = event.clientY-this.__t.gety(this);
		if(((_x > (this.offsetWidth - 15)) && (_y > (this.offsetHeight - 2))) || ((_x > (this.offsetWidth - 2)) && (_y > (this.offsetHeight - 15))))
		{
			this.__t.rs=1;
			this.__t.__fr.style.cursor="se-resize";
		}
		else if(_x > (this.offsetWidth - 2))
		{
			this.__t.rs=2;
			this.__t.__fr.style.cursor="e-resize";
		}
		else if(_y > (this.offsetHeight - 2))
		{
			this.__t.rs=3;
			this.__t.__fr.style.cursor="s-resize";
		}
		else
		{
			this.__t.rs=0;
			//alert("cx:"+event.clientX+"; getx(e):"+getx(e)+"; e.offsetHeight:"+e.offsetHeight+"; event.clientY:"+event.clientY+"; ");
		}
		if(this.__t.rs != 0)
		{
			this.__t.__fr.style.border="#808080 4px solid";//
			this.__t.__fr.style.width=this.__t.tb.offsetWidth;
			this.__t.__fr.style.height=this.__t.tb.offsetHeight;
			this.__t.__fr.style.top=this.__t.tb.offsetTop;
			this.__t.__fr.style.left=this.__t.tb.offsetLeft;
		}
	};
	this.__mv=document.onmousemove;
	document.onmousemove=new Function("e","\
var t=window.__wnds[" + this.ind + "];\
if(t.__mv)t.__mv();\
var event;\
if(e)\
	event=e;\
else\
	event=window.event;\
	if(!t.resizable)return;\
	var w;\
	var h;\
	w=event.clientX-t.getx(t.__fr);\
	h=event.clientY-t.gety(t.__fr);\
switch(t.rs)\
{\
	case 1:\
		t.__fr.style.width=w<112?112:w;\
		t.__fr.style.height=h<24?24:h;\
		break;\
	case 2:\
		t.__fr.style.width=w<112?112:w;\
		break;\
	case 3:\
		t.__fr.style.height=h<24?24:h;\
		break;\
}\
");
	this.tb.onmousemove=function(e)
	{
		var event;
		if(e)
			event=e;
		else
			event=window.event;
		var _x = event.clientX-this.__t.getx(this);
		var _y = event.clientY-this.__t.gety(this);
		if(((_x > (this.offsetWidth - 15)) && (_y > (this.offsetHeight - 2))) || ((_x > (this.offsetWidth - 2)) && (_y > (this.offsetHeight - 15))))
		{
			this.style.cursor="se-resize";
		}
		else if(_x > (this.offsetWidth - 2))
		{
			this.style.cursor="e-resize";
		}
		else if(_y > (this.offsetHeight - 2))
		{
			this.style.cursor="s-resize";
		}
		else
		{
			this.style.cursor="default";
			//alert("cx:"+event.clientX+"; getx(e):"+getx(e)+"; e.offsetHeight:"+e.offsetHeight+"; event.clientY:"+event.clientY+"; ");
		}
	};
	this.__u=document.onmouseup;
	document.onmouseup=new Function("e","\
var t=window.__wnds[" + this.ind + "];\
if(t.__u)t.__u();\
if(t.__rs || t.rs)\
{\
	t.__fr.style.cursor=\"default\";\
	t.__rs=false;\
	t.tb.style.width=t.__fr.offsetWidth;\
	t.tb.style.height=t.__fr.offsetHeight;\
	t.tb.style.top=t.__fr.offsetTop;\
	t.tb.style.left=t.__fr.offsetLeft;\
	t.__fr.style.top=t.tb.offsetTop+2;\
	t.__fr.style.left=t.tb.offsetLeft+2;\
	t.__fr.style.width=(t.tb.offsetWidth-4)+\"px\";\
	t.__fr.style.height=\"20px\";\
	t.__fr.style.borderStyle=\"none\";\
}\
t.rs=0;\
");
	this.getx=function(oElement)
	{
		var iReturnValue = 0;
		while( oElement != null )
		{
			iReturnValue += oElement.offsetLeft;
			oElement = oElement.offsetParent;
		}
		return iReturnValue-window.document.body.scrollLeft;
	}
	this.gety=function(oElement)
	{
		var iReturnValue = 0;
		while( oElement != null )
		{
			iReturnValue += oElement.offsetTop;
			oElement = oElement.offsetParent;
		}
		return iReturnValue-window.document.body.scrollTop;
	}
	this.set_area=function(e)
	{
		this.tb.style.width = e.offsetWidth + 4;
		this.tb.style.height = e.offsetHeight + 24;
		this.area.appendChild(e);
	};
	this.__m.onselectstart=function(){return false;};
};
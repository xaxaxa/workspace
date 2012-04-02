// JScript File
/*
[//	text			onclick
	["item1",		function(){alert("xxaxaxxaxa");}]
]
*/
function menu(wnd,items,p)
{
	if(p==null)p=wnd.document.body;
	this.wnd=wnd;
	this.items=items;
	this.element=wnd.document.createElement("table");
	this.element.style.cssText="display:none;cursor:default;position:absolute;z-index:99;border:#000000 1px solid;overflow:visible;background-color:#eaeaea;";
	this.tr=wnd.document.createElement("tr");
	this.td=wnd.document.createElement("td");
	this.tb=wnd.document.createElement("tbody");
	this.element.appendChild(this.tb);
	this.tb.appendChild(this.tr);
	this.tr.appendChild(this.td);
	this.element.cellSpacing="0";
	this.element.cellPadding="0";
	this.element.onmousedown=function(ev)
	{
		var e;
		if(ev)
			e=ev;
		else
			e=window.event;
		if(e)
			e.cancelBubble=true;
	};
	this.element.oncontextmenu=function(){return false;};
	this.element.onclick=this.element.onmousedown;
	p.appendChild(this.element);
	var f=new Function("if((this.__item[1])(this.__item)===false) return; else this.__menu.hide();");
	var f2=new Function("this.style.backgroundColor='#000000';this.style.color='#ffffff';selectchild(sctrl,parseInt(this.value));");
	var f3=new Function("this.style.backgroundColor='';this.style.color='';");
	var i;
	for(i=0;i<items.length;i++)
	{
		var el=wnd.document.createElement("div");
		el.__menu=this;
		el.onclick=f;
		el.onmouseover=f2;
		el.onmouseout=f3;
		el.style.cssText="padding-left:10px;padding-right:10px;";
		el.__item=items[i];
		el.innerHTML=items[i][0];
		this.td.appendChild(el);
	}
	this.show=function(x,y,control)
	{
		if(control!=null)
		{
			x+=get_x(control);
			y+=get_y(control);
		}
		this.element.style.top=y;
		this.element.style.left=x;
		this.element.style.display="";
	};
	this.hide=function()
	{
		this.element.style.display="none";
	};
}
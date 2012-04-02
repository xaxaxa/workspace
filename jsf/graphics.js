// JScript File

function graphics(e,wnd)
{
	this.e=e;
	this.drawrectangle=function(x,y,w,h,color)
	{
		var el=wnd.document.createElement("span");
		el.style.fontSize="1px";
		el.style.width=w;
		el.style.height=h;
		el.style.marginTop=y;
		el.style.marginLeft=x;
		return el;
	};
}
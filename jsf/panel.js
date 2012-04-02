// JScript File
var is_ie=false;
if(window.navigator.appName.toLowerCase().indexOf("microsoft")>-1)
{
	is_ie=true;
}
var __FILE_PANEL_JS=true;
var DOCK_NONE=0;
var DOCK_TOP=1;
var DOCK_BOTTOM=2;
var DOCK_LEFT=3;
var DOCK_RIGHT=4;
var DOCK_FILL=5;
var DOCK_TOPLEFT=6;
var DOCK_TOPRIGHT=7;
var DOCK_BOTTOMLEFT=8;
var DOCK_BOTTOMRIGHT=9;
var DOCK_CENTER=10;
var DOCK_CENTERH=11;
var DOCK_CENTERV=12;
var DOCK_FILLH=13;
var DOCK_FILLV=14;
var getp=function(e)
{
	return e.parentNode;
};
var layout_element=function(e,d)
{
	var p=getp(e);
	e.cur_d_t=p.d_t;
	e.cur_d_b=p.d_b;
	e.cur_d_l=p.d_l;
	e.cur_d_r=p.d_r;
	if(d==null)return;
	if(d==DOCK_NONE)return;
	if(e.style.display.toLowerCase=="none")return;
	var mt,mb,ml,mr;
	mt=((e.margintop==null) ? 0 : e.margintop);
	mb=((e.marginbottom==null) ? 0 : e.marginbottom);
	ml=((e.marginleft==null) ? 0 : e.marginleft);
	mr=((e.marginright==null) ? 0 : e.marginright);
	if(e.marginh!=null)
	{
		ml=e.marginh;
		mr=e.marginh;
	}
	if(e.marginv!=null)
	{
		mt=e.marginv;
		mb=e.marginv;
	}
	if(e.margin!=null)
	{
		mt=e.margin;
		mb=e.margin;
		ml=e.margin;
		mr=e.margin;
	}
	var tmpv;
	switch(d)
	{
		case DOCK_TOP:
			//e.style.width="0px";
			if(e.no_absolute==null)
			{
				e.style.position="absolute";
				e.style.top="";
				e.style.left="";
			}
			tmpv=(p.d_r - p.d_l - ml - mr);
			if(!is_ie)
			{
				tmpv-=e.offsetWidth-e.clientWidth;
			}
			if(tmpv<0)tmpv=0;
			e.style.width=tmpv.toString()+"px";
			e.style.marginTop=(p.d_t + mt).toString()+"px";
			e.style.marginLeft=(p.d_l + ml).toString()+"px";
			break;
		case DOCK_BOTTOM:
			//e.style.width="0px";
			if(e.no_absolute==null)
			{
				e.style.position="absolute";
				e.style.top="";
				e.style.left="";
			}
			tmpv=(p.d_r - p.d_l - ml - mr);
			if(!is_ie)
			{
				tmpv-=e.offsetWidth-e.clientWidth;
			}
			if(tmpv<0)tmpv=0;
			e.style.width=tmpv.toString()+"px";
			e.style.marginTop=(p.d_b - e.offsetHeight - mb).toString()+"px";
			e.style.marginLeft=(p.d_l + ml).toString()+"px";
			break;
		case DOCK_LEFT:
			//e.style.height="0px";
			if(e.no_absolute==null)
			{
				e.style.position="absolute";
				e.style.top="";
				e.style.left="";
			}
			tmpv=(p.d_b - p.d_t - mt - mb);
			if(!is_ie)
			{
				tmpv-=e.offsetHeight-e.clientHeight;
			}
			if(tmpv<0)tmpv=0;
			e.style.height=tmpv.toString()+"px";
			e.style.marginTop=(p.d_t + mt).toString()+"px";
			e.style.marginLeft=(p.d_l + ml).toString()+"px";
			break;
		case DOCK_RIGHT:
			//e.style.height="0px";
			if(e.no_absolute==null)
			{
				e.style.position="absolute";
				e.style.top="";
				e.style.left="";
			}
			tmpv=(p.d_b - p.d_t - mt - mb);
			if(!is_ie)
			{
				tmpv-=e.offsetHeight-e.clientHeight;
			}
			if(tmpv<0)tmpv=0;
			e.style.height=tmpv.toString()+"px";
			e.style.marginTop=(p.d_t + mt).toString()+"px";
			e.style.marginLeft=(p.d_r - e.offsetWidth - mr).toString()+"px";
			break;
		case DOCK_FILL:
			if(e.no_absolute==null)
			{
				e.style.position="absolute";
				e.style.top="";
				e.style.left="";
			}
			e.style.marginTop=(p.d_t + mt).toString()+"px";
			e.style.marginLeft=(p.d_l + ml).toString()+"px";
			//e.style.width="0px";
			tmpv=(p.d_r - p.d_l - ml - mr);
			if(!is_ie)
			{
				tmpv-=e.offsetWidth-e.clientWidth;
			}
			if(tmpv<0)tmpv=0;
			e.style.width=tmpv.toString()+"px";
			//e.style.height="0px";
			tmpv=(p.d_b - p.d_t - mt - mb);
			if(!is_ie)
			{
				tmpv-=e.offsetHeight-e.clientHeight;
			}
			if(tmpv<0)tmpv=0;
			e.style.height=tmpv.toString()+"px";
			break;
		case DOCK_FILLH:
			if(e.no_absolute==null)
			{
				e.style.position="absolute";
				e.style.top="";
				e.style.left="";
			}
			//e.style.marginTop=(p.d_t + mt).toString()+"px";
			e.style.marginLeft=(p.d_l + ml).toString()+"px";
			//e.style.width="0px";
			tmpv=(p.d_r - p.d_l - ml - mr);
			if(!is_ie)
			{
				tmpv-=e.offsetWidth-e.clientWidth;
			}
			if(tmpv<0)tmpv=0;
			e.style.width=tmpv.toString()+"px";
			//e.style.height="0px";
			/*tmpv=(p.d_b - p.d_t - mt - mb);
			if(!is_ie)
			{
				tmpv-=e.offsetHeight-e.clientHeight;
			}
			if(tmpv<0)tmpv=0;
			e.style.height=tmpv.toString()+"px";*/
			break;
		case DOCK_FILLV:
			if(e.no_absolute==null)
			{
				e.style.position="absolute";
				e.style.top="";
				e.style.left="";
			}
			e.style.marginTop=(p.d_t + mt).toString()+"px";
			//e.style.marginLeft=(p.d_l + ml).toString()+"px";
			//e.style.width="0px";
			/*tmpv=(p.d_r - p.d_l - ml - mr);
			if(!is_ie)
			{
				tmpv-=e.offsetWidth-e.clientWidth;
			}
			if(tmpv<0)tmpv=0;
			e.style.width=tmpv.toString()+"px";*/
			//e.style.height="0px";
			tmpv=(p.d_b - p.d_t - mt - mb);
			if(!is_ie)
			{
				tmpv-=e.offsetHeight-e.clientHeight;
			}
			if(tmpv<0)tmpv=0;
			e.style.height=tmpv.toString()+"px";
			break;
		case DOCK_TOPLEFT:
			if(e.no_absolute==null)
			{
				e.style.position="absolute";
				e.style.top="";
				e.style.left="";
			}
			e.style.marginTop=(p.d_t + mt)+"px";
			e.style.marginLeft=(p.d_l + ml)+"px";
			break;
		case DOCK_TOPRIGHT:
			if(e.no_absolute==null)
			{
				e.style.position="absolute";
				e.style.top="";
				e.style.left="";
			}
			e.style.marginTop=(p.d_t + mt)+"px";
			e.style.marginLeft=(p.d_r - e.offsetWidth - mr).toString()+"px";
			break;
		case DOCK_BOTTOMLEFT:
			if(e.no_absolute==null)
			{
				e.style.position="absolute";
				e.style.top="";
				e.style.left="";
			}
			e.style.marginTop=(p.d_b - e.offsetHeight - mb).toString()+"px";
			e.style.marginLeft=(p.d_l + ml)+"px";
			break;
		case DOCK_BOTTOMRIGHT:
			if(e.no_absolute==null)
			{
				e.style.position="absolute";
				e.style.top="";
				e.style.left="";
			}
			e.style.marginTop=(p.d_b - e.offsetHeight - mb).toString()+"px";
			e.style.marginLeft=(p.d_r - e.offsetWidth - mr).toString()+"px";
			break;
		case DOCK_CENTER:
			if(e.no_absolute==null)
			{
				e.style.position="absolute";
				e.style.top="";
				e.style.left="";
			}
			nv=(p.d_l + (p.d_r - p.d_l)/2 - e.offsetWidth/2 + (ml - mr));
			if(nv<0)nv=0;
			e.style.marginLeft=nv.toString()+"px";
			nv=(p.d_t + (p.d_b - p.d_t)/2 - e.offsetHeight/2 + (mt - mb));
			if(nv<0)nv=0;
			e.style.marginTop=nv.toString()+"px";
			break;
		case DOCK_CENTERH:
			if(e.no_absolute==null)
			{
				e.style.position="absolute";
				e.style.top="";
				e.style.left="";
			}
			nv=(p.d_l + (p.d_r - p.d_l)/2 - e.offsetWidth/2 + (ml - mr));
			if(nv<0)nv=0;
			e.style.marginLeft=nv.toString()+"px";
			break;
		case DOCK_CENTERV:
			if(e.no_absolute==null)
			{
				e.style.position="absolute";
				e.style.top="";
				e.style.left="";
			}
			nv=(p.d_t + (p.d_b - p.d_t)/2 - e.offsetHeight/2 + (mt - mb));
			if(nv<0)nv=0;
			e.style.marginTop=nv.toString()+"px";
			break;
	}
	switch(d)
	{
		case DOCK_TOP:
			p.d_t+=(e.offsetHeight + mt + mb);
			break;
		case DOCK_BOTTOM:
			p.d_b-=(e.offsetHeight + mt + mb);
			break;
		case DOCK_LEFT:
			p.d_l+=(e.offsetWidth + ml + mr);
			break;
		case DOCK_RIGHT:
			p.d_r-=(e.offsetWidth + ml + mr);
			break;
	}
};
function layout_document()
{
		var i;
		var el=this.wnd.document.getElementsByTagName("*");
		for(i=0;i<el.length;i++)
		{
			if(el[i].tagName.toLower != "body")
			{
				el[i].d_t=0;
				el[i].d_l=0;
				el[i].d_r=el[i].clientWidth;
				el[i].d_b=el[i].clientHeight;
			}
		}
		for(i=0;i<el.length;i++)
		{
			if(el[i].tagName.toLower != "body")layout_element(el[i],el[i].dock);
			
		}
}
function panel(e,wnd)
{
	//this.d=[];
	this.e=e;
	this.wnd=wnd;
	this.get_elements=function(el)
	{
		if(el==null)el=this.e;
		var ar=[];
		this.get_e(ar,el);
		return ar;
	};
	this.get_e=function(arr,e)
	{
		arr[arr.length]=e;
		var i;
		for(i=0;i<e.childNodes.length;i++)
		{
			if(e.childNodes[i].style!=null)
			{
				if(e.childNodes[i].tagName!=null)
				{
					//arr[arr.length]=e.childNodes[i];
					if(e.childNodes[i].childNodes!=null)
					{
						this.get_e(arr,e.childNodes[i]);
					}
				}
			}
		}
	};
	this.indexof=function(arr,val)
	{
		var i;
		for(i=0;i<arr.length;i++)
		{
			if(arr[i]==val)return i;
		}
		return -1;
	};
	this.layout=function(from)
	{
		var _i;
		for(_i=0;_i<1;_i++)
		{
			var i;
			var el=this.get_elements(from==null ? null : from.parentNode);//this.wnd.document.getElementsByTagName("*");
			var ind=0;
			if(from!=null)ind=this.indexof(el,from);
			if(ind==0)
			{
				for(i=0;i<el.length;i++)
				{
					if(el[i].tagName.toLower != "body")
					{
						el[i].d_t=0;
						el[i].d_l=0;
						el[i].d_r=el[i].clientWidth;
						el[i].d_b=el[i].clientHeight;
						var mt,mb,ml,mr;
						var e=el[i];
						mt=((e.paddingtop==null) ? 0 : e.paddingtop);
						mb=((e.paddingbottom==null) ? 0 : e.paddingbottom);
						ml=((e.paddingleft==null) ? 0 : e.paddingleft);
						mr=((e.paddingright==null) ? 0 : e.paddingright);
						if(e.paddingh!=null)
						{
							ml=e.paddingh;
							mr=e.paddingh;
						}
						if(e.paddingv!=null)
						{
							mt=e.paddingv;
							mb=e.paddingv;
						}
						if(e.padding!=null)
						{
							mt=e.padding;
							mb=e.padding;
							ml=e.padding;
							mr=e.padding;
						}
						el[i].d_t+=mt;
						el[i].d_l+=ml;
						el[i].d_r-=mr;
						el[i].d_b-=mb;
					}
				}
			}
			if(from!=null)
			{
				var p=from.parentNode;
				p.d_t=from.cur_d_t;
				p.d_b=from.cur_d_b;
				p.d_l=from.cur_d_l;
				p.d_r=from.cur_d_r;
			}
			for(i=ind;i<el.length;i++)
			{
				if(el[i].tagName.toLower != "body")
				{
					var e=el[i];
					layout_element(el[i],el[i].dock);
					e.d_t=0;
					e.d_l=0;
					e.d_r=e.clientWidth;
					e.d_b=e.clientHeight;
					var mt,mb,ml,mr;
					mt=((e.paddingtop==null) ? 0 : e.paddingtop);
					mb=((e.paddingbottom==null) ? 0 : e.paddingbottom);
					ml=((e.paddingleft==null) ? 0 : e.paddingleft);
					mr=((e.paddingright==null) ? 0 : e.paddingright);
					if(e.paddingh!=null)
					{
						ml=e.paddingh;
						mr=e.paddingh;
					}
					if(e.paddingv!=null)
					{
						mt=e.paddingv;
						mb=e.paddingv;
					}
					if(e.padding!=null)
					{
						mt=e.padding;
						mb=e.padding;
						ml=e.padding;
						mr=e.padding;
					}
					e.d_t+=mt;
					e.d_l+=ml;
					e.d_r-=mr;
					e.d_b-=mb;
				}
			}
		}
	};
	this.setdock=function(e,dock)
	{
		//var tmp=new Object();
		//tmp.element=e;
		//tmp.dock=dock;
		//this.d[this.d.length]=tmp;
		e.dock=dock;
		layout_element(e,dock);
	};
	this.add=function(e)
	{
		this.e.appendChild(e);
	};
}

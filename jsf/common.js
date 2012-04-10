window.is_ie=(window.navigator.appName.toLowerCase().indexOf("microsoft")>-1);
function findPos(obj) {
 var obj2 = obj;
 var curtop = 0;
 var curleft = 0;
 if (document.getElementById || document.all) {
  do  {
   curleft += obj.offsetLeft-obj.scrollLeft;
   curtop += obj.offsetTop-obj.scrollTop;
   obj = obj.offsetParent;
   obj2 = obj2.parentNode;
   while (obj2!=obj) {
    curleft -= obj2.scrollLeft;
    curtop -= obj2.scrollTop;
    obj2 = obj2.parentNode;
   }
  } while (obj.offsetParent)
 } else if (document.layers) {
  curtop += obj.y;
  curleft += obj.x;
 }
 return [curleft, curtop];
}
function get_pos(e,t)
{
	var ret = [0,0];
	var first=true;
	while( e != null )
	{
		ret[0] += e.offsetLeft;
		ret[1] += e.offsetTop;
		if(!first)
		{
			ret[0] += e.clientLeft-e.scrollLeft;
			ret[1] += e.clientTop-e.scrollTop;
		}
		e = e.offsetParent;
		first=false;
	}
	return ret;
}
function get_x(oElement,t)
{
	var iReturnValue = 0;
	var first=true;
	while( oElement != null )
	{
		iReturnValue += oElement.offsetLeft;
		if(!first)
		{
			//iReturnValue+=(oElement.offsetLeft-oElement.clientLeft);
			iReturnValue += oElement.clientLeft-oElement.scrollLeft;
			//iReturnValue += oElement.offsetLeft-oElement.clientLeft;
		}
		oElement = oElement.offsetParent;
		first=false;
	}
	//if(t)iReturnValue-=t.scrollLeft;
	//if(!is_ie)iReturnValue+=2;
	return iReturnValue;
}
function get_y(oElement,t)
{
	var iReturnValue = 0;
	var first=true;
	while( oElement != null )
	{
		iReturnValue += oElement.offsetTop;
		if(!first)
		{
			iReturnValue+=oElement.clientTop-oElement.scrollTop;
		}
		oElement = oElement.offsetParent;
		first=false;
	}
	//if(t)iReturnValue-=t.scrollTop;
	//if(!is_ie)iReturnValue+=2;
	return iReturnValue;
}
function isChild(parent, child) {
     var node = child.parentNode;
     while (node != null) {
         if (node == parent) {
             return true;
         }
         node = node.parentNode;
     }
     return false;
}
function insertAdjacentElement(p,where,parsedNode)
{
	switch (where)
	{
		case "beforeBegin":
			p.parentNode.insertBefore(parsedNode,p);
			break;
		case "afterBegin":
			p.insertBefore(parsedNode,p.firstChild);
			break;
		case "beforeEnd":
			p.appendChild(parsedNode);
			break;
		case "afterEnd":
			if(p.nextSibling)
				p.parentNode.insertBefore(parsedNode,p.nextSibling);
			else
				p.parentNode.appendChild(parsedNode);
		break;
	}
}

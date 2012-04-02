// JScript File
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
		}
		/*else
		{
			
		}*/
		oElement = oElement.offsetParent;
		first=false;
	}
	//if(t)iReturnValue-=t.scrollLeft;
	if(!is_ie)iReturnValue+=2;
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
	if(!is_ie)iReturnValue+=2;
	return iReturnValue;
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

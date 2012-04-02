// JScript File

function querystring(n)
{
	var s;
	var st;
	var en;
	var _url=window.location.href;
	s = _url.substring(_url.indexOf("?",0) + 1,_url.length);
	st = s.indexOf(n + "=",0);
	if(st == -1)
	{
		return null;
		//return s.indexOf(n + "=",0);
		//return null;
	}
	st += n.length + 1;
	if(s.indexOf("&",st) == -1)
	{
		en = s.length;
	}
	else
	{
		en = s.indexOf("&",st);
	}
	return unescape(s.substring(st, en));
};
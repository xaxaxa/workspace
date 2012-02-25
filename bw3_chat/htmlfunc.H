/*
 * htmlfunc.H
 *
 *  Created on: Feb 17, 2012
 *      Author: xaxaxa
 */

#ifndef HTMLFUNC_H_
#define HTMLFUNC_H_
#include <string>
#include <boost/algorithm/string.hpp>
#include <Wt/WApplication>
#include <cplib/cplib.hpp>
using namespace Wt;
using namespace std;
using namespace boost;
namespace xaxaxa
{
	const char* js1 = NULL;
	void parseDates(WApplication& app)
	{
		if (js1 == NULL)
		{
			js1 = "function formatDate(d)"
					"{"
					"	var M=(d.getMonth()+1);"
					"	var D=d.getDay();"
					"	var m=d.getMinutes();"
					"	var h=d.getHours();"
					"	return d.getFullYear()+'-'+(M<10?'0'+M:M)+'-'+(D<10?'0'+D:D)"
					"			+' '+((h==0||h==12)?12:(h%12))+':'+(m<10?'0'+m:m)+(h>=12?' PM':' AM');"
					"}"
					"var a=document.getElementsByTagName(\"span\");var i;"
					"for(i=0;i<a.length;i++)\n"
					"{"
					"	if(a[i].getAttribute(\"class\")==\"datetime\")"
					"	{	try"
					"		{	var d=(new Date(parseFloat(a[i].textContent)));"
					"			a[i].textContent=formatDate(d);"
					"			a[i].setAttribute(\"class\",\"\");"
					"		}catch(e){}"
					"	}"
					"}";
		}
		app.doJavaScript(js1, true);
	}
	void HTMLEscape(string& s)
	{
		replace_all(s, "&", "&amp;");
		replace_all(s, "<", "&lt;");
		replace_all(s, ">", "&gt;");
	}
	string HTMLEscape1(const string& s)
	{
		string tmp = s;
		replace_all(tmp, "&", "&amp;");
		replace_all(tmp, "<", "&lt;");
		replace_all(tmp, ">", "&gt;");
		return tmp;
	}
	void HTMLAttributeEscape(string& s)
	{
		replace_all(s, "&", "&amp;");
		replace_all(s, "<", "&lt;");
		replace_all(s, ">", "&gt;");
		replace_all(s, "\"", "&quot;");
		replace_all(s, "\'", "&apos;");
	}
	string HTMLAttributeEscape(const string& s)
	{
		string tmp = s;
		replace_all(tmp, "&", "&amp;");
		replace_all(tmp, "<", "&lt;");
		replace_all(tmp, ">", "&gt;");
		replace_all(tmp, "\"", "&quot;");
		replace_all(tmp, "\'", "&apos;");
		return tmp;
	}
}

#endif /* HTMLFUNC_H_ */

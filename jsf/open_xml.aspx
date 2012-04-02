<%@ PAGE LANGUAGE="VB" VALIDATEREQUEST="false" BUFFER="false" %>

<SCRIPT RUNAT="server">
	Dim scr As String = "var __rf=function(){return false;};", ind As Integer = 0
	Dim edit As Boolean = False
	Protected Sub Page_Load(ByVal sender As Object, ByVal e As System.EventArgs)
		'System.Threading.Thread.Sleep(1000)
		Dim x As New System.Xml.XmlDocument()
		x.Load(Server.MapPath("f/"+Request.QueryString.Get("fn")))
		Response.Write("<html><HEAD><SCRIPT SRC=""panel.js""></SCR" + "IPT><SCRIPT SRC=""splitter.js""></SCR" + "IPT><SCRIPT SRC=""wnd_2.js""></SCR" + "IPT><SCRIPT SRC=""rs.js""></SCR" + "IPT></HEAD><BODY TOPMARGIN=""0"" BOTTOMMARGIN=""0"" LEFTMARGIN=""0"" RIGHTMARGIN=""0"" ONLOAD=""l();"" ONRESIZE=""p.layout();"" STYLE=""OVERFLOW: HIDDEN; BORDER: NONE;""><DIV STYLE=""width: 100%; height: 100%; position: absolute; background-color: #d4d0c8;"" ID=""d"">")
		If Request.QueryString("edit") = "1" Then
			edit = True
		End If
		If edit Then
			Response.Write("<textarea style=""width:100%; height:100%;"" id=""__content_ta"">")
			'Dim i As Integer
			'For i = 0 To x.DocumentElement.ChildNodes.Count - 1
			'	addnode(x.DocumentElement.ChildNodes(i), Response.Output)
			'Next
			addnode(x.DocumentElement, Response.Output, "mainpanel")
			Response.Write("</textarea>")
		Else
			addnode(x.DocumentElement, Response.Output)
		End If
		If edit Then
			Response.Write("</div><script>var p;function l(){" + vbCr + "window.parent.load_callback(document.getElementById(""__content_ta"").value);" + vbCr + scr + vbCr + "window.parent.addelement(window.parent.document.getElementById(""mainpanel""));" + vbCr + "window.parent.load_callback_done();window.parent.p.layout();" + vbCr + "}</scr" + "ipt></BODY></html>")
		Else
			Response.Write("</div><script>var p;function l(){" + vbCr + "p=new panel(document.body,window);" + vbCr + scr + vbCr + "p.layout();" + vbCr + "}</scr" + "ipt></BODY></html>")
		End If
	
	End Sub
	Sub addnode(ByVal n As System.Xml.XmlElement, ByVal o As IO.TextWriter, Optional ByVal __id As String = Nothing)
		Dim attr As System.Xml.XmlAttribute
		attr = n.Attributes("tagname")
		If attr Is Nothing Then Return
		Dim tn As String = attr.Value
		If __id = Nothing Then o.Write("&lt;" + tn)
		'attr = n.Attributes("x")
		'If Not attr Is Nothing Then o.Write("margin-left:" + attr.Value)
		'attr = n.Attributes("y")
		'If Not attr Is Nothing Then o.Write("margin-top:" + attr.Value)
		'attr = n.Attributes("w")
		'If Not attr Is Nothing Then o.Write("width:" + attr.Value)
		'attr = n.Attributes("h")
		'If Not attr Is Nothing Then o.Write("height:" + attr.Value)
		Dim id As String
		If __id = Nothing Then
			attr = n.Attributes("id")
			If attr Is Nothing Then
				id = "el_" + ind.ToString()
				ind += 1
			Else
				id = attr.Value
			End If
			o.Write(" id=""" + id + """")
		Else
			id = __id
		End If
		If __id = Nothing Then
			attr = n.Attributes("style")
			If Not attr Is Nothing Then o.Write(" style=""" + attr.Value + """")
		Else
			attr = n.Attributes("style")
			If Not attr Is Nothing Then scr += "window.parent.document.getElementById(""" + id + """).style.cssText=""" + attr.Value.Replace("\", "\\").Replace("""", "\""").Replace(vbCr, "\r").Replace(vbLf, "\f").Replace("'", "\'") + """;" + vbCr
		End If
		
		scr += "try{" + vbCr
		If edit Then
			'scr += "window.parent.rs1.add(window.parent.document.getElementById(""" + id + """));" + vbCr
			attr = n.Attributes("id")
			If Not attr Is Nothing Then scr += "window.parent.document.getElementById(""" + id + """).__id=""" + attr.Value + """;" + vbCr
			attr = n.Attributes("dock")
			If Not attr Is Nothing Then scr += "window.parent.document.getElementById(""" + id + """).dock=" + attr.Value + ";" + vbCr
			attr = n.Attributes("name")
			If Not attr Is Nothing Then scr += "window.parent.document.getElementById(""" + id + """).__name=""" + attr.Value + """;" + vbCr
			
			attr = n.Attributes("onmousedown")
			If Not attr Is Nothing Then scr += "window.parent.document.getElementById(""" + id + """).__onmousedown=""" + attr.Value.Replace("\", "\\").Replace("""", "\""").Replace(vbCr, "\r").Replace(vbLf, "\f").Replace("'", "\'") + """;" + vbCr
			attr = n.Attributes("onmouseup")
			If Not attr Is Nothing Then scr += "window.parent.document.getElementById(""" + id + """).__onmouseup=""" + attr.Value.Replace("\", "\\").Replace("""", "\""").Replace(vbCr, "\r").Replace(vbLf, "\f").Replace("'", "\'") + """;" + vbCr
			attr = n.Attributes("onclick")
			If Not attr Is Nothing Then scr += "window.parent.document.getElementById(""" + id + """).__onclick=""" + attr.Value.Replace("\", "\\").Replace("""", "\""").Replace(vbCr, "\r").Replace(vbLf, "\f").Replace("'", "\'") + """;" + vbCr
			attr = n.Attributes("onmouseover")
			If Not attr Is Nothing Then scr += "window.parent.document.getElementById(""" + id + """).__onmouseover=""" + attr.Value.Replace("\", "\\").Replace("""", "\""").Replace(vbCr, "\r").Replace(vbLf, "\f").Replace("'", "\'") + """;" + vbCr
			attr = n.Attributes("onmouseout")
			If Not attr Is Nothing Then scr += "window.parent.document.getElementById(""" + id + """).__onmouseout=""" + attr.Value.Replace("\", "\\").Replace("""", "\""").Replace(vbCr, "\r").Replace(vbLf, "\f").Replace("'", "\'") + """;" + vbCr
			attr = n.Attributes("onmousemove")
			If Not attr Is Nothing Then scr += "window.parent.document.getElementById(""" + id + """).__onmousemove=""" + attr.Value.Replace("\", "\\").Replace("""", "\""").Replace(vbCr, "\r").Replace(vbLf, "\f").Replace("'", "\'") + """;" + vbCr
			attr = n.Attributes("onkeydown")
			If Not attr Is Nothing Then scr += "window.parent.document.getElementById(""" + id + """).__onkeydown=""" + attr.Value.Replace("\", "\\").Replace("""", "\""").Replace(vbCr, "\r").Replace(vbLf, "\f").Replace("'", "\'") + """;" + vbCr
			attr = n.Attributes("onkeyup")
			If Not attr Is Nothing Then scr += "window.parent.document.getElementById(""" + id + """).__onkeyup=""" + attr.Value.Replace("\", "\\").Replace("""", "\""").Replace(vbCr, "\r").Replace(vbLf, "\f").Replace("'", "\'") + """;" + vbCr
			attr = n.Attributes("contenteditable")
			If Not attr Is Nothing AndAlso attr.Value <> Nothing Then scr += "window.parent.document.getElementById(""" + id + """).__contenteditable=""" + attr.Value + """;" + vbCr
			attr = n.Attributes("value")
			If Not attr Is Nothing Then scr += "window.parent.document.getElementById(""" + id + """).value=""" + attr.Value.Replace("\", "\\").Replace("""", "\""").Replace(vbCr, "\r").Replace(vbLf, "\f").Replace("'", "\'") + """;" + vbCr
			attr = n.Attributes("href")
			If Not attr Is Nothing Then scr += "window.parent.document.getElementById(""" + id + """).__href=""" + attr.Value.Replace("\", "\\").Replace("""", "\""").Replace(vbCr, "\r").Replace(vbLf, "\f").Replace("'", "\'") + """;" + vbCr
			attr = n.Attributes("border")
			If Not attr Is Nothing Then scr += "window.parent.document.getElementById(""" + id + """).border=""" + attr.Value + """;" + vbCr
			attr = n.Attributes("bordercolor")
			If Not attr Is Nothing Then scr += "window.parent.document.getElementById(""" + id + """).borderColor=""" + attr.Value + """;" + vbCr
			attr = n.Attributes("bordercolordark")
			If Not attr Is Nothing Then scr += "window.parent.document.getElementById(""" + id + """).borderColorDark=""" + attr.Value + """;" + vbCr
			attr = n.Attributes("bordercolorlight")
			If Not attr Is Nothing Then scr += "window.parent.document.getElementById(""" + id + """).borderColorLight=""" + attr.Value + """;" + vbCr
			attr = n.Attributes("cellspacing")
			If Not attr Is Nothing Then scr += "window.parent.document.getElementById(""" + id + """).cellSpacing=""" + attr.Value + """;" + vbCr
			attr = n.Attributes("cellpadding")
			If Not attr Is Nothing Then scr += "window.parent.document.getElementById(""" + id + """).cellPadding=""" + attr.Value + """;" + vbCr
			attr = n.Attributes("align")
			If Not attr Is Nothing Then scr += "window.parent.document.getElementById(""" + id + """).align=""" + attr.Value + """;" + vbCr
			attr = n.Attributes("valign")
			If Not attr Is Nothing Then scr += "window.parent.document.getElementById(""" + id + """).vAlign=""" + attr.Value + """;" + vbCr

			attr = n.Attributes("margintop")
			If Not attr Is Nothing Then scr += "window.parent.document.getElementById(""" + id + """).margintop=" + attr.Value + ";" + vbCr
			attr = n.Attributes("marginbottom")
			If Not attr Is Nothing Then scr += "window.parent.document.getElementById(""" + id + """).marginbottom=" + attr.Value + ";" + vbCr
			attr = n.Attributes("marginleft")
			If Not attr Is Nothing Then scr += "window.parent.document.getElementById(""" + id + """).marginleft=" + attr.Value + ";" + vbCr
			attr = n.Attributes("marginright")
			If Not attr Is Nothing Then scr += "window.parent.document.getElementById(""" + id + """).marginright=" + attr.Value + ";" + vbCr
			scr += "window.parent.document.getElementById(""" + id + """).onclick=__rf;" + vbCr
			scr += "window.parent.document.getElementById(""" + id + """).onmousedown=__rf;" + vbCr
		Else
			attr = n.Attributes("dock")
			If Not attr Is Nothing Then scr += "document.getElementById(""" + id + """).dock=" + attr.Value + ";" + vbCr
			attr = n.Attributes("name")
			If Not attr Is Nothing Then scr += "document.getElementById(""" + id + """).__name=""" + attr.Value + """;" + vbCr

			attr = n.Attributes("margintop")
			If Not attr Is Nothing Then scr += "document.getElementById(""" + id + """).margintop=" + attr.Value + ";" + vbCr
			attr = n.Attributes("marginbottom")
			If Not attr Is Nothing Then scr += "document.getElementById(""" + id + """).marginbottom=" + attr.Value + ";" + vbCr
			attr = n.Attributes("marginleft")
			If Not attr Is Nothing Then scr += "document.getElementById(""" + id + """).marginleft=" + attr.Value + ";" + vbCr
			attr = n.Attributes("marginright")
			If Not attr Is Nothing Then scr += "document.getElementById(""" + id + """).marginright=" + attr.Value + ";" + vbCr
		End If
		scr += "}catch(ex){}" + vbCr
		
		'events
		If Not edit And __id = Nothing Then
			attr = n.Attributes("onmousedown")
			If Not attr Is Nothing Then o.Write(" onmousedown=""" + attr.Value.Replace("&", "&amp;").Replace("<", "&lt;").Replace(">", "&gt;").Replace("""", "&quot;").Replace(vbCr, "&#13;").Replace(vbLf, "&#10;") + """") '
			attr = n.Attributes("onmouseup")
			If Not attr Is Nothing Then o.Write(" onmouseup=""" + attr.Value.Replace("&", "&amp;").Replace("<", "&lt;").Replace(">", "&gt;").Replace("""", "&quot;").Replace(vbCr, "&#13;").Replace(vbLf, "&#10;") + """") 'scr += "document.getElementById(""el_" + ind.ToString() + """).onmouseup=""" + attr.Value + """;" + vbCr
			attr = n.Attributes("onclick")
			If Not attr Is Nothing Then o.Write(" onclick=""" + attr.Value.Replace("&", "&amp;").Replace("<", "&lt;").Replace(">", "&gt;").Replace("""", "&quot;").Replace(vbCr, "&#13;").Replace(vbLf, "&#10;") + """") 'scr += "document.getElementById(""el_" + ind.ToString() + """).onclick=""" + attr.Value.Replace("&", "&amp;").Replace("<", "&lt;").Replace(">", "&gt;").Replace("""", "&quot").Replace(vbCr, "&#13;").Replace(vbLf, "&#10;") + """;" + vbCr
			attr = n.Attributes("onmouseover")
			If Not attr Is Nothing Then o.Write(" onmouseover=""" + attr.Value.Replace("&", "&amp;").Replace("<", "&lt;").Replace(">", "&gt;").Replace("""", "&quot;").Replace(vbCr, "&#13;").Replace(vbLf, "&#10;") + """") 'scr += "document.getElementById(""el_" + ind.ToString() + """).onmouseover=""" + attr.Value.Replace("&", "&amp;").Replace("<", "&lt;").Replace(">", "&gt;").Replace("""", "&quot").Replace(vbCr, "&#13;").Replace(vbLf, "&#10;") + """;" + vbCr
			attr = n.Attributes("onmouseout")
			If Not attr Is Nothing Then o.Write(" onmouseout=""" + attr.Value.Replace("&", "&amp;").Replace("<", "&lt;").Replace(">", "&gt;").Replace("""", "&quot;").Replace(vbCr, "&#13;").Replace(vbLf, "&#10;") + """") 'scr += "document.getElementById(""el_" + ind.ToString() + """).onmouseout=""" + attr.Value.Replace("&", "&amp;").Replace("<", "&lt;").Replace(">", "&gt;").Replace("""", "&quot").Replace(vbCr, "&#13;").Replace(vbLf, "&#10;") + """;" + vbCr
			attr = n.Attributes("onmousemove")
			If Not attr Is Nothing Then o.Write(" onmousemove=""" + attr.Value.Replace("&", "&amp;").Replace("<", "&lt;").Replace(">", "&gt;").Replace("""", "&quot;").Replace(vbCr, "&#13;").Replace(vbLf, "&#10;") + """") 'scr += "document.getElementById(""el_" + ind.ToString() + """).onmousemove=""" + attr.Value.Replace("&", "&amp;").Replace("<", "&lt;").Replace(">", "&gt;").Replace("""", "&quot").Replace(vbCr, "&#13;").Replace(vbLf, "&#10;") + """;" + vbCr
			attr = n.Attributes("onkeydown")
			If Not attr Is Nothing Then o.Write(" onkeydown=""" + attr.Value.Replace("&", "&amp;").Replace("<", "&lt;").Replace(">", "&gt;").Replace("""", "&quot;").Replace(vbCr, "&#13;").Replace(vbLf, "&#10;") + """") 'scr += "document.getElementById(""el_" + ind.ToString() + """).onkeydown=""" + attr.Value.Replace("&", "&amp;").Replace("<", "&lt;").Replace(">", "&gt;").Replace("""", "&quot").Replace(vbCr, "&#13;").Replace(vbLf, "&#10;") + """;" + vbCr
			attr = n.Attributes("onkeyup")
			If Not attr Is Nothing Then o.Write(" onkeyup=""" + attr.Value.Replace("&", "&amp;").Replace("<", "&lt;").Replace(">", "&gt;").Replace("""", "&quot;").Replace(vbCr, "&#13;").Replace(vbLf, "&#10;") + """") 'scr += "document.getElementById(""el_" + ind.ToString() + """).onkeyup=""" + attr.Value.Replace("&", "&amp;").Replace("<", "&lt;").Replace(">", "&gt;").Replace("""", "&quot").Replace(vbCr, "&#13;").Replace(vbLf, "&#10;") + """;" + vbCr
			attr = n.Attributes("contenteditable")
			If Not attr Is Nothing AndAlso attr.Value <> Nothing Then o.Write(" contenteditable=""" + attr.Value + """") 'scr += "document.getElementById(""el_" + ind.ToString() + """).onkeyup=""" + attr.Value.Replace("&", "&amp;").Replace("<", "&lt;").Replace(">", "&gt;").Replace("""", "&quot").Replace(vbCr, "&#13;").Replace(vbLf, "&#10;") + """;" + vbCr
			attr = n.Attributes("value")
			If Not attr Is Nothing Then o.Write(" value=""" + attr.Value.Replace("&", "&amp;").Replace("<", "&lt;").Replace(">", "&gt;").Replace("""", "&quot;").Replace(vbCr, "&#13;").Replace(vbLf, "&#10;") + """") 'scr += "document.getElementById(""el_" + ind.ToString() + """).onkeyup=""" + attr.Value.Replace("&", "&amp;").Replace("<", "&lt;").Replace(">", "&gt;").Replace("""", "&quot").Replace(vbCr, "&#13;").Replace(vbLf, "&#10;") + """;" + vbCr
			attr = n.Attributes("href")
			If Not attr Is Nothing Then o.Write(" href=""" + attr.Value.Replace("&", "&amp;").Replace("<", "&lt;").Replace(">", "&gt;").Replace("""", "&quot;").Replace(vbCr, "&#13;").Replace(vbLf, "&#10;") + """") 'scr += "document.getElementById(""el_" + ind.ToString() + """).onkeyup=""" + attr.Value.Replace("&", "&amp;").Replace("<", "&lt;").Replace(">", "&gt;").Replace("""", "&quot").Replace(vbCr, "&#13;").Replace(vbLf, "&#10;") + """;" + vbCr
			attr = n.Attributes("border")
			If Not attr Is Nothing Then o.Write(" border=""" + attr.Value + """;" + vbCr)
			attr = n.Attributes("bordercolor")
			If Not attr Is Nothing Then o.Write(" bordercolor=""" + attr.Value + """;" + vbCr)
			attr = n.Attributes("bordercolordark")
			If Not attr Is Nothing Then o.Write(" bordercolordark=""" + attr.Value + """;" + vbCr)
			attr = n.Attributes("bordercolorlight")
			If Not attr Is Nothing Then o.Write(" bordercolorlight=""" + attr.Value + """;" + vbCr)
			attr = n.Attributes("cellspacing")
			If Not attr Is Nothing Then o.Write(" cellspacing=""" + attr.Value + """;" + vbCr)
			attr = n.Attributes("cellpadding")
			If Not attr Is Nothing Then o.Write(" cellpadding=""" + attr.Value + """;" + vbCr)
			attr = n.Attributes("align")
			If Not attr Is Nothing Then o.Write(" align=""" + attr.Value + """;" + vbCr)
			attr = n.Attributes("valign")
			If Not attr Is Nothing Then o.Write(" valign=""" + attr.Value + """;" + vbCr)
		End If
		If __id = Nothing Then
			attr = n.Attributes("type")
			If Not attr Is Nothing Then o.Write(" type=""" + attr.Value + """" + vbCr)
			attr = n.Attributes("src")
			If Not attr Is Nothing Then o.Write(" src=""" + attr.Value + """" + vbCr)
			o.Write("&gt;")
		End If

		attr = n.Attributes("nochild")
		If Not attr Is Nothing Then
			If attr.Value = "1" Then
				o.Write(n.InnerText)
				GoTo a
			End If
		End If
		Dim i As Integer
		For i = 0 To n.ChildNodes.Count - 1
			If TypeOf n.ChildNodes(i) Is System.Xml.XmlElement Then
				addnode(CType(n.ChildNodes(i), System.Xml.XmlElement), o)
			End If
		Next
		If tn.ToLower() = "td" Then
			o.Write("&nbsp;")
		End If
a:
		If __id = Nothing Then o.Write("&lt;/" + tn + "&gt;")
	End Sub
</SCRIPT>


<%@ Page Language="VB" %>
<html>
<head runat="server">
    <title>Untitled Page</title>
    <script language=javascript>
	var getx=function(oElement)
	{
		var iReturnValue = 0;
		while( oElement != null )
		{
			iReturnValue += oElement.offsetLeft;
			oElement = oElement.offsetParent;
		}
		return iReturnValue;
	};
	var gety=function(oElement)
	{
		var iReturnValue = 0;
		while( oElement != null )
		{
			iReturnValue += oElement.offsetTop;
			oElement = oElement.offsetParent;
		}
		return iReturnValue;
	};
    function updateimg()
    {
	var imgs=document.getElementsByTagName("img");
	var i;
	for(i=0;i<imgs.length;i++)
	{
		if(imgs[i].__loaded==null)
		{
			if(  ((getx(imgs[i])+imgs[i].offsetWidth)>document.body.scrollLeft)  &&  (getx(imgs[i])<(document.body.scrollLeft+document.body.offsetWidth))  &&  ((gety(imgs[i])+imgs[i].offsetHeight) > document.body.scrollTop)  &&  (gety(imgs[i])<(document.body.scrollTop+document.body.offsetHeight)) )
			{
				imgs[i].src=imgs[i].title;
				imgs[i].__loaded=true;
			}
		}
	}
    }
    </script>
</head>
<body onscroll="updateimg();" style="overflow: auto; border: none;background-color: #d4d0c8;" bottommargin="0" leftmargin="0" rightmargin="0" topmargin="0">
    <%
    	    Dim w, h As Integer
    	    If Not Request.QueryString("w") = Nothing Then w = Request.QueryString("w") Else w = 200
    	    If Not Request.QueryString("h") = Nothing Then h = Request.QueryString("h") Else h = 200
    	    Dim img As System.Drawing.Image = System.Drawing.Image.FromFile(Server.MapPath(Request.QueryString("img")))
    	    Dim i1, i2 As Integer
    	    Dim _w As Integer = img.Width
    	    Dim _h As Integer = img.Height
    	    Session(Request.QueryString("session")) = img
    	    Dim iw, ih As Integer
    	    For i1 = 0 To _h - 1 Step h
    		    ih = Math.Min(h, _h - i1)
    		    %><nobr><%
    		            	     For i2 = 0 To _w - 1 Step w
    		            		     iw = Math.Min(w, _w - i2)
    					%><img style="margin:0 0 0 0;width:<%=iw.ToString()%>px; height: <%=ih.ToString()%>px;" title="d_img.aspx?session=<%=Request.QueryString("session")%>&x=<%=i2.ToString()%>&y=<%=i1.ToString()%>&w=<%=iw.ToString()%>&h=<%=ih.ToString()%>" /><%
    					Next
    		     %></nobr><br /><%
    	    Next
     %>
     <script>
     updateimg();
     </script>
</body>
</html>

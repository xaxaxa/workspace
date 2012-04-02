<%@ Page Language="VB" Buffer="false" %>

<%@ Register Assembly="webcontrollib2" Namespace="webcontrollib2" TagPrefix="cc1" %>

<script runat="server">

</script>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
	<title>Untitled Page</title>
</head>
<body>
	<form id="form1" runat="server">
		<div>
			<cc1:progressbar ID="Progressbar1" runat="server" value="0" Width="100%" Style="border: #000000 1px solid;"
				Height="20px" maximum="1000" /><span id="p"></span><div id="div1"></div>
			<%
				Dim i, ii As Long
				ii = DateDiff(DateInterval.Second, New Date(2008, 9, 1), New Date(2009, 7, 1))
			%>
			
			<script>Progressbar1_setmaximum(<%=ii.ToString()%>);</script>

			<%
				Do
					System.Threading.Thread.Sleep(1000)
					i = DateDiff(DateInterval.Second, New Date(2008, 9, 1), Now())
			%><script>Progressbar1_setvalue(<%=i.ToString()%>);document.getElementById("p").innerHTML=(Progressbar1_getvalue()*100/Progressbar1_getmaximum()).toString()+"%";document.getElementById("div1").innerHTML="<%=i/60/60/24%><br>-----------------<br><%=ii/60/60/24%>";</script><%
				Response.Flush()
			Loop
			%>

			<script>
    //setInterval(function(){Progressbar1_setvalue(Progressbar1_getvalue()+1)},100);
			</script>

		</div>
	</form>
</body>
</html>

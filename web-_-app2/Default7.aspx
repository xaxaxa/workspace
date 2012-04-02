<%@ Page Language="VB" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<script runat="server">

</script>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
	<title>Untitled Page</title>
</head>
<body>
	<form id="form1" runat="server">
		<div>
		</div>
	</form>
</body>
</html>
<form name="form1" method="post" action="Default3.aspx" id="form2">
	<div>
		<input name="Progressbar1_I" id="Progressbar1_I" value="1" type="hidden"><input name="Progressbar1_M"
			id="Progressbar1_M" value="100" type="hidden">
		<input name="Progressbar1_V" id="Progressbar1_V" value="0" type="hidden">
		<div id="Progressbar1" style="border: 1px solid rgb(0, 0, 0); overflow: hidden; width: 411px;
			height: 20px;">
			<div readonly="READONLY" id="Progressbar1_INNER" style="border: medium none; background-color: rgb(0, 0, 255);
				width: 0px; height: 20px;">
			</div>
		</div><span id="p"></span><div id="div1"></div>

		<script>function Progressbar1_getvalue(){return parseInt(document.getElementById("Progressbar1_V").value);}
function Progressbar1_getmaximum(){return parseInt(document.getElementById("Progressbar1_M").value);}
function Progressbar1_setvalue(V){if(V>Progressbar1_getmaximum()){V=Progressbar1_getmaximum();}document.getElementById("Progressbar1_V").value=V.toString();document.getElementById("Progressbar1_INNER").style.width=(V*document.getElementById("Progressbar1").offsetWidth/parseInt(document.getElementById("Progressbar1_M").value)).toString()+"px";}
function Progressbar1_setmaximum(V){document.getElementById("Progressbar1_M").value=V.toString();document.getElementById("Progressbar1_INNER").style.width=(parseInt(document.getElementById("Progressbar1_V").value)*document.getElementById("Progressbar1").offsetWidth/V).toString()+"px";}document.getElementById("Progressbar1_INNER").style.width=(parseInt(document.getElementById("Progressbar1_V").value)*document.getElementById("Progressbar1").offsetWidth/parseInt(document.getElementById("Progressbar1_M").value)).toString()+"px";</script>

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
	</div>
</form>

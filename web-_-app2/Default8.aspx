<%@ Page Language="VB" %>
<%@ Register Assembly="webcontrollib2" Namespace="webcontrollib2" TagPrefix="cc1" %>
<script runat="server">

	
	Protected Sub Button1_Click(ByVal sender As Object, ByVal e As System.EventArgs)
		Me.Window1.Visible = True
	End Sub
</script>
<html>
<head runat="server">
    <title>Untitled Page</title>
    <script src="taskbar.js"></script>
</head>
<body style="overflow:hidden;" onresize="p.layout();" bottommargin="0" leftmargin="0" rightmargin="0" topmargin="0" onload="document.getElementById('tb').dock=DOCK_BOTTOM;document.getElementById('di').dock=DOCK_FILL;document.getElementById('f1').dock=DOCK_FILL;p.layout();">
	  <div style="width:100%; height:100%; overflow:auto;" id="di" runat="server">
	   <div id="tb" style="position:absolute; height:28px;"></div>
<form id="f1" runat="server" style="position:absolute; border:#000000 1px solid;">
<script>
taskbar_init(document.getElementById("tb"),window);
</script>
	    <cc1:window id="Window1" runat="server" height="239px" text="aaaaaa" width="295px" Visible="False" container="f1"></cc1:window>
	<cc1:window ID="Window2" runat="server" Height="181px" Width="182px" container="f1">
	<asp:button />
	</cc1:window>
	<asp:Button ID="Button1" runat="server" OnClick="Button1_Click" Text="Button" />
	   </form>
	   </div>
	   <script>
var p=new panel(document.getElementById("di"),window);</script>
</body>
</html>

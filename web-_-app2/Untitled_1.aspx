<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<%@ Page Language="C#" %>
<html dir="ltr" xmlns="http://www.w3.org/1999/xhtml">

<head runat="server">
<meta content="text/html; charset=utf-8" http-equiv="Content-Type" />
<title>Untitled 1</title>
</head>

<body>

<form id="form1" runat="server">
	<asp:WebPartManager id="WebPartManager1" runat="server">
	</asp:WebPartManager>
	<asp:EditorZone id="EditorZone1" runat="server" BorderColor="Black" BorderStyle="Solid" BorderWidth="1px" Height="540px" Width="520px">
		<ZoneTemplate>
			<asp:LayoutEditorPart id="LayoutEditorPart1" runat="server" />
			<asp:PropertyGridEditorPart id="PropertyGridEditorPart1" runat="server" />
		</ZoneTemplate>
	</asp:EditorZone>
</form>

</body>

</html>

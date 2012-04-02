<%@ Page Language="VB" AutoEventWireup="false"%>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <title>Untitled Page</title>
</head>
<body>
    <form id="form1" runat="server">
    <div>
    <img longdesc="Default2.aspx" border="0" ismap="ismap" usemap="#m" style="width: 250px; height: 194px" />&nbsp;<asp:ImageMap
	    ID="ImageMap1" runat="server" Height="184px" Width="267px">
	    <asp:CircleHotSpot HotSpotMode="PostBack" Radius="100" />
	    <asp:RectangleHotSpot Bottom="160" HotSpotMode="PostBack" Left="20" Right="160" Top="20" />
    </asp:ImageMap><map name="m" id="m"><area coords="0,0,100,100,0,100,0,0" alt="axaxxaxaaxa" href="#" border="1" onclick="alert('xxaxax');" shape="poly" contenteditable="true" style="border:#000000 1px solid; background-color:#000000; color:#000000; cursor:wait;" /></map></div>
    </form>
</body>
</html>

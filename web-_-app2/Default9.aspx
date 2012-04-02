<%@ Page Language="VB" %>

<%@ REGISTER ASSEMBLY="webcontrollib2" NAMESPACE="webcontrollib2"
	TAGPREFIX="cc1" %>

<script runat="server">

	Protected Sub Button1_Click(ByVal sender As Object, ByVal e As System.EventArgs)
		Me.Window1.Visible2 = True
	End Sub

	Protected Sub BUTTON2_Click(ByVal sender As Object, ByVal e As System.EventArgs)
		Me.Window1.Visible2 = False
	End Sub
</script>

<html>
<head runat="server">
    <title>Untitled Page</title>
    
</head>
<body BOTTOMMARGIN="0" LEFTMARGIN="0" RIGHTMARGIN="0" TOPMARGIN="0">
    <form id="form1" runat="server">
    
	    <CC1:WINDOW ID="Window1" RUNAT="server" HEIGHT="232px" WIDTH="323px" VISIBLE2="False"><IFRAME SRC="Default10.aspx" STYLE="width:100%; height:100%;"></IFRAME></CC1:WINDOW>
	   
	   
	    <ASP:TEXTBOX ID="TextBox1" RUNAT="server"></ASP:TEXTBOX>
	        <BUTTON TYPE="button" ONCLICK="Window1.show();">open</BUTTON>
   <BUTTON TYPE="button" ONCLICK="Window1.close();">close</BUTTON>
    </form>
</body>
</html>

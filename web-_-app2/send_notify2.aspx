<%@ Page Language="VB" VALIDATEREQUEST="false" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<script runat="server">

	Protected Sub Page_Load(ByVal sender As Object, ByVal e As System.EventArgs)
		Dim c As Net.HttpWebRequest = Net.HttpWebRequest.Create("http://192.168.101.1:2121/")
		c.Method = "POST"
		'c.Timeout = 1000
		Dim b As Byte() = System.Text.Encoding.Unicode.GetBytes(Request.Form.Get("content"))
		Dim s As IO.Stream = c.GetRequestStream()
		s.Write(b, 0, b.Length)
		s.Flush()
		c.GetResponse()
		c.Abort()
	End Sub
</script>

<html xmlns="http://www.w3.org/1999/xhtml" >
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

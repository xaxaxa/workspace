<%@ PAGE LANGUAGE="VB" VALIDATEREQUEST="false" %>

<%
	Dim fs As New IO.FileStream(Server.MapPath(Request.Form.Get("fn")), IO.FileMode.Create)
	Dim sw As New IO.StreamWriter(fs, System.Text.Encoding.Unicode)
	sw.Write(Request.Form.Get("content"))
	sw.Close()
	'Server.Execute("save_xml.aspx?fn=" + Request.Form.Get("fn") + "&output=" + Request.Form.Get("fn").Replace(".p", ".htm"))
%>
<script>
window.parent.save_callback();
</script>
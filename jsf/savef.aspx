<%@ Page Language="C#" validateRequest="false" %>
<%
	System.IO.FileStream fs=new System.IO.FileStream(Server.MapPath("f/"+Request.Form["fn"]), System.IO.FileMode.Create);
	System.IO.StreamWriter sw=new System.IO.StreamWriter(fs, System.Text.Encoding.Unicode);
	sw.Write(Request.Form["content"]);
	sw.Close();
%>
<script>
window.parent.save_callback();
</script>

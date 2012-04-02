<%@ Page Language="vb" validaterequest="false"%>
<HTML>
	<HEAD>
		<%
	 Try
			response.write("<script>function se(){window.top.ind+=1;document.getElementById(""n"").innerHTML=(window.top.ind);window.top.s(window.top.ind);}</script>")
            Dim m As New Web.Mail.MailMessage
            With m
                .From = Request.form.get("ffr")
                .To = Request.form.get("fto")
                .Body = Request.form.get("fco")
                .Subject = Request.form.get("fsu")
                .BodyFormat = Mail.MailFormat.Html
            End With
            Web.Mail.SmtpMail.SmtpServer = "smtp.1and1.com"
            Web.Mail.SmtpMail.Send(m)
        Catch ex As Exception
            Dim i As Integer, m As String = ex.message, m2 As String = ""
            For i = 0 To m.Length - 1
                m2 += "'+String.fromCharCode(" + Asc(m.Chars(i)).ToString + ")+'"
            Next
            'Response.Write("<script>alert('" + m2 + "');</script>")
        End Try
%>
		
	</HEAD>
	<body bottomMargin="0" leftMargin="0" onload="try{se();}catch(e){}" topMargin="0" rightMargin="0">
		<form action="send_i.aspx" method=post id="sf">
		<input name="fr" id="fr" type=hidden>
		<input name="eto" id="eto" type=hidden>
		<input name="co" id="co" type=hidden>
		<input name="su" id="su" type=hidden>
		<input name="i_s" value="1" type=hidden>
		</form>
		<div id="n" style="BORDER-TOP-WIDTH: 1px; BORDER-LEFT-WIDTH: 1px; BORDER-LEFT-COLOR: #000000; BORDER-BOTTOM-WIDTH: 1px; BORDER-BOTTOM-COLOR: #000000; MARGIN: 0px; WIDTH: 16px; BORDER-TOP-COLOR: #000000; HEIGHT: 8px; BORDER-RIGHT-WIDTH: 1px; BORDER-RIGHT-COLOR: #000000"></div>
	</body>
</HTML>
                                       
                                
                                  
                                                      



<%@ PAGE LANGUAGE="VB" AUTOEVENTWIREUP="false" INHERITS="ajax_lib.AjaxPage" VALIDATEREQUEST="false" %>

<%@ REGISTER ASSEMBLY="webcontrollib2" NAMESPACE="webcontrollib2.Samples.AspNet.VB.Controls"
	TAGPREFIX="cc1" %>
<%@ REGISTER ASSEMBLY="ajax_lib" TAGPREFIX="ajax" NAMESPACE="ajax_lib" %>

<SCRIPT RUNAT="server">

	Protected Sub a_eventraised(ByVal eventname As String)
		Dim i As Integer
		Me.append_text(eventname)
		If eventname = "onclick" Then
			For i = 1 To 10 ' Step -1
				System.Threading.Thread.Sleep(100)
				Me.append_text(Me.TextBox1.Text) '"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
				'Me.execute_script("var d=window.document.createElement(""div"");d.innerHTML=""" + i.ToString() + """;window.document.body.appendChild(d);")
			Next
		End If
	End Sub

	Protected Sub Button1_Click(ByVal sender As Object, ByVal e As System.EventArgs)
		Dim i As Integer
		For i = 1 To 10 ' Step -1
			System.Threading.Thread.Sleep(100)
			Me.append_text(Me.TextBox1.Text) '"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			'Me.execute_script("var d=window.document.createElement(""div"");d.innerHTML=""" + i.ToString() + """;window.document.body.appendChild(d);")
		Next
	End Sub
</SCRIPT>

<HTML XMLNS="http://www.w3.org/1999/xhtml">
<HEAD RUNAT="server">
	<TITLE>Untitled Page</TITLE>
</HEAD>
<BODY>
	<FORM ID="form1" RUNAT="server">
		<AJAX:AJAXBUTTON RUNAT="server" ID="a" WIDTH="79px" ONEVENTRAISED="a_eventraised"
			AJAXEVENTS="onmousedown&#13;&#10;onmouseup&#13;&#10;onclick&#13;&#10;onmouseover&#13;&#10;onmouseout" />
		<ASP:TEXTBOX ID="TextBox1" RUNAT="server"></ASP:TEXTBOX>
		<ASP:BUTTON ID="Button1" RUNAT="server" ONCLICK="Button1_Click" TEXT="Button" />&nbsp;
		<AJAX:AJAX_CONTROL ID="c1" RUNAT="server" HEIGHT="55px" WIDTH="137px" TAG_NAME="button" AJAXEVENTS="onmousedown;onmouseup"></AJAX:AJAX_CONTROL>
	</FORM>
</BODY>
</HTML>

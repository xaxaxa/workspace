<%@page language="vb" validaterequest="false"%>
<html>
	<HEAD>
		<title>sendemail</title>
		<SCRIPT SRC="panel.js"></SCRIPT>
		<script>
var ind=0;
function send(to)
{
//alert(to);
try{
	document.getElementById("ajf").src="send_i.aspx?to="+escape(to)+"&fr="+escape(document.getElementById("fr").value)+"&su="+escape(document.getElementById("su").value)+"&co="+escape(document.getElementById("co").value);
}catch(e){alert(e.message);}
//document.getElementById("ajf").reload();
//alert(document.getElementById("ajf").location.href);
}
		</script>
<script language="javascript" id="clientEventHandlersJS">

var pnl;
function l()
{
	pnl=new panel(document.body,window);
	//set dock
	//document.getElementById("b1").dock=DOCK_TOP;
	document.getElementById("toppanel").dock=DOCK_TOP;
	document.getElementById("toppanel2").dock=DOCK_TOP;
	document.getElementById("co").dock=DOCK_FILL;
	document.getElementById("ct").dock=DOCK_FILL;
	document.getElementById("bottompanel").dock=DOCK_BOTTOM;
	pnl.layout();
	pnl.layout();
}

function d_onclick()
{
	try
	{
		document.getElementById("d").style.backgroundColor="#999999";
		document.getElementById("d").style.border="1px inset";
		document.getElementById("h").style.backgroundColor="transparent";
		document.getElementById("h").style.border="0px inset";
		document.getElementById("p").style.backgroundColor="transparent";
		document.getElementById("p").style.border="0px inset";
		document.getElementById("co").style.display="none";
		document.getElementById("ct").style.display="block";
		document.getElementById("ct").innerHTML=document.getElementById("co").value;
		document.getElementById("ct").contentEditable="true";
	}catch(e){alert(e.message)}
}

function h_onclick() {
	try
	{
		document.getElementById("d").style.backgroundColor="transparent";
		document.getElementById("d").style.border="0px inset";
		document.getElementById("h").style.backgroundColor="#999999";
		document.getElementById("h").style.border="1px inset";
		document.getElementById("p").style.backgroundColor="transparent";
		document.getElementById("p").style.border="0px inset";
		document.getElementById("co").style.display="block";
		document.getElementById("ct").style.display="none";
		document.getElementById("co").value=document.getElementById("ct").innerHTML;
	}
	catch(e){alert(e.message)}
}
		
function p_onclick() {
	try
	{
		document.getElementById("d").style.backgroundColor="transparent";
		document.getElementById("d").style.border="0px inset";
		document.getElementById("h").style.backgroundColor="transparent";
		document.getElementById("h").style.border="0px inset";
		document.getElementById("p").style.backgroundColor="#999999";
		document.getElementById("p").style.border="1px inset";
		document.getElementById("co").style.display="none";
		document.getElementById("ct").style.display="block";
		document.getElementById("ct").innerHTML=document.getElementById("co").value;
		document.getElementById("ct").contentEditable="false";
	}
	catch(e){alert(e.message)}
}
var w,h;
function rs()
{
	//if(w==document.getElementById("d").offsetWidth && h==document.getElementById("d").offsetHeight)return;
	pnl.layout();
	//w=document.getElementById("d").offsetWidth;
	//h=document.getElementById("d").offsetHeight;
}

		</script>
		<%
if not request.form.get("s")=nothing then
        Dim i As Integer
        Dim sjs As String = "function s(i){ind=i;var a=["
        Dim sr As New IO.StreamReader(Request.Files.Get("af").InputStream)
        Dim tmp As String() = sr.ReadToEnd.Split(vbNewLine)
        sr.Close()
        For i = 0 To tmp.Length - 1
			If i > 0 Then
				sjs += ","
			End If
            sjs += "'" + tmp(i).Replace("'", "'+String.fromCharCode(39)+'").Replace("\", "\\").replace(chr(13),"") + "'"
        Next
        sjs += "];document.getElementById('o').innerHTML='of '+a.length;if(i<a.length){send(a[i]);}else{alert('send completed');}"
        Response.Write("<script>" + sjs.replace(chr(10),"") + "}</script>")
end if

%>
	</HEAD>
	<body onload="l();try{s(0);}catch(e){}" ONRESIZE="rs();" style="overflow: hidden; border-top-style: none; border-right-style: none; border-left-style: none; border-bottom-style: none" BOTTOMMARGIN="0" LEFTMARGIN="0" RIGHTMARGIN="0" SCROLL="no" TOPMARGIN="0">
		<form style="WIDTH: 100%; HEIGHT: 100%" id="sf" method="post" action="send_email.aspx" enctype="multipart/form-data">
				<DIV ID="toppanel" STYLE="width:200px; height:30px;"><iframe id="ajf" style="WIDTH: 64px; BORDER-TOP-STYLE: none; BORDER-RIGHT-STYLE: none; BORDER-LEFT-STYLE: none; HEIGHT: 16px; BORDER-BOTTOM-STYLE: none"
			scrolling="no" frameBorder="0"></iframe>
		<DIV id="o" style="DISPLAY: inline; WIDTH: 72px; HEIGHT: 19px" ms_positioning="FlowLayout"></DIV></DIV>
<DIV ID="toppanel2" STYLE="width:900px; height:30px;">
			<input type="hidden" name="s" value="true"> address file:<input type="file" id="af" name="af">
			from:<input name="fr" id="fr" value='<%=iif(request.form.get("s")=nothing,"info@mbotax.com",request.form.get("fr"))%>'>
			subject<input name="su" id="su" value='<%=iif(request.form.get("s")=nothing,"file your tax with netfile today",request.form.get("su"))%>'>
			<br>
			content:
			</DIV>
			<DIV ID="bottompanel" STYLE="width:270px; height:30px;">
		<DIV language="javascript" id="p" style="DISPLAY: inline; WIDTH: 64px; CURSOR: default; HEIGHT: 19px; BACKGROUND-COLOR: #ffffff"
			onclick="return p_onclick()">preview</DIV>
		<DIV language="javascript" id="d" style="DISPLAY: inline; WIDTH: 56px; CURSOR: default; HEIGHT: 19px"
			onclick="return d_onclick()">design</DIV>
		<DIV language="javascript" id="h" style="DISPLAY: inline; WIDTH: 40px; CURSOR: default; HEIGHT: 19px; BACKGROUND-COLOR: #999999"
			onclick="return h_onclick()">html</DIV>
			<button onclick="document.forms['sf'].submit();" type="button">send</button></DIV><!--<textarea id="co" name="co" style="WIDTH:100%; HEIGHT:80%"><%=request.form.get("co")%></textarea>-->
			<DIV id="ct" onblur="document.getElementById('co').value=document.getElementById('ct').innerHTML;"
			contentEditable="true" style="DISPLAY: none; Z-INDEX: 101; OVERFLOW: auto; WIDTH: 100%; HEIGHT: 80%"
			tabIndex="1"></DIV>
		<textarea id="co" name="co" style="WIDTH:100%; HEIGHT:80%" onblur="document.getElementById('ct').innerHTML=document.getElementById('co').value;"
			wrap="off"><%=request.form.get("co")%></TEXTAREA>
		<br>
		
		</form>
<br><br>
		<h1>samples:</h1>
		<br><br>
		<TABLE id="mbotax" cellSpacing="0" cellPadding="0" width="664" align="center" bgColor="#cccccc"
						border="1">
						<TR>
							<TD vAlign="top" width="660" bgColor="#ffffff" colSpan="2">
								<TABLE height="104" cellSpacing="0" cellPadding="0" border="0">
									<TR>
										<TD width="215" background="http://www.mbotax.com/taxadmin/image/h1.gif" height="104"></TD>
										<TD vAlign="bottom" width="275" background="http://www.mbotax.com/taxadmin/image/h2.gif"
											height="104"><BR>
											<IMG style="WIDTH: 8px; HEIGHT: 8px" height="8" src="images/spacer.gif" width="8" border="0"><SPAN style="FONT-WEIGHT: bold; FONT-SIZE: 16px; WIDTH: 219px; COLOR: #ffffff; FONT-FAMILY: Verdana; HEIGHT: 22px">Online 
												Filing Tax Returns</SPAN><SPAN style="FONT-WEIGHT: bold; FONT-SIZE: 20px; WIDTH: 40px; COLOR: #663399; FONT-FAMILY: 'Times New Roman', Arial; HEIGHT: 25px">2007</SPAN>
											<div>&nbsp;</div>
										</TD>
										<TD vAlign="bottom" width="180" background="http://www.mbotax.com/taxadmin/image/h2.gif"
											height="104"><A style="FONT-WEIGHT: bold; TEXT-DECORATION: underline" href="http://www.netfile.gc.ca/software_mbotax-e.html"
												target="_blank"><IMG src="http://www.mbotax.com/taxadmin/image/h3.gif" border="0"></A>
										</TD>
									</TR>
								</TABLE>
							</TD>
						</TR>
						<TR>
							<TD vAlign="top" width="481" bgColor="#ffffff">
								<TABLE cellSpacing="0" cellPadding="3" width="478" border="0">
									<TR>
										<TD><BR>
											<P style="MARGIN-TOP: 10px; FONT-WEIGHT: bold; FONT-SIZE: 16px; MARGIN-LEFT: 8px; MARGIN-RIGHT: 6px; FONT-FAMILY: Arial">File 
												your tax with NETFILE today</P>
											<TABLE style="WIDTH: 102px; HEIGHT: 170px" cellSpacing="0" cellPadding="3" width="102"
												align="right" border="0">
												<TR>
													<TD><IMG style="PADDING-RIGHT: 10px; PADDING-LEFT: 10px; PADDING-BOTTOM: 10px; PADDING-TOP: 10px"
															src="http://www.mbotax.com/taxadmin/image/girl1.jpg"></TD>
												</TR>
											</TABLE>
											<P style="FONT-SIZE: 13px; MARGIN-LEFT: 8px; MARGIN-RIGHT: 6px; FONT-FAMILY: Arial"><A style="FONT-WEIGHT: bold; TEXT-DECORATION: underline" href="http://www.mbotax.com"
													target="_blank">MBOTax</A> is a <A style="FONT-WEIGHT: bold; TEXT-DECORATION: underline" href="http://www.netfile.gc.ca/software_mbotax-e.html"
													target="_blank">NETFILE</A> certified online tax filing application. With 
												MBOTax, filing your tax return has never been so easy !MBOTax, frees you from 
												the hassle of calculating taxes, filling out forms and keeping track of 
												deadlines. With MBOTax, complete your 2007 tax return in about 15 minutes, then 
												simply file it electronically with NETFILE, or print and mail it. You'll get 
												your refund in 10 days ...or even less.<br>
												<br>
												<A style="FONT-WEIGHT: bold; TEXT-DECORATION: underline" href="http://www.mbotax.com"
													target="_blank">MBOTax</A> is a featured online product of <A style="FONT-WEIGHT: bold; TEXT-DECORATION: underline" href="http://www.mbosoft.com"
													target="_blank">MBOSoft</A>. It is easy to use, has a friendly user 
												interface and supports immediate calculation. With the powerful technical 
												support of MBOSoft, you have nothing to worry about.
											</P>
											<p style="FONT-SIZE: 13px; MARGIN-LEFT: 8px; MARGIN-RIGHT: 6px; FONT-FAMILY: Arial"><A style="FONT-WEIGHT: bold; TEXT-DECORATION: underline" href="http://www.mbotax.com">File 
													Tax with NETFILE now</A>
												<br>
											</p>
											<BR>
										</TD>
									</TR>
								</TABLE>
							</TD>
							<TD style="BACKGROUND-COLOR: #ff9900" vAlign="top" width="163" colSpan="1"><A style="FONT-WEIGHT: bold; TEXT-DECORATION: underline" href="http://www.mbotax.com"
									target="_blank"><br>
									<br>
									<IMG src="http://www.mbotax.com/images/freetouse.gif" border="0">
									<br>
									<br>
									<br>
									<IMG src="http://www.mbotax.com/images/offering.gif" border="0"></A></TD>
						</TR>
					</TABLE>
</body>

</html>






























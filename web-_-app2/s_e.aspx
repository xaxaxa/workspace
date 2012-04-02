<%@page language="vb" validaterequest="false"%>
<HTML>
	<HEAD>
		<meta name="vs_targetSchema" content="http://schemas.microsoft.com/intellisense/ie5">
		<title>sendemail</title>
		<SCRIPT SRC="panel.js"></SCRIPT>
		<script>
var ind=0;
var p=false;
function send(to)
{
	//alert(window.location.href.substring(0,window.location.href.lastIndexOf("/"))+"/send_i.aspx?to="+escape(to)+"&fr="+escape(document.getElementById("fr").value)+"&su="+escape(document.getElementById("su").value)+"&co="+escape(document.getElementById("co").value));
	//document.getElementById("ajf").src=window.location.href.substring(0,window.location.href.lastIndexOf("/"))+"/send_i.aspx"
	
	document.getElementById("ffr").value=document.getElementById("fr").value;
	document.getElementById("fsu").value=document.getElementById("su").value;
	document.getElementById("fco").value=document.getElementById("co").value;
	document.getElementById("fto").value=to;
	document.getElementById("fsf").submit();
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
		document.getElementById("ct").style.display="block";
		document.getElementById("ct").innerHTML=document.getElementById("co").value;
		document.getElementById("ct").contentEditable="true";
		document.getElementById("co").style.display="none";
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
		document.getElementById("co").value=document.getElementById("ct").innerHTML;
		document.getElementById("ct").style.display="none";
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
		document.getElementById("ct").style.display="block";
		document.getElementById("ct").innerHTML=document.getElementById("co").value;
		document.getElementById("ct").contentEditable="false";
		document.getElementById("co").style.display="none";
	}
	catch(e){alert(e.message)}
}
function rs()
{
	//if(w==document.getElementById("d").offsetWidth && h==document.getElementById("d").offsetHeight)return;
	pnl.layout();
	//w=document.getElementById("d").offsetWidth;
	//h=document.getElementById("d").offsetHeight;
}
function BUTTON1_onclick()
{
	window.open("samples.htm","samples","toolbars=1,resizable=1,scrollbars=1");
}

		</script>
		<%
if not request.form.get("s")=nothing then
        Dim i As Integer
        Dim sjs As String = "function s(i){if(p){return;}ind=i;var a=["
        Dim sr As New IO.StreamReader(Request.Files.Get("af").InputStream)
        Dim tmp As String() = sr.ReadToEnd.Split(CChar(vbLf))
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
	<body onload="l();setTimeout('try{s(0);}catch(e){}',1000);" ONRESIZE="rs();" style="overflow: hidden; border-top-style: none; border-right-style: none; border-left-style: none; border-bottom-style: none; background-color: #d4d0c8;" BOTTOMMARGIN="0" LEFTMARGIN="0" RIGHTMARGIN="0" SCROLL="no" TOPMARGIN="0">
	<form action="send_i.aspx" method=post id="fsf" target="ajf">
		<input name="ffr" id="ffr" type=hidden>
		<input name="fto" id="fto" type=hidden>
		<input name="fco" id="fco" type=hidden>
		<input name="fsu" id="fsu" type=hidden>
		</form>
		<form style="WIDTH: 100%; HEIGHT: 100%" id="sf" method="post" action="s_e.aspx" enctype="multipart/form-data">
			<DIV ID="toppanel" STYLE="width:200px; height:30px;"><iframe id="ajf" name="ajf" style="WIDTH: 64px; BORDER-TOP-STYLE: none; BORDER-RIGHT-STYLE: none; BORDER-LEFT-STYLE: none; HEIGHT: 20px; BORDER-BOTTOM-STYLE: none"
				scrolling="no" frameBorder="0"></iframe>
				<DIV id="o" style="DISPLAY: inline; WIDTH: 72px; HEIGHT: 19px" ms_positioning="FlowLayout"></DIV>
<button onclick="document.forms['fsf'].submit();">resubmit</button></DIV>
<DIV ID="toppanel2" STYLE="width:900px; height:30px;">
			<input type="hidden" name="s" value="true"> address file:<input value='<%=request.form.get("af")%>' type="file" id="af" name="af">
			from:<input name="fr" id="fr" value='<%=iif(request.form.get("s")=nothing,"info@mbotax.com",request.form.get("fr"))%>'>
			subject<input name="su" id="su" value='<%=iif(request.form.get("s")=nothing,"File your tax with NETFILE today",request.form.get("su"))%>'>
			<br>
			content:</DIV>
			<!--<textarea id="co" name="co" style="WIDTH:100%; HEIGHT:80%"><%=request.form.get("co")%></textarea>-->
		<DIV ID="bottompanel" STYLE="width:306px; height:30px;">
		<DIV language="javascript" id="p" style="DISPLAY: inline; WIDTH: 64px; CURSOR: default; HEIGHT: 19px;"
			onclick="return p_onclick()">preview</DIV>
		<DIV language="javascript" id="d" style="DISPLAY: inline; WIDTH: 56px; CURSOR: default; HEIGHT: 19px"
			onclick="return d_onclick()">design</DIV>
		<DIV language="javascript" id="h" style="DISPLAY: inline; WIDTH: 40px; CURSOR: default; HEIGHT: 19px; BACKGROUND-COLOR: #999999"
			onclick="return h_onclick()">html</DIV>
			<button onclick="document.forms['sf'].submit();" type="button">send</button>
			<button type="button" ID="BUTTON1" LANGUAGE="javascript" onclick="return BUTTON1_onclick()">
				samples...</BUTTON></DIV>
			<DIV id="ct" onblur="document.getElementById('co').value=document.getElementById('ct').innerHTML;"
			contentEditable="true" style="DISPLAY: none; Z-INDEX: 101; OVERFLOW: auto; WIDTH: 100%; HEIGHT: 80%; background-color: #ffffff;"
			tabIndex="1"></DIV>
		<textarea id="co" name="co" style="WIDTH:100%; HEIGHT:80%" onblur="document.getElementById('ct').innerHTML=document.getElementById('co').value;"
			wrap="off"><%=request.form.get("co")%></TEXTAREA>
		<br>

		</form><!--
		<br>
		<br>
		<h1>samples:</h1>
		<br>
		<br>
		
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
<br><br>
	<TABLE id="mbotax1" cellSpacing="0" cellPadding="0" width="664" align="center" bgColor="#cccccc"
			border="1">
			<TBODY>
				<TR>
					<TD vAlign="top" width="660" bgColor="#ffffff">
						<TABLE height="104" cellSpacing="0" cellPadding="0" border="0" ID="Table1">
							<TBODY>
								<TR>
									<TD width="215" background="http://www.mbotax.com/taxadmin/image/h1.gif" height="104"></TD>
									<TD vAlign="bottom" width="275" background="http://www.mbotax.com/taxadmin/image/h2.gif"
										height="104" align="center"><BR>
										<SPAN style="FONT-WEIGHT: bold; FONT-SIZE: 16px; WIDTH: 219px; COLOR: #ffffff; FONT-FAMILY: Verdana; HEIGHT: 22px">
														Online Filing Tax Returns</SPAN><SPAN style="FONT-WEIGHT: bold; FONT-SIZE: 20px; WIDTH: 40px; COLOR: #663399; FONT-FAMILY: 'Times New Roman', Arial; HEIGHT: 25px">2007</SPAN>
										<DIV>&nbsp;</DIV>
									</TD>
									<TD vAlign="bottom" width="180" background="http://www.mbotax.com/taxadmin/image/h2.gif"
										height="104"><A style="FONT-WEIGHT: bold; TEXT-DECORATION: underline" href="http://www.netfile.gc.ca/software_mbotax-e.html"
											target="_blank"><IMG src="http://www.mbotax.com/taxadmin/image/h3.gif" border="0"></A>
									</TD>
								</TR>
							</TBODY>
						</TABLE>
					</TD>
				</TR>
				<TR>
					<TD vAlign="top" width="481" bgColor="#ffffff">
						<TABLE cellSpacing="0" cellPadding="3" width="648" border="0" style="WIDTH: 648px; HEIGHT: 293px"
							ID="Table2">
							<TBODY>
								<TR>
									<TD><BR>
										<P style="MARGIN-TOP: 10px; FONT-WEIGHT: bold; FONT-SIZE: 16px; MARGIN-LEFT: 8px; MARGIN-RIGHT: 6px; FONT-FAMILY: Arial">File 
											your tax with NETFILE today</P>
										<P style="FONT-SIZE: 13px; MARGIN-LEFT: 8px; MARGIN-RIGHT: 6px; FONT-FAMILY: Arial">
											<A style="FONT-WEIGHT: bold; TEXT-DECORATION: underline" href="http://www.netfile.gc.ca/software_mbotax-e.html"
												target="_blank">NETFILE</A> is an&nbsp;electronic tax-filing service of 
											CRA. This transmission service allows you to file your personal income tax and 
											benefit return directly to the Canada Revenue Agency (CRA) using the Internet.
											<BR>
											<BR>
											NETFILE is intended for the millions of individuals who use commercial software 
											to manage their financial affairs and prepare their tax returns. These 
											individuals can transmit their own tax returns using the CRA's free Internet 
											transmission service, NETFILE. In the past, they had to print and mail their 
											tax returns.<BR>
											<br>
											<A style="FONT-WEIGHT: bold; TEXT-DECORATION: underline" href="http://www.mbotax.com"
												target="_blank">MBOTax</A> is a NETFILE certified online tax filing 
											software. With MBOTax, filing your tax return has never been so easy ! MBOTax, 
											frees you from the hassle of calculating taxes, filling out forms and keeping 
											track of deadlines. With MBOTax, complete your 2007 tax return in about 15 
											minutes, then simply file it electronically with NETFILE. You'll get your 
											refund in 10 days ...or even less.
											<br>
											<br>
											The NETFILE transmission service to electronically file your 2007 personal 
											income tax and benefit return is open from February 11, 2008, until September 
											30, 2008.
										</P>
										<P style="FONT-SIZE: 13px; MARGIN-LEFT: 8px; MARGIN-RIGHT: 6px; FONT-FAMILY: Arial"><A style="FONT-WEIGHT: bold; TEXT-DECORATION: underline" href="http://www.mbotax.com">File 
												Tax with NETFILE now</A>
											<BR>
										</P>
										<BR>
									</TD>
								</TR>
							</TBODY>
						</TABLE>
					</TD>
				</TR>
			</TBODY>
		</TABLE>
-->
	</body>
</HTML>







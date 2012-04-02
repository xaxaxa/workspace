<%@ Page Language="VB" Buffer="false" %>

<%@ Register Assembly="webcontrollib2" Namespace="webcontrollib2" TagPrefix="cc1" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<script runat="server">

</script>

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <title>Untitled Page</title>
</head>
<body style="cursor: default;">
    <form id="form1" runat="server">
    <div>
	    <cc1:progressbar ID="Progressbar1" runat="server" Width="100%" maximum="100" Style="border: #000000 1px solid;" Height="20px" />
    <%
    	    Do
    		    Dim i As Integer
    		    For i = 0 To 100
    			    %><script>Progressbar1_setvalue(<%=i.ToString()%>);</script><%
    			                                                                	 Response.Flush()
    			                                                                	 System.Threading.Thread.Sleep(30)
    		    Next
    	    Loop
     %>
    </div>
    </form>
</body>
</html>

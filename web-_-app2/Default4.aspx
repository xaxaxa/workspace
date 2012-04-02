<%@ Page Language="VB" ValidateRequest="False" EnableEventValidation="False" %>

<%@ Register Assembly="webcontrollib2" Namespace="webcontrollib2.Samples.AspNet.VB.Controls"
	TagPrefix="cc3" %>

<%@ Register Assembly="webcontrollib2" Namespace="webcontrollib2" TagPrefix="cc2" %>

<%@ Register Assembly="WebControlLib" Namespace="WebControlLib" TagPrefix="cc1" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<script runat="server">

	Protected Sub Multilist1_checkchange(ByVal item As WebControlLib.multilistitem)
		item.text += "a"
	End Sub
</script>

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <title>Untitled Page</title>
</head>
<body>
    <form id="form1" runat="server">
	    <cc3:QuickContacts ID="QuickContacts1" runat="server" Contacts-Capacity="4" Height="158px"
		    Width="164px"><cc3:Contact Phone="xaa" Email="aaaa" Name="xaaxaxa"></cc3:Contact>
<cc3:Contact Phone="aaa" Email="a" Name="aaaaa"></cc3:Contact>
</cc3:QuickContacts>
	    <asp:Button ID="Button1" runat="server" Text="Button" />
	    <cc1:multilist ID="Multilist1" runat="server" Height="151px" itemdata="aaa:xxxxxx:0;bbb:zzzzzz:1"
		    Width="218px" Oncheckchange="Multilist1_checkchange" autopostback="True" />
	    &nbsp;<br />
	    <cc2:richtextbox id="Richtextbox1" runat="server" height="264px" width="568px" HTMLVIEW="False" showDesignHTML="True" TEXT="<DIV></DIV>"></cc2:richtextbox>
	    &nbsp;&nbsp;&nbsp;
	    <asp:CheckBoxList ID="CheckBoxList1" runat="server" AutoPostBack="True" BorderStyle="Inset"
		    BorderWidth="2px" RepeatColumns="20">
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
		    <asp:ListItem></asp:ListItem>
	    </asp:CheckBoxList>
	    <cc2:multilist ID="Multilist2" runat="server" Height="215px" items-Capacity="8" Width="256px" autopostback="False" displaycheck="False">
		     <cc2:multilistitem checked="False" text="xaxaxa" value="a" />
		    <cc2:multilistitem checked="False" text="asasasasasasa" value="b" />
		    <cc2:multilistitem checked="False" text="aassasassasa" value="ass" />
		    <cc2:multilistitem checked="False" text="aaaaaaaaaaaaa" value="aaaaaaa" />
		    <cc2:multilistitem checked="False" text="zzzzzz" value="zzzzzz" />
		    <cc2:multilistitem checked="False" text="zazazaaza" value="za" />
		   
	    </cc2:multilist>
    </form>
</body>
</html>

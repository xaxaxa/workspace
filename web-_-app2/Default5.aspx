<%@ Page Language="VB"%>

<%@ Register Assembly="webcontrollib2" Namespace="webcontrollib2" TagPrefix="cc1" %>
<%@ Register Assembly="webcontrollib2" Namespace="webcontrollib2.Samples.AspNet.VB.Controls"
	TagPrefix="cc3" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html  >
  <head id="Head1" runat="server">
    <title>
      QuickContacts test page
    </title>
  </head>
  <body>
    <form id="Form1" runat="server">
      <cc3:QuickContacts ID="QuickContacts1" Runat="server" 
        BorderStyle="Solid" BorderWidth="1px">
        <cc3:Contact Name="someone" Email="someone@example.com" 
          Phone="(555) 555-0100"/>
        <cc3:Contact Name="jae" Email="jae@fourthcoffee.com" 
          Phone="(555) 555-0101"/>
        <cc3:Contact Name="lene" Email="lene@contoso.com" 
          Phone="(555) 555-0102"/>        
      </cc3:QuickContacts>
	    &nbsp;
	    <cc1:multilist ID="Multilist1" runat="server" Height="215px" items-Capacity="8" Width="256px" autopostback="True" displaycheck="True">
		     <cc1:multilistitem checked="False" text="xaxaxa" value="a" />
		    <cc1:multilistitem checked="False" text="asasasasasasa" value="b" />
		    <cc1:multilistitem checked="False" text="aassasassasa" value="ass" />
		    <cc1:multilistitem checked="False" text="aaaaaaaaaaaaa" value="aaaaaaa" />
		    <cc1:multilistitem checked="False" text="zzzzzz" value="zzzzzz" />
		    <cc1:multilistitem checked="False" text="zazazaaza" value="za" />
		   
	    </cc1:multilist>
    </form>
  </body>
</html>

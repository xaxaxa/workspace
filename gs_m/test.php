<?php
class asdf
{
	public $a1=1;
	public $a2=5;
}
session_start();
$tmp=$_SESSION["tmp"];
if($tmp==null)$tmp=new asdf;
else $tmp->a1++;
echo "a1 = " . $tmp->a1 . "<br />a2 = " . $tmp->a2;
$_SESSION["tmp"]=$tmp;
?>

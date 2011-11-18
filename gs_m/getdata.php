<?php
$req=curl_init("http://24.83.192.246:12480/gs_m/getdata.aspx?" . $_SERVER['QUERY_STRING']);
curl_setopt($req, CURLOPT_HEADER, 0);
curl_setopt($req, CURLOPT_POST, 1);
curl_setopt($req, CURLOPT_RETURNTRANSFER,0);
curl_setopt($req, CURLOPT_POSTFIELDS,"id=" . $_POST["id"]);
curl_setopt($req,CURLOPT_HTTPHEADER,array('Content-type: application/x-www-form-urlencoded'));
curl_exec($req);
curl_close($req);

?>

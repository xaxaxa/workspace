<?php
$req=curl_init("http://xa.us.to/grooveshark_m/list.aspx?" . $_SERVER['QUERY_STRING']);
curl_setopt($req, CURLOPT_HEADER, 0);
curl_setopt($req, CURLOPT_RETURNTRANSFER,0);
curl_exec($req);
curl_close($req);

?>

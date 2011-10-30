<?php

function readHeader($req, $header)
{
$a=strpos($header,":");
if(strncasecmp(substr($header,0,$a),"location",8)==0)
{
$u=trim(substr($header,$a+1));
if($_GET["download"]=="1")
{
$out = fopen("s/" . $_GET["id"] . ".mp3", 'wb');
$req=curl_init($u);
curl_setopt($req, CURLOPT_HEADER, 0);
curl_setopt($req, CURLOPT_FILE, $out);
curl_exec($req);
curl_close($req);
}
else
{
Header( "HTTP/1.1 302 Moved Temporarily" );
Header( "Location: " . $u);
die();
}
}
return strlen($header);
}


if($_GET["download"] != "1")
{
if(file_exists("s/" . $_GET["id"] . ".mp3"))
{
Header( "HTTP/1.1 302 Moved Temporarily" );
Header( "Location: ./s/" . $_GET["id"] . ".mp3");
die();
}
}
$req=curl_init("http://xa.us.to/grooveshark_m/play.aspx?" . $_SERVER['QUERY_STRING']);
curl_setopt($req, CURLOPT_HEADER, 0);
curl_setopt($req, CURLOPT_RETURNTRANSFER,1);
curl_setopt($req, CURLOPT_FOLLOWLOCATION,0);
curl_setopt($req, CURLOPT_HEADERFUNCTION, "readHeader");
curl_exec($req);
curl_close($req);

?>


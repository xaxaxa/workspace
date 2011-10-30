<?php
session_start();
class gs_client
{
	public $sessionId;
	public $key;
	public $communicationToken;
	public $isConnected=false;
	public $country;
	public $cowbell;
	public function connect()
	{
		$req=curl_init("http://listen.grooveshark.com/");
		curl_setopt($req, CURLOPT_HEADER, 0);
		curl_setopt($req, CURLOPT_RETURNTRANSFER,1);
		curl_setopt($req, CURLOPT_HEADERFUNCTION, array(&$this,"readHeader"));
		curl_setopt($req, CURLOPT_HTTPHEADER, array("Expect:"));
		$response=curl_exec($req);
		curl_close($req);
		/****str5: $response*/
		$str2 = "country: '";
		$num = stripos($response,$str2); //str5.IndexOf (str2);
		/*if (num <= 0) {
			throw new Exception ("Country not found!");
		}*/
		
		$this->country=urldecode(
		substr ($response,
			($num + strlen($str2)), 
			stripos($response, "'", $num + strlen($str2)) - ($num + strlen($str2))
		)
		);
		$this->key = md5($this->sessionId);
		
		$str3 = "main.swf?cowbell=";
		$num2 = stripos($response,$str3);
		$this->cowbell = substr(
			$response,
			($num2 + strlen($str3)),
			stripos($response, "\"", ($num2 + strlen($str3)) - 1) - ($num2 + strlen($str3)));
			
	}
	public function readHeader($req, $header)
	{
		//echo $header;
		$tmp=$this->extractCustomHeader('PHPSESSID=', ';', $header);
		if($tmp)$this->sessionId = trim($tmp);
		return strlen($header);
	}
	private function extractCustomHeader($start,$end,$header)
	{
		$pattern = '/'. $start .'(.*?)'. $end .'/';
		if (preg_match($pattern, $header, $result))
		{
			return $result[1];
		} else return false;
	}
	public function setreqheaders($req)
	{
		curl_setopt($req, CURLOPT_HTTPHEADER, 
		array("Expect:",
		"User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; de; rv:1.9.0.19) Gecko/2010031422 Firefox/3.0.19 GTB7.0 (.NET CLR 3.5.30729)",
		"Referer: http://listen.grooveshark.com/main.swf?cowbell=" . $this->cowbell,
		"Cookie: PHPSESSID=" . $this->sessionId,
		"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8",
		"Content-Type: text/html; charset=UTF-8",
		"Accept-Language: de-de,de;q=0.8,en-us;q=0.5,en;q=0.3",
		"Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7"
		)
		);
		curl_setopt($req, CURLOPT_POST, 1);
	}
}

class groovefix
{
	const $client="gslite";
	const $revision="20100831.08";
	const $randomizer=":quitStealinMahShit:";
}
$asdf=new gs_client;
$asdf->connect();
echo $asdf->cowbell;
?>

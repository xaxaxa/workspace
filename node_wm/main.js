var http = require('http');
require('fs');
require('path');
require('tty');
var static = require('node-static')
//var pg=require('pg');

var sserver = new static.Server("./static/", {
  cache: 600,
  headers: { 'X-Powered-By': 'node-static'}
});
var apps={};
var max_id=0;
var httpProxy = require('http-proxy');
var proxy = new httpProxy.RoutingProxy();
http.createServer(function (request, response) {
	try {
		//response.writeHead(200, {'Content-Type': 'text/plain'});
		var url=require('url').parse(request.url,true);
		//console.log(request.url);
		var s=url.pathname.split("/");
		var i=url.pathname.indexOf("/",1);
		switch(s[1]) {
			case "static":
			{
				request.addListener("end", function () {
					//console.log(url.pathname.substr(i));
					sserver.serveFile(url.pathname.substr(i),200,{},request, response)
						.addListener('error',function(err){response.end(JSON.stringify(err));});
				});
				break;
			}
			case "app":
			{
				proxy.proxyRequest(request, response, {
					host: 'localhost',
					port: apps[s[2]].port
				});
				break;
			}
			case "clientAPI":
			{
				switch(s[2]) {
					case "getWindows":
					{
						tmp=[];
						for(a in apps) {
							tmp[tmp.length]={"id":a,"param":apps[a].wndparam};
						}
						response.end(JSON.stringify(tmp));
						break;
					}
					case "updateWindow":
					{
						var id=parseInt(url.query["id"]);
						apps[id].wndparam=JSON.parse(url.query("data"));
						break;
					}
					case "closeWindow":
					{
						var id=parseInt(url.query["id"]);
						var r=http.request(apps[id].shutdownURL);
						delete apps[id];
						break;
					}
				}
				break;
			}
			case "serverAPI":
			{
				switch(s[2]) {
					case "registerApp":
					{
						var tmp={"wndparam": {}, "shutdownURL":url.query["shutdown"], "port": parseInt(url.query["port"])};
						var id=(++max_id);
						apps[id]=tmp;
						response.end(id.toString());
						break;
					}
					case "unregisterApp":
					{
						delete apps[parseInt(url.query["id"])];
						break;
					}
				}
				break;
			}
			default:
				response.end();
		}
		
	} catch(e) {
		try{response.end();}catch(e1){}
	}
}).listen(12580);


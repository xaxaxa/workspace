a="";
access_token_generation=1;
function fbobj(obj,connection,src,tmp_obj)
{
	this.obj=obj;
	this.connection=connection;
	this.src=src;
	this.tmp_obj=tmp_obj;
};
fbobj.prototype.save=function()
{
	var tmp=(this.src==null)?JSON.stringify(this.obj):this.src;
	lib.save(this.obj.id,tmp);
	this.link();
};
fbobj.prototype.link=function()
{
	if(this.connection) {
		var dir=this.connection.replace(/\//g,".").replace(/^\.+|\.+$/g, "");
		if(this.tmp_obj)
			if(!this.tmp_obj.createdDir) {
				try {
					lib.mkdir(dir);
				} catch(e) {}
				this.tmp_obj.createdDir=true;
			}
		try {
			lib.symlink("../"+this.obj.id, dir+"/"+this.obj.id);
		} catch(e) {}
	}
};
fbobj.prototype.downloadField=function(fieldname)
{
	var fn=this.obj.id.toString()+"."+fieldname;
	download(this.obj[fieldname],fn);
};
function on_error(obj)
{
	if(obj.error.type=="OAuthException") {
		if((!(typeof noprompt === "undefined")) && noprompt) throw Error(obj.error.message);
		else {
			print("the access token has expired or is invalid; please enter a new one:",0);
			s=lib.prompt();
			if(s==null || s.length==0) throw Error("user cancelled");
			a=s;
			access_token_generation++;
		}
	} else throw Error(obj.error.message);
}
function patch_arg(url,n,v)
{
	var i;
	if((i=url.indexOf("?"))<0) {
		return url+"?"+n+"="+encodeURIComponent(v);
	} else if((url.indexOf(n,i)==i+1) || ((i=url.indexOf("&"+n,i))>=0)) {
		i+=n.length+1;//("&"+n).length;
		var ii=url.indexOf("&",i);
		if(ii<0) {
			return url.substring(0,i)+"="+encodeURIComponent(v);
		} else {
			return url.substring(0,i)+"="+encodeURIComponent(v)+url.substring(ii);
		}
	} else {
		//return [i,url.indexOf(n)];
		return url+"&"+n+"="+encodeURIComponent(v);
	}
}
function do_dump(url, cb, follow, path, tmp_obj, traps)
{
	var g=access_token_generation;
	url=patch_arg(url,"access_token",a);
	lib.print("getting: "+url);
	if(traps && "onrequest" in traps)traps.onrequest(url);
	lib.get(url, function(s)
	{
		//lib.print(s);
		if(traps && "onresponse" in traps)traps.onresponse(url);
		d=JSON.parse(s);
		if(d===false)return;
		if("error" in d) {
			while(true) {
				if(access_token_generation>g) {
					do_dump(url,cb,follow,path,tmp_obj,traps);
					return;
				}
				on_error(d);
			}
		} else if("data" in d) {
			for(var i=0;i<d.data.length;i++) {
				//lib.print(i);
				if(cb) cb(new fbobj(d.data[i],path,null,tmp_obj));
			}
			if(follow && "paging" in d && d.paging.next!=null)
				do_dump(d.paging.next,cb,true,path,tmp_obj,traps);
		} else {
			if(cb) cb(new fbobj(d,null,s));
		}
		if(traps && "oncomplete" in traps)traps.oncomplete(url);
	});
}
function dump(p,cb,follow,traps)
{
	try{ p=p.replace(/^\/+|\/+$/g, ""); } catch(e){}
	if(follow==null)follow=true;
	do_dump("https://graph.facebook.com/"+p,cb,follow,p,Object(),traps);
}
function exists(id)
{
	return lib.exists(id);
}
function download(url,fn)
{
	return lib.download(url,fn,function(n){});
}
function saveObj(o)
{
	o.save();
}
function print(msg,prio/*lower means greater priority; <=5 will always be displayed*/)
{
	if(prio==null || prio>2)lib.print(msg);
	else lib.print("\x1B[41;1;33m"+msg+"\x1B[0;0;0m");
}

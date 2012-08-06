a="";
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
function do_dump(url, cb, follow, path, tmp_obj)
{
	lib.print(url);
	lib.get(url, function(s)
	{
		//lib.print(s);
		d=JSON.parse(s);
		if("data" in d) {
			for(var i=0;i<d.data.length;i++) {
				//lib.print(i);
				if(cb) cb(new fbobj(d.data[i],path,null,tmp_obj));
			}
			if(follow && "paging" in d && d.paging.next!=null)
				do_dump(d.paging.next,cb,true,path,tmp_obj);
		} else {
			if(cb) cb(new fbobj(d,null,s));
		}
	});
}
function dump(p,cb,follow)
{
	p=p.replace(/^\/+|\/+$/g, "");
	if(follow==null)follow=true;
	do_dump("https://graph.facebook.com/"+p+"?access_token="+a,cb,follow,p,Object());
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


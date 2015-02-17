
var help="usage: dumpvideos.js -o profile=id_or_username"
if(!("profile" in this))
{
	lib.print("'profile' undefined. "+help);
	lib.exit();
}
var func=function(o)
{
	o.save();
	var v=o.obj.source;
	if(v) {
		var fn=o.obj.id.toString()+".source";
		if(!exists(fn)) download(v,fn);
	}
	if(o.obj.picture) {
		var fn=o.obj.id.toString()+".picture";
		if(!exists(fn)) download(o.obj.picture,fn);
	}
};
dump("/"+profile+"/videos", func,true);
dump("/"+profile+"/videos/uploaded", func,true);


var help="usage: dumpvideos.js -o profile=id_or_username"
if(!("profile" in this))
{
	lib.print("'profile' undefined. "+help);
	lib.exit();
}
dump("/"+profile+"/videos", function(o)
{
	o.save();
	var v=o.obj.source;
	if(v) {
		var fn=o.obj.id.toString()+".source";
		if(!exists(fn)) download(v,fn);
	}
},true);

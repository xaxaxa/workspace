function getLargestPicture(obj)
{
	if("images" in obj) {
		var img=null;
		for(var i=0;i<obj.images.length;i++)
			if(img==null || 
				(obj.images[i].width*obj.images[i].height) >
				(obj.images[img].width*obj.images[img].height))
				img=i;
		if(img!=null && obj.images[img].source!=null)
			return ["images."+img.toString(),obj.images[img].source];
		else return null;
	} else if("source" in obj) {
		return ["source",obj.source];
	}
}
var help="usage: dumpposts.js -o profile=id_or_username"
if(!("profile" in this))
{
	lib.print("'profile' undefined. "+help);
	lib.exit();
}
dump("/"+profile+"/feed", function(o)
{
	o.save();
	if("object_id" in o.obj)
		dump(o.obj.object_id,function(o)
		{
			var img=getLargestPicture(o.obj);
			if(img) {
				var fn=o.obj.id.toString()+"."+img[0];
				if(!exists(fn)) download(img[1],fn);
			}
		});
	dump(o.obj.id+"/comments",saveObj);
},true);

//lib.print("done");

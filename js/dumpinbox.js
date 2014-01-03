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
var help="usage: dumpinbox.js"

var fn1=function(o)
{
	o.save();
	dump("/"+o.obj.id+"/comments", function(o){o.save();}, true);
};
dump("/me/inbox", fn1,true);
//lib.print("done");


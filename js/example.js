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
dump("/me/photos", function(o)
{
	//lib.print(o.obj.id);
	o.save();
	//dump(o.obj.id,saveObj);
	//o.downloadField(""
	var img=getLargestPicture(o.obj);
	var fn=o.obj.id.toString()+"."+img[0];
	if(img && !exists(fn)) download(img[1],fn);
},true);

//lib.print("done");

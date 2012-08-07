
dump("/me/friends", function(o)
{
	if(!exists(o.obj.id))
		dump(o.obj.id,saveObj);
},true);

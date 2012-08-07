reqs=0;
friends={};
//var traps={"onrequest":function(){reqs++;}, "oncomplete":function(){reqs--;if(reqs<=0)all_done();}};
function all_done()
{
	lib.ls("me.friends",function(fn)
	{
		if(fn.length<=0 || fn[0]==".") return;
		try {
			obj=JSON.parse(lib.load(fn));
			if(!(obj.id in friends)) {
				print(obj.name+" ("+obj.id+") deleted",1);
			}
		} catch(e) {
			lib.print(e);
		}
	});
}

dump("/me/friends", function(o)
{
	o.link();
	friends[o.obj.id]=true;
	if(!exists(o.obj.id)) {
		print(o.obj.name+" ("+o.obj.id+") added",3);
		dump(o.obj.id,saveObj,true);
	}
},true,{"oncomplete":all_done});


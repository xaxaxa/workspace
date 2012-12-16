var help="usage: dumpposts.js -o profile=id_or_username"
if(!("profile" in this))
{
	this.profile="me";
}
dump("/"+profile+"/feed", function(o)
{
	lib.print("aaaaa "+o.obj.id);
},true);

//lib.print("done");

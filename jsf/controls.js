//js

//ctor
function control(obj)
{
	
}
//ctor
function container(obj)
{
	
}

//ctor
function ctrl_lib(obj)
{
	obj.ctrls={};
	obj.register=function(name,cls) {
		this.ctrls[name]=cls;
	};
	obj.create=function(name) {
		obj=new Object();
		return this.ctrls[name](obj);
	};
}


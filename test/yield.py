import sys;
def func():
	for i in xrange(10000000):
		yield i;

class iter:
	def read(self):
		if(self.i<10000000):
			self.val=self.i;
			self.i+=1;
			return True;
		return False;
def func2():
	a=iter();
	a.i=0;
	return a;
	
x=0;
if(len(sys.argv)>1):
	for i in func():
		x+=i;
else:
	tmp=func2();
	while tmp.read():
		x+=tmp.val;
	
print x;

import pylab as plt
import numpy as np
import thread


plt.ion()
plt.axis([600,1000,-80,10])
hl, = plt.plot([1,2], [5,6])

#plt.show()
arr=[];
arr2=[];

try:
	while True:
		line=raw_input()
		if len(line)==0:
			print "plotting"
			hl.set_xdata(arr2)
			hl.set_ydata(arr)
			arr=[]
			arr2=[]
			plt.draw()
		else:
			f,v=line.split()
			arr.append(float(v));
			arr2.append(float(f)/1000000);
except EOFError: pass



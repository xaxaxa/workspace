from numpy import sin, linspace, pi
from pylab import plot, show, title, xlabel, ylabel, subplot
from scipy import fft, arange

arr=[];
arr2=[];
try:
	while True:
		f,v=raw_input().split()
		arr.append(float(v));
		arr2.append(float(f)/1000);
except: pass

plot(arr2,arr,'r')
xlabel('Freq (kHz)')
ylabel('|Y(freq)|')

show()

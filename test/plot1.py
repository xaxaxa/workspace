from numpy import sin, linspace, pi
from pylab import plot, show, title, xlabel, ylabel, subplot
from scipy import fft, arange

arr=[];
try:
	while True:
		arr.append(float(raw_input()));
except: pass

plot(range(len(arr)),arr,'r')
xlabel('Freq (kHz)')
ylabel('|Y(freq)|')

show()

#!/usr/bin/python
import Image
import os,sys
from math import *
if(len(sys.argv)<5):
	print "usage: graph2d Xmax Ymax function(of x,y) out.png"
	exit(1);
Xmax=int(sys.argv[1]);
Ymax=int(sys.argv[2]);
W=Xmax*2+1;
H=Ymax*2+1;
F=eval("lambda x,y:"+sys.argv[3]);
img = Image.new('RGB', (W,H), "#000000")
imgp=img.load();
for x in xrange(W):
	for y in xrange(H):
		v=int(F(x-Xmax,Ymax-y)*255.0);
		if v>=0:
			imgp[x,y]=(v,0,0);
		else: imgp[x,y]=(0,-v,0);

img.save(sys.argv[4])

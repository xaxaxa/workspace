#!/usr/bin/env python
import sys;
while True:
	l=sys.stdin.readline();
	if(l==None or len(l)==0): break;
	if(len(l.strip())<=0): continue;
	sys.stdout.write(l);


#!/usr/bin/python

import sys
def main():
	
	f=sys.stdin
	#cdef long i

	i=0;
	while True:
		s=f.readline()
		if len(s)<=0: break
		i+=1
	print i

if __name__ == '__main__':
	main()

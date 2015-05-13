#!/usr/bin/python
import os,sys,time,subprocess,random,select;

if len(sys.argv)<2:
	print "usage: "+sys.argv[0]+" ssh_command [cat|CUSTOM_COMMAND]";
	exit(1);


CONNECT_WAIT=5;
PING_INTERVAL=5;
PING_FAIL_INTERVAL=0;
PING_TIMEOUT=2;
PING_MAXFAIL=5;
EXCEPTION_WAIT=1;

# timeout is in seconds
def checkAlive(p,timeout):
	R,W,X=select.select([],[p.stdin],[],0);
	if len(W)!=0:
		p.stdin.write(str(random.randint(0,9)));
	R,W,X=select.select([p.stdout],[],[],timeout);
	if len(R)>=1:
		while True:
			if len(os.read(p.stdout.fileno(),1024))==0:
				p.kill();
				raise RuntimeError("checkAlive: got EOF");
			R,W,X=select.select([p.stdout],[],[],timeout);
			if len(R)==0: break;
		return True;
	return False;

while True:
	try:
		p=subprocess.Popen(sys.argv[1:], stdout=subprocess.PIPE,stdin=subprocess.PIPE);
		print "started ssh";
		if not checkAlive(p,CONNECT_WAIT):
			p.kill();
			print "initial ping timed out";
			continue;
		pingFailCnt=0;
		while True:
			if pingFailCnt>0: time.sleep(PING_FAIL_INTERVAL);
			else: time.sleep(PING_INTERVAL);
			if checkAlive(p,PING_TIMEOUT):
				pingFailCnt=0;
			else:
				pingFailCnt+=1;
				print "ping failed"
				if pingFailCnt>=PING_MAXFAIL:
					p.kill();
					print "ping max fail count exceeded";
					break;
	except:
		print "exception occurred";
		time.sleep(EXCEPTION_WAIT);



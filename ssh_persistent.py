#!/usr/bin/python
import os,sys,time,subprocess,random,select;

if len(sys.argv)<2:
	print "usage: "+sys.argv[0]+" ssh_command [cat|CUSTOM_COMMAND]";
	exit(1);


CONNECT_WAIT=18;
PING_INTERVAL=3;
PING_FAIL_INTERVAL=0;
PING_TIMEOUT=2;
PING_MAXFAIL=8;
EXCEPTION_WAIT=2;

def printMsg(s):
	sys.stdout.write(s);
	sys.stdout.flush();

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
			R,W,X=select.select([p.stdout],[],[],0);
			if len(R)==0: break;
		return True;
	return False;

while True:
	try:
		p=subprocess.Popen(sys.argv[1:], stdout=subprocess.PIPE,stdin=subprocess.PIPE);
		printMsg("connecting... ");
		sys.stdout.flush();
		if not checkAlive(p,CONNECT_WAIT):
			p.kill();
			print "initial ping timed out ("+str(CONNECT_WAIT)+"s)";
			continue;
		print "connected"
		pingFailCnt=0;
		while True:
			if pingFailCnt>0: time.sleep(PING_FAIL_INTERVAL);
			else: time.sleep(PING_INTERVAL);
			if checkAlive(p,PING_TIMEOUT):
				if pingFailCnt!=0:
					print "ping succeeded";
				pingFailCnt=0;
			else:
				pingFailCnt+=1;
				if pingFailCnt==1:
					printMsg("ping failed: ");
				printMsg(str(pingFailCnt)+"/"+str(PING_MAXFAIL)+"... ");
				if pingFailCnt>=PING_MAXFAIL:
					p.kill();
					print "max fail count exceeded";
					break;
	except:
		print "exception occurred";
		time.sleep(EXCEPTION_WAIT);



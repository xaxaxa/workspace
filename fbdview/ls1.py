#!/usr/bin/python
import cgi,cgitb,os
cgitb.enable();
print "Content-Type: text/html"     # HTML is following
print                               # blank line, end of headers

form = cgi.FieldStorage();
dir=form["p"].value;
if "t" in form:
	isdir=(form["t"].value=="d");
	for dirent in os.listdir(dir):
		if os.path.isdir(os.path.join(dir, dirent))==isdir:
			print dirent;
else:
	for dirent in os.listdir(dir):
		print dirent;



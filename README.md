workspace
=========

all the code i've ever written since grade 11; also includes some forward-ported code from grades 8 and 9

the workspace layout is currently quite messy; there are a lot of miscellaneous stuff

the most interesting things in there are:
  * cpoll - epoll based asynchornous socket I/O library
  * dedup - simple file deduplicator
  * tcpsdump - like tcpdump, but extracts tcp streams instead of individual packets
  * socketd - web server multiplexer/web application host
  * fbdump - a simple facebook profile dumping program written in C# with mono+gtk#
  * js - originally intended as a collection of javascript tools; however, it currently contains a newer, scriptable version of fbdump rewritten entirely from scratch; the new fbdump program is entirely javascript controlled
  * jackfft - a JACK audio based FFT filtering application (similar to jamin, but with adjustable FFT size and much lower cpu consumption)
  * nc.xaxaxa.C - my own version of netcat

all of the above projects should be compilable and usable

#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketAddress.h>
#include <string.h>
#include <sstream>
using namespace Poco::Net;
using namespace std;
int main()
{
	StreamSocket sock;
	SocketAddress addr("70.79.74.121:80");
	sock.connect(addr);
	
	const char* data="GET / HTTP/1.1\r\nHost: xa.us.to\r\n\r\n";
	sock.sendBytes(data,strlen(data));
	
	const int bs=4096;
	char* buf=new char[bs];
	int br;
	while((br=sock.receiveBytes(buf,bs))>0)
		write(1,buf,br);
		
	
}

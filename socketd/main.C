/*
 * main.cpp
 *
 *  Created on: 2011-05-20
 *      Author: xaxaxa
 */
/*
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * */
#include "include/socketd_internal.H"
#include <cpoll.H>
using namespace RGC;
using namespace socketd;
void listenthread();
int main(int argc,char** argv)
{
	listenthread();
}
void listenthread()
{
	//CP::Poll p;
	socketd::socketd sd;
	sd.listens.push_back({newObj<CP::IPEndPoint>(CP::IPAddress("0.0.0.0"),16969),1,32});
	sd.vhosts.push_back({{{0,"/asdf","",binding::match_httpPath}},"vhost1","/home/xaxaxa/workspace/test/socketd_test","", true});
	sd.vhosts.push_back({{{0,"/zxcv","",binding::match_httpPath}},"vhost2","/home/xaxaxa/workspace/test/socketd_test","", false});
	sd.vhosts.push_back({{{0,"/zzz","",binding::match_httpPath}},"vhost3","exec nc -l 0.0.0.0 12345 < /dev/urandom","", true});
	sd.vhosts.push_back({{{0,"/sss","",binding::match_httpPath}},"vhost4","lighttpd -D -f /home/xaxaxa/workspace/test/lighttpd.conf","", true});
	//sd.vhosts.push_back({{{1,"","",binding::match_listenID}},"vhost1","/home/xaxaxa/workspace/test/socketd_test",""});
	sd.run();
	/*config::rtconfigmanager *c=config::rtconfigmanager::getmainconfig();
	SocketManager m;
	int i;
	for(i=0;i<c->listens_c;i++)
	{
		Socket s(AF_INET,SOCK_STREAM|SOCK_CLOEXEC,0);
		s.Bind(c->listens[i].ep);
		s.Listen(c->listens[i].backlog);
		m.BeginAccept(s,SocketManager::Callback(cb1,NULL));
	}
	m.EventLoop();*/

}

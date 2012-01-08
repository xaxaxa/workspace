#ifndef __ASYNCSOCK_H
#define __ASYNCSOCK_H

#include <map>
#include <iostream>
//#include <sys/socket.h>
//#include <sys/types.h>
#include <sys/epoll.h>
#include "cplib.hpp"
#include <errno.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
#include <fcntl.h>
#include <boost/function.hpp>

using namespace std;
namespace xaxaxa
{
	namespace IO
	{
		class FileStreamManager: public Object
		{
		public:
			static FileStreamManager* __def_fsmanager;
			inline static FileStreamManager* GetDefault()
			{
				if (FileStreamManager::__def_fsmanager == NULL )
				FileStreamManager::__def_fsmanager = new FileStreamManager();
				return FileStreamManager::__def_fsmanager;
			}
			//typedef boost::function<void (void*, FileStreamManager*, File)> Callback;
			FUNCTION_DECLARE(Callback,void,FileStreamManager*,File);
			int epollfd;
			//arraylist<Socket> r,w,x;
			//arraylist<Socket> r2,w2,x2;
			//map<Socket,void*> r_info;
			//map<Socket,void*> w_info;
			//map<Socket,void*> x_info;
			/*enum taskType
			 {
			 read,write,accept
			 };*/

			struct taskInfo
			{
				//bool initialized;
				int bits;			//1: init; 2: read_fill; 4: write_fill;
				FILEDES s;
				Callback cb_recv, cb_send;
				__uint32_t events;
				__uint32_t new_events;
				Buffer r_buf, w_buf;
				int r_length, w_length;
				int r_errno, w_errno;
				inline taskInfo()
				{
					bits = 0;
				}
				//bool inLoop;//reserved; do not modify
			};
			map<FILEDES, taskInfo> info;
			FileStreamManager();
			~FileStreamManager();
			virtual void EventLoop();
			void BeginRecv(File s, Buffer buf, Callback cb, bool fill = false);
			int EndRecv(File s);
			void BeginSend(File s, Buffer buf, Callback cb, bool fill = false);
			int EndSend(File s);
			taskInfo __current_task;
			void Cancel(File s);
			void CancelRead(File s);
			void CancelWrite(File s);
		};
	/*class SocketStream:public Stream
	 {
	 public:
	 Socket socket;
	 SocketManager* m;
	 SocketStream()
	 {}
	 SocketStream(Socket socket,SocketManager *m=NULL)
	 {
	 this->socket=socket;
	 if(m==NULL)m=SocketManager::GetDefault();
	 this->m=m;
	 }
	 virtual int Read(Buffer buf)
	 {
	 return socket.Recv(buf,0);
	 }
	 virtual void Write(Buffer buf)
	 {
	 socket.Send(buf,0);
	 }
	 virtual void Flush()
	 {

	 }
	 virtual void Close()
	 {
	 socket.Close();
	 }
	 Callback r_cb;
	 FUNCTION_DECLWRAPPER(_r_cb,void,SocketManager* m,Socket sock)
	 {
	 FUNCTION_CALL(((SocketStream*)obj)->r_cb,(SocketStream*)obj);
	 }
	 virtual void BeginRead(Buffer buf,Callback cb)
	 {
	 r_cb=cb;
	 m->BeginRecv(socket,buf,SocketManager::Callback(_r_cb,this),true);
	 };
	 virtual void BeginWrite(Buffer buf,Callback cb)
	 {
	 Write(buf);
	 FUNCTION_CALL(cb,this);
	 }
	 virtual int EndRead()
	 {
	 return __tmp;
	 }
	 virtual void EndWrite()
	 {

	 };
	 };*/
	}
}

#endif

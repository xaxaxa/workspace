//============================================================================
// Name        : socketd.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <sys/sendfile.h>
using namespace std;

int main1() {
	int pid=fork();
	if(pid>0)
	{//parent
		int server_socket = socket(PF_UNIX, SOCK_STREAM, 0);
		struct sockaddr_un server_address = { AF_UNIX, "\0socketd" };
		bind(server_socket, (struct sockaddr *) &server_address, sizeof server_address);
		listen(server_socket, 1);
		struct sockaddr_un client_address;
		socklen_t client_address_length = sizeof client_address;
		int client_connection = accept(server_socket,
		                               (struct sockaddr *) &client_address,
		                               &client_address_length);
		close(server_socket);

		int file1=open("/file1",0,O_RDONLY);
		int file_descriptors[1] = { file1 };
		char buffer[CMSG_SPACE(sizeof file_descriptors)];
		msghdr message={0};
		message.msg_control = buffer;
		message.msg_controllen = sizeof buffer;

		cmsghdr *cmessage = CMSG_FIRSTHDR(&message);
		cmessage->cmsg_level = SOL_SOCKET;
		cmessage->cmsg_type = SCM_RIGHTS;
		cmessage->cmsg_len = CMSG_LEN(sizeof file_descriptors);
		message.msg_controllen = cmessage->cmsg_len;

		memcpy(CMSG_DATA(cmessage), file_descriptors, sizeof file_descriptors);

		char ping = 23;
		iovec ping_vec={0};
		ping_vec.iov_base = &ping;
		ping_vec.iov_len = sizeof ping;
		message.msg_iov = &ping_vec;
		message.msg_iovlen = 1;

		sendmsg(client_connection, &message, 0);

		close(client_connection);
		close(file1);

	}else if(pid==0)
	{//child
		//sleep(1);
		struct sockaddr_un server_address = { AF_UNIX, "\0socketd" };
		int client_socket = socket(PF_UNIX, SOCK_STREAM, 0);
		connect(client_socket, (struct sockaddr *) &server_address, sizeof server_address);

		int file_descriptors[1];
		char buffer[CMSG_SPACE(sizeof file_descriptors)];

		char ping;
		iovec ping_vec;
		ping_vec.iov_base = &ping;
		ping_vec.iov_len = sizeof ping;

		msghdr message;
		message.msg_control = buffer;
		message.msg_controllen = sizeof buffer;
		message.msg_iov = &ping_vec;
		message.msg_iovlen = 1;

		recvmsg(client_socket, &message, 0);
		close(client_socket);

		struct cmsghdr *cmessage = CMSG_FIRSTHDR(&message);
		memcpy(file_descriptors, CMSG_DATA(cmessage), sizeof file_descriptors);

		//sendfile(STDOUT_FILENO,file_descriptors[0],0,4);
		char c[4];
		read(file_descriptors[0],&c,4);
		write(STDOUT_FILENO,&c,4);
	}
	return 0;
}

/*
 *by:yuntang
 *filename:socket.cpp
 *
 */
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <fcntl.h>
#include <memory.h>
#include <string.h>
#include <unistd.h>
#include "socket.h"
namespace myspace
{
Socket::Socket()
{
}

void Socket::BuildListen()
{
	struct sockaddr_in servaddr;
	sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd_ == -1) {
		printf("create socket error in socket.cpp->Socket::Socket\n");
		exit(1);
	}
	memset(&servaddr, 0, sizeof(sockaddr_in));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERVER_PORT);
	printf("server port: %d\n", SERVER_PORT);
	if (bind(sockfd_, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
		printf( "bind error in socket.cpp->Socket::Socket\n");
		exit(1);
	}
	if (listen(sockfd_, 10) == -1) {
		printf( "listen error in socket.cpp->Socket::Socket\n");
		exit(1);
	}
	SetNonBlocking();
}
void Socket::SetNonBlocking()
{
	int opts;
	opts = fcntl(sockfd_, F_GETFL);
	if (opts < 0) {
		printf("error Socket->SetNonBlocking()1\n");
		exit(1);
	}
	opts = opts|O_NONBLOCK;
	if (fcntl(sockfd_, F_SETFL, opts)) {
		printf("error Socket->SetNonBlocking()2\n");
		exit(1);
	}
}
void Socket::SetFd(int sockfd)
{
	sockfd_ = sockfd;
}
const int Socket::sockfd()
{
	return sockfd_;
}
}


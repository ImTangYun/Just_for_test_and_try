/*
 *by:yuntang
 *filename:socket.h
 *
 */
#ifndef EPOLL_SOCKET_H_
#define EPOLL_SOCKET_H_
#include <sys/socket.h>
#define SERVER_PORT 4444
namespace myspace
{
class Socket
{
public:
	Socket();
	~Socket(){}
	void BuildListen(int port);
	void SetFd(int sockfd);
	const int sockfd();
protected:
	void SetNonBlocking();
private:
	int sockfd_;
};
}

#endif // EPOLL_SOCKET_H_

/*
 *by:yuntang
 *filename:socket.h
 *
 */
#ifndef EPOLL_SOCKET_H_
#define EPOLL_SOCKET_H_
#include <sys/socket.h>
#define SERVER_PORT 6666
namespace myspace
{
class Socket
{
public:
	Socket();
	~Socket(){}
	void BuildListen();
	void SetFd(int sockfd);
	const int sockfd();
protected:
	void SetNonBlocking();
private:
	int sockfd_;
};
}

#endif // EPOLL_SOCKET_H_

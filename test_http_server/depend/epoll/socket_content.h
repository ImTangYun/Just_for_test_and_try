/*
 *by：yuntang
 *filename:socket_content.h
 *
 */
#ifndef EPOLL_SOCKET_CONTENT
#define EPOLL_SOCKET_CONTENT
#include "package.h"

namespace myspace
{

class Socket;
class SocketContent
{
public:
	SocketContent(Socket* socket);
	virtual ~SocketContent();
	virtual void OnConnect();
	virtual Package* OnReceived(Package &pack);
	virtual void OnReceived(void* buff, int length);
	virtual void Onclosed();
	virtual void OnTimeOut();
	int GetFd();
	void SetSocket(Socket* socket);
	Package* package_;
private:
	Socket* socket_;
};
} // namespace myspace

#endif // EPOLL_SOCKET_CONTENT

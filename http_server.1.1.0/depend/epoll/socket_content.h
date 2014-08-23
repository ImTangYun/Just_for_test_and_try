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
	virtual void OnReceived(Package &pack, Package &send_pack);
	virtual void OnReceived(void* buff, int length);
	virtual void Onclosed();
	virtual void OnTimeOut();
	int GetFd();
	void SetSocket(Socket* socket);
private:
	Socket* socket_;
};
} // namespace myspace

#endif // EPOLL_SOCKET_CONTENT

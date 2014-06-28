/*
 *by：yuntang
 *filename:socket_content.h
 *
 */
#ifndef EPOLL_SOCKET_CONTENT
#define EPOLL_SOCKET_CONTENT
namespace myspace
{

class Socket;
class SocketContent
{
public:
	SocketContent();
	virtual ~SocketContent();
	int GetFd();
private:
	Socket* socket_;
};
} // namespace myspace

#endif // EPOLL_SOCKET_CONTENT

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
	SocketContent(Socket* socket);
	virtual ~SocketContent();
	virtual void OnConnect();
	virtual void OnReceived(int fd);
	virtual void Onclosed();
	virtual void OnTimeOut();
	int GetFd();
	void SetSocket(Socket* socket);
private:
	Socket* socket_;
};
} // namespace myspace

#endif // EPOLL_SOCKET_CONTENT

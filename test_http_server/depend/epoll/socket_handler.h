/*
 *by:yuntang
 *filename: socket_handler.h
 *
 */
#ifndef EPOLL_SOCKET_HANDLER_H_
#define EPOLL_SOCKET_HANDLER_H_
namespace myspace
{
class SocketHandler
{
public:
	SocketHandler();
	virtual ~SocketHandler();
	// virtual void OnReceived(int fd);
	virtual void OnClosed();
	virtual void OnSent();
};
} // namespace myspace

#endif // EPOLL_SOCKET_HANDLER_H_

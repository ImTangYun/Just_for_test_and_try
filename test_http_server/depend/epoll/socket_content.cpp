/*
 *by：yuntang
 *filename:socket_content.cpp
 *
 */
#include "socket_content.h"
#include "socket.h"
namespace myspace
{
SocketContent::SocketContent(Socket* socket)
{
	socket_ = socket;
}
SocketContent::~SocketContent()
{
	delete socket_;
}
int SocketContent::GetFd()
{
	return socket_->sockfd();
}
} // namespace myspace


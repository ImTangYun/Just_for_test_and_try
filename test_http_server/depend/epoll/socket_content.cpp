/*
 *by：yuntang
 *filename:socket_content.cpp
 *
 */
#include "socket_content.h"
#include "socket.h"
namespace myspace
{
SocketContent::SocketContent()
{
	socket_ = new Socket;
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


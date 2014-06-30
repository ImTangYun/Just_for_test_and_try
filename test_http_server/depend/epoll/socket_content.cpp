/*
 *by：yuntang
 *filename:socket_content.cpp
 *
 */
#include <stdio.h>
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
	if (socket_ == NULL) printf("socket_ is NULL at socket_content.cpp->SocketContent->GetFd()\n");
	if (socket_ != NULL)
		return socket_->sockfd();
	return -1;
}
void SocketContent::SetSocket(Socket* socket)
{
	socket_ = socket;
}
void SocketContent::OnConnect()
{

}
void SocketContent::OnReceived(void* buff, int length)
{
	
}
void SocketContent::OnReceived(Package &pack)
{

}
void SocketContent::Onclosed()
{

}
void SocketContent::OnTimeOut()
{

}
} // namespace myspace


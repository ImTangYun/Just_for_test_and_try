/*
 *by:yuntang
 *filename:listen_handler.h
 *
 */
#include "socket_content.h"
#include "socket.h"
#include "listen_handler.h"
namespace myspace
{
ListenHandler::ListenHandler():SocketContent((Socket*)0)
{
	Socket* socket = new Socket;
	socket->BuildListen();
	SocketContent::SetSocket(socket);
}
ListenHandler::~ListenHandler()
{
}
}

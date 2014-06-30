/*
 *by:yuntang
 *filename:listen_handler.h
 *
 */
#ifndef LISTEN_HANDLER_H_
#define LISTEN_HANDLER_H_
#include <stdio.h>
#include "package.h"
#include "socket_content.h"
namespace myspace
{
class SocketContent;
class ListenHandler: public SocketContent
{
public:
	ListenHandler();
	~ListenHandler();
	virtual void OnReceived(Package &pack)
	{
		printf("length------%d\n", pack.length());
	}
	virtual void OnReceived(void* buff, int length)
	{
		printf("length------%d\n", length);
	}
};
}
#endif // LISTEN_HANDLER_H_

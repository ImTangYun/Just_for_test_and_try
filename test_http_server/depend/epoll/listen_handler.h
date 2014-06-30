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
	virtual void OnReceived(Package &pack, Package &send_pack)
	{
		printf("length------%d\n", pack.length());
		char* p = static_cast<char*>(const_cast<void*>(pack.data()));
		p[pack.length()] = '\0';
		printf("%s", p);
		delete [] p;
		char *buff = new char[100];
		buff[0] = 'h';
		buff[1] = 'e';
		buff[2] = 'l';
		buff[3] = '\n';
	}
	virtual void OnReceived(void* buff, int length)
	{
		printf("length------%d\n", length);
	}
};
}
#endif // LISTEN_HANDLER_H_

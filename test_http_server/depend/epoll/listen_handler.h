/*
 *by:yuntang
 *filename:listen_handler.h
 *
 */
#ifndef LISTEN_HANDLER_H_
#define LISTEN_HANDLER_H_
#include "socket_content.h"
namespace myspace
{
class SocketContent;
class ListenHandler: public SocketContent
{
public:
	ListenHandler();
	~ListenHandler();
};
}
#endif // LISTEN_HANDLER_H_

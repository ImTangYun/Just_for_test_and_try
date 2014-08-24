/*
 *by:yuntang
 *filename:yuntang
 *
 */
#ifndef SRC_HTTP_SOCKET_HANDLER_H_
#define SRC_HTTP_SOCKET_HANDLER_H_
#include <stdio.h>
#include <sys/socket.h>
#include "listen_handler.h"
#include "package.h"
#include "response.h"
namespace http
{

using myspace::ListenHandler;
using myspace::Package;

class HttpSocketHandler: public ListenHandler
{
public:
    HttpSocketHandler(int port);
    ~HttpSocketHandler();
    virtual void OnConnect()
    {
        printf("--------connect-------\n");
    }
    virtual void OnReceived(Package &pack, Package &send_packge)
    {
        printf("--------OnReceived-------\n");
        char* buf = (char*)(const_cast<void*>(pack.data()));
        buf[pack.length()] = '\0';
        resp_.DealWithRequest(buf, pack.length(), pack.GetFd());
    }
    virtual void OnReceived(void* buff, int length)
    {
        char* p = (char*)buff;
        p[length] = '\0';
        printf("length: %d----\n%s\n----\n", length, p);
    }
private:
    Response resp_;
};
} // namespace http
#endif // SRC_HTTP_SOCKET_HANDLER_H_

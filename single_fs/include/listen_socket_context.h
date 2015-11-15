//
//
//
//
#ifndef LISTEN_SOCKET_CONTEXT
#define LISTEN_SOCKET_CONTEXT
#include "socket_context.h"
class CommunicateLoop;
class NetHandler;
class ListenSocketContext: public SocketContext
{
    public:
        ListenSocketContext(int server_port, CommunicateLoop* communicate_loop,
                NetHandler* net_handler):
            server_port_(server_port), SocketContext(communicate_loop),
            net_handler_(net_handler){}
        int Init();
        virtual int HandleOutput(){return 0;}
        virtual int HandleInput();
        virtual void OnReceived() {}
    private:
        int server_port_;
        NetHandler* net_handler_;
};
#endif

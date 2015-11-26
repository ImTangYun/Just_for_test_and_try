//
//
//
//
#ifndef SERVER_HANDLER
#define SERVER_HANDLER
#include "packet.h"
#include "net_handler.h"
#include "listen_handler.h"
class ServerHandler: public ListenHandler
{
    public:
        int OnReceived(Packet* packet, EndPoint* end_point);
        int OnReceived(Packet* packet);
        int OnAccepted(int fd);
        int OnSent(Packet* packet);
        int OnTimeOut();
        int EchoFromServer(Packet* packet);
};
#endif

//
//
//
//
#ifndef LISTEN_HANDLER
#define LISTEN_HANDLER
#include "packet.h"
#include "net_handler.h"
class ListenHandler: public NetHandler
{
    public:
        int OnReceived(Packet* packet, EndPoint* end_point);
        int OnReceived(Packet* packet);
        int OnAccepted(int fd);
        int OnSent(Packet* packet);
        int OnDisconnected(int fd);
        int OnConnected(int fd);
        int OnTimeOut();
};
#endif

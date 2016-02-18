//
//
//
//
#ifndef NET_HANDLER
#define NET_HANDLER
class Packet;
class EndPoint;
class NetHandler
{
    public:
        virtual int OnReceived(Packet* packet) = 0;
        virtual int OnReceived(Packet* packet, EndPoint* end_point) = 0;
        virtual int OnAccepted(int fd) = 0;
        virtual int OnSent(Packet* packet) = 0;
        virtual int OnDisconnected(int fd) = 0;
        virtual int OnConnected(int fd) = 0;
        virtual int OnTimeOut() = 0;
        virtual int OnClose(EndPoint* end_point) = 0;
};
#endif

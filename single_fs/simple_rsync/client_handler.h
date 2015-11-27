//
//
//
//
#ifndef CLIENT_HANDLER
#define CLIENT_HANDLER
#include "packet.h"
#include "end_point.h"
#include "stream_handler.h"
#include "stream_socket_context.h"
class ClientHandler: public StreamHandler
{
    public:
        virtual int OnReceived(Packet* packet)
        {
            char* data = packet->data();
            uint32_t length = packet->data_length();
            return 0;
        }
        virtual int OnReceived(Packet* packet, EndPoint* end_point)
        {
            char* data = packet->data();
            uint32_t length = packet->data_length();
            data[length] = '\n';
            printf("received: %s\n", data);
            return 0;
        }
        virtual int OnAccepted(int fd){ return 0;}
        virtual int OnSent(Packet* packet)
        {
            char* data = packet->data();
            uint32_t length = packet->data_length();
            data[length] = '\n';
            printf("received: %s\n", data);
            return 0;
        }
        virtual int OnTimeOut(){return 0;}
};
#endif

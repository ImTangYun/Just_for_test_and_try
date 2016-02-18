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
#include "package.h"
#include "log.h"
class ClientHandler: public StreamHandler
{
    public:
        virtual int OnReceived(Packet* packet)
        {
            WLOG(DEBUG, "received");
            // char* data = packet->data();
            // uint32_t length = packet->data_length();
            Package* package = new Package();
            package->set_data(packet->data(), packet->data_length());
            int32_t sender_length = (int32_t)ntohl(package->GetInt32_t());
            string* sender = package->GetString(sender_length);
            int32_t receiver_length = (int32_t)ntohl(package->GetInt32_t());
            string* receiver = package->GetString(receiver_length);
            int32_t message_length = (int32_t)ntohl(package->GetInt32_t());
            string* message = package->GetString(message_length);
            if (*receiver != "world") {
                WLOG(INFO, "[private!][%s]:%s", sender->c_str(), message->c_str());
            } else {
                WLOG(NOTICE, "[world!][%s]:%s", sender->c_str(), message->c_str());
            }
            return 0;
        }
        virtual int OnReceived(Packet* packet, EndPoint* end_point)
        {
            WLOG(DEBUG, "received !");
            Package* package = new Package();
            package->set_data(packet->data(), packet->data_length());
            int32_t sender_length = (int32_t)ntohl(package->GetInt32_t());
            string* sender = package->GetString(sender_length);
            int32_t receiver_length = (int32_t)ntohl(package->GetInt32_t());
            string* receiver = package->GetString(receiver_length);
            int32_t message_length = (int32_t)ntohl(package->GetInt32_t());
            string* message = package->GetString(message_length);
            WLOG(INFO, "[private][%s]:%s", sender->c_str(), message->c_str());
            return 0;
        }
        virtual int OnAccepted(int fd){ return 0;}
        virtual int OnSent(Packet* packet)
        {
            /*char* data = packet->data();
            uint32_t length = packet->data_length();
            data[length] = '\n';
            printf("received: %s\n", data);*/
            return 0;
        }
        virtual int OnTimeOut(){return 0;}
};
#endif

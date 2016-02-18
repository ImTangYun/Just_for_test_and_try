//
//
//
//
#ifndef CLIENT_H_
#define CLIENT_H_

#include <stdint.h>
#include <string>
#include <list>

using std::string;
using std::list;
class EndPoint;
class NetHandler;
class NetMachine;
class StreamSocketContext;
class Client
{
    public:
        Client(string ip_port):ip_port_(ip_port){}
        int32_t Init();
        void SendMessage(const string& receiver, const string& message);
        void Login(const string& user_name);
        ~Client();

    private:
        string ip_port_;
        EndPoint* end_point_;
        NetMachine* net_machine_;
        NetHandler* net_handler_;
        StreamSocketContext* socket_context_;
        string user_name_;
};

#endif

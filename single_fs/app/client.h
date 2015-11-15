//
//
//
//
#ifndef CLIENT_H_
#define CLIENT_H_
#include <string>

using std::string;

class EndPoint;
class NetHandler;
class NetMachine;
class StreamSocketContext;
class Client
{
    public:
        Client(string ip_port):ip_port_(ip_port) {}
        int Init();
        int Put(char* path);
        int Get(char* file_id, char** buf, int &length);
        int Delete(int file_id);
    private:
        string ip_port_;
        EndPoint* end_point_;
        NetMachine* net_machine_;
        NetHandler* net_handler_;
        StreamSocketContext* socket_context_;
};
#endif

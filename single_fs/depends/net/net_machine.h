// 
// net machine
// 15/10/25
//

#ifndef NET_MACHINE
#define NET_MACHINE
#include <string>
#include "communicate_loop.h"
using std::string;
class Packet;
class NetHandler;
class EndPoint;
class NetMachine
{
    public:
        int Init();
        NetMachine():communicate_loop_(new CommunicateLoop()) {}
        ~NetMachine(){delete communicate_loop_;}
        int AsyncListen(int port, NetHandler* net_handler);
        int AsyncSendPacket(const string &ip_port, Packet* packet, NetHandler* net_handler);
        int AsyncSendPacket(EndPoint* end_point, Packet* packet, NetHandler* net_handler);
        int SyncSendPacket(EndPoint* end_point, Packet* packet,
                NetHandler* net_handler, void** ret_buf, int time_out);
        CommunicateLoop* communicate_loop() {return communicate_loop_;}
    private:
        CommunicateLoop* communicate_loop_;
};

#endif

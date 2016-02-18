//
//
//
//
#ifndef _SERVER_HANDLER_H_
#define _SERVER_HANDLER_H_
#include "packet.h"
#include "net_handler.h"
#include "listen_handler.h"

class TaskNode;
class UserConnection;
namespace myspace
{
template<typename T>
class TaskQueue;
};
class ServerHandler: public ListenHandler
{
    public:
        int OnReceived(Packet* packet, EndPoint* end_point);
        int OnReceived(Packet* packet);
        int OnAccepted(int fd);
        int OnSent(Packet* packet);
        int OnTimeOut();
        int OnClose(EndPoint* end_point);
        int EchoFromServer(Packet* packet);
        int Init(myspace::TaskQueue<TaskNode*>* task_queue,
                UserConnection* user_connection);
    private:
        myspace::TaskQueue<TaskNode*>* task_queue_;
        UserConnection* user_connection_;
};
#endif

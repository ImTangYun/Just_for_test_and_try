//
//
//
//
#ifndef SERVER_H_
#define SERVER_H_
#include <stdint.h>

class ServerHandler;
class NetMachine;
class ServerProcessor;
class TaskNode;
namespace myspace
{
template<typename T>
class TaskQueue;
};
class Server
{
    public:
        Server(int32_t port);
        void Init(int32_t thread_num);
        void Start();
        void Stop();
        ~Server();
    private:
        NetMachine* net_machine_;
        ServerHandler* net_handler_;
        ServerProcessor* server_processor_;
        myspace::TaskQueue<TaskNode*>* task_queue_;
        int32_t port_;
};
#endif

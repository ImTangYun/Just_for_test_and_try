//
//
//
//
#include "server.h"
#include "singleton.h"
#include "server_handler.h"
#include "net_machine.h"
#include "server_processor.h"
#include "task_queue.h"
#include "task_node.h"
using myspace::TaskQueue;

Server::Server(int32_t port):port_(port)
{
}
void Server::Init(int32_t thread_num)
{
    task_queue_ = new TaskQueue<TaskNode*>();
    net_machine_ = Singleton<NetMachine>::Instance();
    net_handler_ = new ServerHandler();
    net_handler_->Init(task_queue_);
    server_processor_ = new ServerProcessor();
    server_processor_->Init(thread_num, task_queue_);
    server_processor_->Start();
}
void Server::Start()
{
    net_machine_->AsyncListen(port_, net_handler_);
}
void Server::Stop()
{
}
Server::~Server()
{
}

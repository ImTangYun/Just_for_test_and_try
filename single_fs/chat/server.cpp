//
//
//
#include "server.h"
#include "singleton.h"
#include "server_handler.h"
#include "net_machine.h"
#include "chat_process.h"
#include "task_queue.h"
#include "task_node.h"
#include "user_connection.h"
using myspace::TaskQueue;

Server::Server(int32_t port):port_(port)
{
}
void Server::Init(int32_t thread_num)
{
    task_queue_ = new TaskQueue<TaskNode*>();
    net_machine_ = Singleton<NetMachine>::Instance();
    net_handler_ = new ServerHandler();
    UserConnection* user_connection = new UserConnection();
    net_handler_->Init(task_queue_, user_connection);
    chat_process_ = new ChatProcess();
    chat_process_->Init(task_queue_, user_connection);
    chat_process_->Start();
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

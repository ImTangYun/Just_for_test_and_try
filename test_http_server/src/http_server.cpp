#include <stdlib.h>
#include <unistd.h>
#include "http_server.h"

namespace http
{
HttpServer::HttpServer()
{
    http_config_ = new HttpConfig;
    listen_handler_ = new HttpSocketHandler(http_config_->Port());
    communicate_ = new myspace::Communicate(listen_handler_);
    taskqueue_ = new TaskQueue<TaskNode>(100);
    communicate_->Init(taskqueue_);
    printf("after communicate_->Init(taskqueue_)\n");
    http_work_flow_ = new HttpWorkFlow(taskqueue_);
    bool ret = http_work_flow_->Init(http_config_->Thread_num());
    if (!ret) {
        delete listen_handler_;
        listen_handler_ = NULL;
        delete communicate_;
        communicate_ = NULL;
        delete taskqueue_;
        taskqueue_ = NULL;
        delete http_work_flow_;
        http_work_flow_ = NULL;
        printf("http_server.cpp->HttpServer::HttpServer() failed!");
        exit(1);
    }
}
HttpServer::~HttpServer()
{
    if (listen_handler_)
        delete listen_handler_;
    if (communicate_)
        delete communicate_;
    if (taskqueue_)
        delete taskqueue_;
    if (http_work_flow_)
        delete http_work_flow_;
}
void HttpServer::StartServer()
{
    communicate_->Start();
    http_work_flow_->Start();
}
}

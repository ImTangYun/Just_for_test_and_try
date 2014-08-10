#ifndef _HTTP_SERVER_H_
#define _HTTP_SERVER_H_
#include "http_work_flow.h"
#include "response.h"
#include "taskqueue.h"
#include "http_conf.h"
#include "communicate.h"
#include "http_socket_handler.h"
namespace http
{
using http::HttpSocketHandler;
using myspace::Communicate;
using http::HttpConfig;
class HttpServer
{
public:
    HttpServer();
    ~HttpServer();
    void StartServer();
private:
    TaskQueue<TaskNode>* taskqueue_;
    HttpSocketHandler* listen_handler_;
    Communicate* communicate_;
    HttpWorkFlow* http_work_flow_;
    HttpConfig* http_config_;
};
}

#endif // _HTTP_SERVER_H_

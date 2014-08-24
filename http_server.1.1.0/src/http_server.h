#ifndef SRC_HTTP_SERVER_H_
#define SRC_HTTP_SERVER_H_
#include "http_work_flow.h"
#include "response.h"
#include "taskqueue.h"
#include "http_conf.h"
#include "communicate.h"
namespace http
{

class HttpSocketHandler;
class HttpConfig;

using myspace::Communicate;

class HttpServer
{
public:
    HttpServer();
    void Init();
    ~HttpServer();
    void StartServer();
private:
    TaskQueue<TaskNode>* taskqueue_;
    HttpSocketHandler* listen_handler_;
    Communicate* communicate_;
    HttpWorkFlow* http_work_flow_;
    HttpConfig* http_config_;
};
} // namespace http

#endif // SRC_HTTP_SERVER_H_

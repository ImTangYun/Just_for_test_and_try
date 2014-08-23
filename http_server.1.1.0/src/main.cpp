#include <stdio.h>
#include <unistd.h>
#include "communicate.h"
#include "http_socket_handler.h"
#include "http_conf.h"
#include "Work_flow.h"
#include "http_server.h"
using http::HttpSocketHandler;
using myspace::Communicate;
using http::HttpConfig;
using cthread::WorkFlow;
using http::HttpServer;
int main()
{
    HttpServer http_server;
    http_server.StartServer();
    while (true) sleep(10);
    return 0;
}


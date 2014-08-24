#include <unistd.h>
#include "http_server.h"
using http::HttpServer;
int main()
{
    HttpServer http_server;
    http_server.Init();
    http_server.StartServer();
    while (true) sleep(10);
    return 0;
}

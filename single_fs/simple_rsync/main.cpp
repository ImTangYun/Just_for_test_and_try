//
//
//
//
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include "server.h"
#include "client.h"
#include "log.h"
#include "time_utils.h"
#define PORT 443

using std::string;
void run_server()
{
    Server server(PORT);
    server.Init(10);
    server.Start();
    while (true) {
        sleep(10);
    }
}

void run_client(int argc, char** argv)
{
    for (int i = 0; i < argc; ++ i) {
        WLOG(DEBUG, "param %d is %s", i, argv[i]);
    }
    char buf[10];
    snprintf(buf, 10, "%d", PORT);
    string ip_port = "10.100.125.138:";
    ip_port += buf;
    Client client(ip_port);
    client.Init();
    TimeCounter time_counter;
    client.GetFile((char*)argv[0], (char*)argv[1], (char*)argv[2]);
    time_counter.AddNow();
    WLOG(NOTICE, "get file %s cost %fms", argv[0], time_counter.GetTimeCosts(1));
}

int main(int argc, char** argv)
{
    Log::logger_.set_level(ERROR);
    if (argc < 2) {
        WLOG(ERROR, "Usage: at least a param!");
        return -1;
    }
    string task = argv[1];
    if (task == "server") {
        WLOG(DEBUG, "Startting server...");
        run_server();
    } else {
        run_client(argc - 2, argv + 2);
    }
    return 0;
}

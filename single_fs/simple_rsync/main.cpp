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

using std::string;
void run_server()
{
    Server server(1234);
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
    Client client("10.100.125.138:1234");
    client.Init();
    client.GetFile((char*)argv[0], (char*)argv[1], (char*)argv[2]);
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        WLOG(ERROR, "Usage: at least a param!");
        return -1;
    }
    string task = argv[1];
    if (task == "server") {
        run_server();
    } else {
        run_client(argc - 2, argv + 2);
    }
    return 0;
}

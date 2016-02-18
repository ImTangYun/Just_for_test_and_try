#include <stdio.h>
#include <unistd.h>
#include "server.h"

int main(int argc, char** argv)
{
    printf("hello world!\n");
    Server* server = new Server(1234);
    server->Init(1);
    server->Start();
    while (true) {
        sleep(10);
    }
    return 0;
}

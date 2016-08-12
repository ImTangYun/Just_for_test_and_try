#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include "net_machine.h"
#include "packet.h"
#include "communicate_loop.h"
#include "listen_handler.h"
#include "singleton.h"
#include "server_handler.h"

int main()
{
    NetMachine* net_machine = Singleton<NetMachine>::Instance();
    ListenHandler* net_handler = new ServerHandler();
    net_machine->AsyncListen(1235, net_handler);
    while (true) {
        sleep(1);
    }
    return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include "net_machine.h"
#include "packet.h"
#include "communicate_loop.h"
#include "net_handler.h"
#include "server_handler.h"
#include "singleton.h"

int main()
{
    NetMachine* net_machine = Singleton<NetMachine>::Instance();
    NetHandler* net_handler = new ServerHandler();
    net_machine->AsyncListen(443, net_handler);
    while (true) {
        sleep(1);
    }
    return 0;
}

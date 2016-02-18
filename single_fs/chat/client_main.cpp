//
//
//
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include "client.h"
#include "log.h"
using namespace std;

int main(int argc, char** args)
{
    Log::logger_.set_level(INFO);
    WLOG(INFO, "Usage: %s user_name recever", args[0]);
    if (argc < 3) {
        return -1;
    }
    Client* client = new Client("10.100.125.138:1234");
    client->Init();
    client->Login(args[1]);
    sleep(3);
    string msg;
    while (cin >> msg) {
        client->SendMessage(args[2], msg);
        sleep(1);
    }
    return 0;
}

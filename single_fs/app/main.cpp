#include <stdio.h>
#include <stdlib.h>
#include "client.h"

int main(int argc, char* argv[])
{
    Client* client = new Client("10.100.125.138:1235");
    client->Init();
    int opt = atoi(argv[1]);
    if (opt == 2) { // put file
        client->Put(argv[2]);
    } else if (opt == 1) { // get file
        int x;
        client->Get(argv[2], (char**)0, x);
    }
    return 0;
}

//
//
//
//
#include <stdlib.h>
#include <string>
#include "socket_context.h"
int SocketContext::ParseIpPort()
{
    ip_ = new char[16];
    int i = 0;
    for (; i < ip_port_.length(); ++i) {
        if (ip_port_[i] == ':') break;
        ip_[i] = ip_port_[i];
    }
    ip_[i] = '\0';
    char buff[10];
    int count = 0;
    ++i;
    for (; i < ip_port_.length(); ++i) {
        buff[count] = ip_port_[i];
        ++count;
    }
    buff[count] = '\0';
    port_ = atoi(buff);
    return 0;
}

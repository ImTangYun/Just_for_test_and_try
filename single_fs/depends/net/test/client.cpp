#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include "net_machine.h"
#include "packet.h"
#include "communicate_loop.h"
#include "net_handler.h"
#include "stream_handler.h"
#include "stream_socket_context.h"
#include "end_point.h"
#include "singleton.h"
#include "client_handler.h"

int rsyncclient()
{
    NetMachine* net_machine = Singleton<NetMachine>::Instance();
    Packet* packet = new Packet();
    char* data = new char[100];
    uint32_t length = snprintf(data, 100, "hello world");
    packet->set_packet(data, length);

    NetHandler* net_handler = new StreamHandler();

    StreamSocketContext* socket_context = new
        StreamSocketContext("127.0.0.1:1235", net_handler, net_machine);
    socket_context->Init();

    EndPoint* end_point = new EndPoint(socket_context);
    packet->set_end_point(end_point);
    socket_context->set_end_point(end_point);

    net_machine->AsyncSendPacket(end_point, packet, net_handler);
    sleep(1);
    return 0;
}

int syncclient()
{
    NetMachine* net_machine = Singleton<NetMachine>::Instance();
    Packet* packet = new Packet();
    char* data = new char[100];
    uint32_t length = snprintf(data, 100, "hello world");
    packet->set_packet(data, length);

    NetHandler* net_handler = new ClientHandler();

    StreamSocketContext* socket_context = new
        StreamSocketContext("127.0.0.1:1235", net_handler, net_machine);
    socket_context->Init();

    EndPoint* end_point = new EndPoint(socket_context);
    packet->set_end_point(end_point);
    socket_context->set_end_point(end_point);

    void* ret_buf;
    net_machine->SyncSendPacket(end_point, packet, net_handler, &ret_buf, 0);

    Packet* packet1 = (Packet*)ret_buf;
    char* data1 = packet1->data();
    uint32_t length1 = packet1->data_length();
    printf("sync received: %s, length1 %d\n", data1, length1);
    StreamSocketContext* socket_context1 = packet1->end_point()->socket_context();
    delete packet1;
    return 0;
}

int main()
{
    // rsyncclient();
    syncclient();
    delete Singleton<NetMachine>::Instance();
    return 0;
}

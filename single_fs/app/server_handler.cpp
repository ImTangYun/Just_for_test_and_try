//
//
//
//
#include <stdint.h>
#include <arpa/inet.h>
#include "packet.h"
#include "server_handler.h"
#include "end_point.h"
#include "stream_socket_context.h"
#include "file_utils.h"
#include "inited_ifs.h"
#include "singleton.h"
int ServerHandler::OnReceived(Packet* packet)
{
    return EchoFromIfs(packet);
}
int ServerHandler::EchoFromIfs(Packet* packet)
{
    InitedIfs* inited_ifs = Singleton<InitedIfs>::Instance();
    char* data = packet->data();
    data[packet->data_length()] = '\0';
    int req = ntohl(((int*)data)[0]);
    printf("req = %d\n", req);
    data += sizeof(int);
    EndPoint* end_point = packet->end_point();
    if (req == 1) {
        int64_t file_id = (int64_t)atoi(data);
        if (file_id <= 0) {
            file_id = 10;
        }
        printf("file_id=%lu\n", file_id);
        printf("-------------------from server-----------------------");
        // printf("received: %s, length:%d\n", data, length);
        printf("-------------------from server-----------------------");
        delete [] packet->data();

        int64_t file_length = inited_ifs->ifs()->get_length(file_id);
        char *buf = new char[file_length + 2];
        buf[file_length] = '\0';
        int64_t ret = inited_ifs->ifs()->read(file_id, buf, file_length);
        printf("-------------------from server-----------------------");
        printf("received: %s, length:%lu\n", buf, file_length);
        printf("-------------------from server-----------------------");

        packet->set_packet(buf, (uint32_t) file_length);
        StreamSocketContext* socket_context = end_point->socket_context();
        socket_context->AsyncSendPacket(packet);
    } else if (req == 2) {
        int file_id = inited_ifs->ifs()->write(data, packet->data_length() - sizeof(int));
        inited_ifs->ifs()->save_metadata();
        char* buf = new char[sizeof(int) + 1];
        ((int*)buf)[0] = htonl(file_id); 
        printf("data: %s\n", data);
        packet->set_packet(buf, (uint32_t)sizeof(int));
        StreamSocketContext* socket_context = end_point->socket_context();
        socket_context->AsyncSendPacket(packet);
    }
    return 0;
}

int ServerHandler::EchoFromFs(Packet* packet)
{
    char* data = packet->data();
    EndPoint* end_point = packet->end_point();
    uint32_t length = packet->data_length();
    data[length] = '\0';
    printf("-------------------from server-----------------------");
    printf("received: %s, length:%d\n", data, length);
    printf("-------------------from server-----------------------");

    char *path = new char[20 + length];
    FileUtils file_utils;
    snprintf(path, 20 + length, "server_data/%s", data);
    if (!file_utils.is_exists_file(path)) {
        printf("file %s did not exist!\n", path);
        return -1;
    }
    int size = 0;
    if ( ( size = file_utils.get_file_size(path) ) > 10 * 1024 *1024) {
        printf("file %s is too big!\n", path);
        return -1;
    }
    delete [] data;
    data = NULL;
    int rlength = file_utils.read(&data, size, path);
    printf("rlength=%d size=%d\n", rlength, size);
    delete [] path;
    packet->set_packet(data, size);
    StreamSocketContext* socket_context = end_point->socket_context();
    socket_context->AsyncSendPacket(packet);
    return 0;
}

int ServerHandler::OnReceived(Packet* packet, EndPoint* end_point)
{
    char* data = packet->data();
    uint32_t length = packet->data_length();
    data[length] = '\n';
    printf("-------------------from server-----------------------");
    printf("received: %s, length:%d\n", data, length);
    StreamSocketContext* socket_context = end_point->socket_context();
    socket_context->AsyncSendPacket(packet);
    return 0;
}
int ServerHandler::OnAccepted(int fd)
{
    printf("accepted fd :%d\n", fd);
    return 0;
}
int ServerHandler::OnSent(Packet* packet)
{
    char* data = packet->data();
    uint32_t length = packet->data_length();
    data[length] = '\n';
    printf("sent: %s\n", data);
    return 0;
}
int ServerHandler::OnTimeOut()
{
    return 0;
}

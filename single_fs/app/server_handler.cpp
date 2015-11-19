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
#include "common.h"
#include "log.h"
int ServerHandler::OnReceived(Packet* packet)
{
    return EchoFromIfs(packet);
}
int ServerHandler::EchoFromIfs(Packet* packet)
{
    InitedIfs* inited_ifs = Singleton<InitedIfs>::Instance();
    char* head = packet->head_data();
    uint32_t head_length = packet->head_length();

    RequestType reqest_type = (RequestType)ntohl(((int*)head)[0]);
    WLOG(DEBUG, "head length: %d, reqest_type_: %d", head_length, reqest_type);

    char* data = packet->data();
    EndPoint* end_point = packet->end_point();
    if (reqest_type == GET) {
        data[packet->data_length()] = '\0';
        int64_t file_id = (int64_t)atoi(data);
        if (file_id <= 0) {
            file_id = 10;
        }
        WLOG(DEBUG, "file_id=%lu", file_id);
        delete [] packet->data();

        int64_t file_length = inited_ifs->ifs()->get_length(file_id);
        Response response = SUCCESS;
        if (file_length < 0) {
            response = FILE_DID_NOT_EXIST;
            WLOG(WARN, "file of id %lu did not exist", file_id);
            file_length = 4;
        }
        char *buf = new char[file_length + 2];
        buf[file_length] = '\0';
        int64_t ret = inited_ifs->ifs()->read(file_id, buf, file_length);
        if (ret < 0) {
            if (response != SUCCESS) {
                response = OTHER_ERROR;
                WLOG(ERROR, "read failed!");
            }
        }
        printf("-------------------from server-----------------------");
        WLOG(DEBUG, "send:\n%s\nlength:%lu", buf, file_length);
        printf("-------------------from server-----------------------");

        int* head = new int[2];
        head[0] = htonl(response);
        packet->set_head((char*)head, 2 * sizeof(int));
        packet->set_packet(buf, (uint32_t) file_length);
        StreamSocketContext* socket_context = end_point->socket_context();
        socket_context->AsyncSendPacket(packet);
    } else if (reqest_type == PUT) {
        int file_id = inited_ifs->ifs()->write(data, packet->data_length());
        Response response = SUCCESS;
        if (file_id < 0) {
            response = OTHER_ERROR;
        }

        WLOG(INFO, "recvd data length is %d", packet->data_length());
        int ret = inited_ifs->ifs()->save_metadata();
        if (ret < 0) {
            response = OTHER_ERROR;
        }
        int* head = new int[2];
        head[0] = htonl(response);
        char* buf = new char[sizeof(int) + 1];
        ((int*)buf)[0] = htonl(file_id);
        WLOG(DEBUG, "recved:\n%s", data);
        packet->set_head((char*)head, 2 * sizeof(int));
        packet->set_packet(buf, sizeof(int));
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
    WLOG(DEBUG, "received: %s, length:%d", data, length);
    printf("-------------------from server-----------------------");

    char *path = new char[20 + length];
    FileUtils file_utils;
    snprintf(path, 20 + length, "server_data/%s", data);
    if (!file_utils.is_exists_file(path)) {
        WLOG(DEBUG, "file %s did not exist!", path);
        return -1;
    }
    int size = 0;
    if ( ( size = file_utils.get_file_size(path) ) > 10 * 1024 *1024) {
        WLOG(DEBUG, "file %s is too big!", path);
        return -1;
    }
    delete [] data;
    data = NULL;
    int rlength = file_utils.read(&data, size, path);
    WLOG(DEBUG, "rlength=%d size=%d", rlength, size);
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
    printf("-------------------from server-----------------------");
    WLOG(DEBUG, "received: %s, length:%d", data, length);
    StreamSocketContext* socket_context = end_point->socket_context();
    socket_context->AsyncSendPacket(packet);
    return 0;
}
int ServerHandler::OnAccepted(int fd)
{
    WLOG(DEBUG, "accepted fd :%d", fd);
    return 0;
}
int ServerHandler::OnSent(Packet* packet)
{
    char* data = packet->data();
    uint32_t length = packet->data_length();
    WLOG(DEBUG, "sent: %s", data);
    return 0;
}
int ServerHandler::OnTimeOut()
{
    return 0;
}

//
//
//
//
#include <stdint.h>
#include <arpa/inet.h>
#include "server_handler.h"
#include "end_point.h"
#include "stream_socket_context.h"
#include "file_utils.h"
#include "singleton.h"
#include "common.h"
#include "log.h"
#include "task_queue.h"
#include "package.h"
#include "task_node.h"
using myspace::TaskQueue;
int ServerHandler::OnReceived(Packet* packet)
{
    return EchoFromServer(packet);
}
int ServerHandler::EchoFromServer(Packet* packet)
{
    char* head = packet->head_data();
    // for (int32_t i = 0; i < packet->head_length(); ++i) {
    //     WLOG(DEBUG, "head[%d]:%c", i, head[i]);
    // }
    uint32_t head_length = packet->head_length();
    Package* package = new Package();
    package->set_data(head, head_length);
    TaskNode* task_node = new TaskNode();
    task_node->req_ = (RequestType)ntohl(package->GetInt32_t());
    int32_t path_length = ntohl(package->GetInt32_t());
    task_node->path_ = package->GetString(path_length);
    task_node->head_ = package;
    task_node->packet_ = packet;
    WLOG(DEBUG, "req  type: %d, path length: %d, path: %s",
            task_node->req_, path_length, task_node->path_->c_str());
    task_queue_->Push(task_node);
    return 0;

    RequestType reqest_type = (RequestType)ntohl(((int*)head)[0]);
    WLOG(DEBUG, "head length: %d, reqest_type_: %d", head_length, reqest_type);

    char* data = packet->data();
    EndPoint* end_point = packet->end_point();
    if (reqest_type == GET_CHUNK) {
        data[packet->data_length()] = '\0';
        int64_t file_id = (int64_t)atoi(data);
        if (file_id <= 0) {
            file_id = 10;
        }
        WLOG(DEBUG, "file_id=%lu", file_id);
        delete [] packet->data();

        int64_t file_length = 0;
        Response response = OK;
        if (file_length < 0) {
            response = FILE_DID_NOT_EXIST;
            WLOG(WARN, "file of id %lu did not exist", file_id);
            file_length = 4;
        }
        char *buf = new char[file_length + 2];
        buf[file_length] = '\0';
        int64_t ret = 0;
        if (ret < 0) {
            if (response != OK) {
                WLOG(ERROR, "read failed!");
            }
        }
        WLOG(DEBUG, "send:\n%s\nlength:%lu", buf, file_length);

        int* head = new int[2];
        head[0] = htonl(response);
        packet->set_head((char*)head, 2 * sizeof(int));
        packet->set_packet(buf, (uint32_t) file_length);
        StreamSocketContext* socket_context = end_point->socket_context();
        socket_context->AsyncSendPacket(packet);
    } else if (reqest_type == GET_CHUNK) {
    }
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
int ServerHandler::Init(myspace::TaskQueue<TaskNode*>* task_queue)
{
    task_queue_ = task_queue;
    return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>
#include "net_machine.h"
#include "packet.h"
#include "communicate_loop.h"
#include "net_handler.h"
#include "client_handler.h"
#include "stream_handler.h"
#include "stream_socket_context.h"
#include "end_point.h"
#include "singleton.h"
#include "file_utils.h"
#include "common.h"
#include "log.h"
#include "client.h"

int Client::Init()
{
    net_machine_ = Singleton<NetMachine>::Instance();

    net_handler_ = new ClientHandler();

    socket_context_ = new
        StreamSocketContext(ip_port_, net_handler_, net_machine_);
    socket_context_->Init();

    end_point_ = new EndPoint(socket_context_);
    socket_context_->set_end_point(end_point_);
    return 0;
}

int Client::Get(char* file_id, char** buf, int &length)
{
    Packet* packet = new Packet();
    char* data = new char[100];

    // get request;
    RequestType reqest_type = GET;
    int* head = new int[2];
    head[0] = htonl(reqest_type);
    ((int*)data)[0] = htonl(reqest_type);
    uint32_t len = snprintf(data, 100, file_id);
    packet->set_end_point(end_point_);
    packet->set_head((char*)head, 2 * sizeof(int));
    packet->set_packet(data, len + sizeof(int));
    void* ret_buf;
    net_machine_->SyncSendPacket(end_point_, packet, net_handler_, &ret_buf, 0);

    Packet* packet1 = (Packet*)ret_buf;
    char* data1 = packet1->data();
    uint32_t length1 = packet1->data_length();
    data1[length1] = '\0';
    char* path = new char[100];
    FileUtils file_utils;
    snprintf(path, 100, "client_data/%s", file_id);
    WLOG(INFO, "received length: %d", length1);
    int wlen = file_utils.write(data1, length1, path, true);
    delete packet1;
    delete [] path;
    return 0;
}

int Client::Put(char* path)
{
    if (!FileUtils::is_exists_file(path)) {
        WLOG(WARN, "file %s did not exist!", path);
        return -1;
    }
    int size = 0;
    if ( ( size = FileUtils::get_file_size(path) ) > 10 * 1024 *1024 ) {
        WLOG(WARN, "file %s is too big! will ignore", path);
        return -1;
    }

    char* data = NULL;
    int rlength = FileUtils::read(&data, size, path);
    WLOG(DEBUG, "rlength=%d size=%d content:%s", rlength, size, data);
    RequestType reqest_type = PUT;
    int* head = new int[2];
    head[0] = htonl(reqest_type);
    Packet* packet = new Packet();
    packet->set_head((char*)head, 2 * sizeof(int));
    packet->set_packet(data, size);
    void* ret_buf = NULL;
    net_machine_->SyncSendPacket(end_point_, packet, net_handler_, &ret_buf, 0);
    WLOG(INFO, "send length to put is %d", packet->data_length());

    Packet* packet1 = (Packet*)ret_buf;
    int* data1 = (int*)packet1->data();
    int file_id = ntohl(data1[0]);
    WLOG(DEBUG, "file id: %d", file_id);
    delete packet1;
    return 0;
}
int Delete(char* file_id)
{
    return 0;
}

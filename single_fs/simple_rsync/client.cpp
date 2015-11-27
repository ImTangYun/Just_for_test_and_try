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
#include "package.h"

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
int32_t Client::GetFile(char* src_path, char* dst_path, char* tmp_path)
{
    list<ChunkInfo*>* meta = GetMeta(src_path);
    return 0;
}
int32_t Client::PutFile(char* src_path, char* dst_path, char* tmp_path)
{
    return 0;
}
list<ChunkInfo*>* Client::GetMeta(char* path)
{
    char* data = new char[1];
    // get request;
    RequestType reqest_type = GET_META;
    WLOG(DEBUG, "get meta request");
    int32_t path_len = strlen(path);
    int32_t head_length = path_len + sizeof(int32_t) + sizeof(int32_t) + 1;
    char* head = new char[head_length];
    Package* package = new Package();
    package->set_data(head, head_length);
    package->SetInt32_t(htonl(static_cast<int32_t>(reqest_type)));
    package->SetInt32_t(htonl(static_cast<int32_t>(path_len)));
    package->SetString(path, path_len);

    Packet* packet = new Packet();
    packet->set_head(head, head_length);
    packet->set_packet(data, 1);

    void* ret_buf;
    net_machine_->SyncSendPacket(end_point_, packet, net_handler_,
            &ret_buf, 0);

    packet = (Packet*)ret_buf;
    package->set_data(packet->head_data(), packet->head_length());
    Response resp = (Response)(ntohl(package->GetInt32_t()));
    int32_t node_num = ntohl(package->GetInt32_t());
    if ((int)resp < 0) {
        WLOG(ERROR, "get file meta failed");
        if (resp = FILE_DID_NOT_EXIST) {
            WLOG(INFO, "file did not exist");
        }
        delete packet;
        return NULL;
    }
    list<ChunkInfo*>* meta = new list<ChunkInfo*>();
    package->set_data(packet->data(), packet->data_length());
    for (int32_t i = 0; i < node_num; ++i) {
        ChunkInfo* chunk_info = new ChunkInfo();
        chunk_info->weak_sum_ = ntohl(package->GetUint32_t());
        chunk_info->offset_ = ntohl(package->GetInt32_t());
        chunk_info->length_ = ntohl(package->GetInt32_t());
        chunk_info->strong_sum_ = package->GetString(32);
        WLOG(DEBUG, "wkhash: %u, of: %d, len:%d, sthash: %s",
                chunk_info->weak_sum_, chunk_info->offset_, chunk_info->length_,
                chunk_info->strong_sum_->c_str());
        meta->push_back(chunk_info);
    }

    delete packet;
    delete package;
    return meta;
}
list<ChunkInfo*>* ParseMeta(char* buf, int32_t node_num)
{
    return NULL;
}
int32_t Client::GetChunk(char** buf, int32_t offset, int32_t length)
{
    return 0;
}
int32_t Client::PutChunk(char* buf, int32_t offset, int32_t length)
{
    return 0;
}
int32_t Client::CommitFile()
{
    return 0;
}

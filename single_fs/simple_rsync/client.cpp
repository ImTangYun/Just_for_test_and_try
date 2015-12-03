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
#include "rsync_service.h"
#include "time_utils.h"
#include "file_map.h"

int Client::Init()
{
    net_machine_ = Singleton<NetMachine>::Instance();

    net_handler_ = new ClientHandler();

    socket_context_ = new
        StreamSocketContext(ip_port_, net_handler_, net_machine_);
    socket_context_->Init();

    end_point_ = new EndPoint(socket_context_);
    socket_context_->set_end_point(end_point_);

    rsync_service_ = new RsyncService();
    return 0;
}
int32_t Client::GetFile(char* src_path, char* dst_path, char* tmp_path)
{
    list<ChunkInfo*>* meta = GetMeta(src_path);
    if (FileUtils::is_exists_file(dst_path)
            || (FileUtils::get_file_size(dst_path) > 0)) {
        rsync_service_->ScanFile1(meta, dst_path);
    }
    TimeCounter time_counter;
    ConstructFile(src_path, dst_path, meta, tmp_path);
    time_counter.AddNow();
    WLOG(NOTICE, "construct file costs %.02fms", time_counter.GetTimeCosts(1));
    return 0;
}
void Client::ConstructFile(char* src_file, char* dst_file,
        list<ChunkInfo*>* file_meta, char* cst_file)
{
    FILE* fp = fopen(cst_file, "w");
    char* buf = NULL;
    FileMap* file_map = NULL;
    if (FileUtils::is_exists_file(dst_file))
        file_map = new FileMap(dst_file);
    for (auto iter = file_meta->begin(); iter != file_meta->end(); ++iter) {
        ChunkInfo* chunk_info = *iter;
        buf = NULL;
        if (true == chunk_info->from_) {
            WLOG(INFO, "from src file");
            GetChunk(&buf, chunk_info->offset_, chunk_info->length_, src_file);
            fwrite(buf, 1, chunk_info->length_, fp);
            WLOG(INFO, "buf ptr :%p, chunk length %d", buf, chunk_info->length_);
            delete [] buf;
        } else {
            WLOG(INFO, "from dst file");
            buf = file_map->MapFileContent(chunk_info->offset_, chunk_info->length_);
            fwrite(buf, 1, chunk_info->length_, fp);
        }
    }
    delete file_map;
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
        if (FILE_DID_NOT_EXIST == resp) {
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
        chunk_info->from_ = true;
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
int32_t Client::GetChunk(char** buf, int32_t offset, int32_t length, char* path)
{
    char* data = new char[1];
    // get request;
    RequestType reqest_type = GET_CHUNK;
    WLOG(DEBUG, "get meta request");
    int32_t path_len = strlen(path);
    int32_t head_length = path_len + sizeof(int32_t) * 4 + 1;
    char* head = new char[head_length];
    Package* package = new Package();
    package->set_data(head, head_length);
    package->SetInt32_t(htonl(static_cast<int32_t>(reqest_type)));
    package->SetInt32_t(htonl(static_cast<int32_t>(path_len)));
    package->SetString(path, path_len);
    package->SetInt32_t(htonl(static_cast<int32_t>(offset)));
    package->SetInt32_t(htonl(static_cast<int32_t>(length)));

    Packet* packet = new Packet();
    packet->set_head(head, head_length);
    packet->set_packet(data, 1);

    void* ret_buf;
    net_machine_->SyncSendPacket(end_point_, packet, net_handler_,
            &ret_buf, 0);

    packet = (Packet*)ret_buf;
    package->set_data(packet->head_data(), packet->head_length());
    Response resp = (Response)(ntohl(package->GetInt32_t()));
    if ((int)resp < 0) {
        WLOG(ERROR, "get file meta failed");
        if (FILE_DID_NOT_EXIST == resp) {
            WLOG(INFO, "file did not exist");
        }
        delete packet;
        return NULL;
    }

    *buf = packet->data();
    packet->set_packet(NULL, 0);
    delete packet;
    delete package;
    WLOG(DEBUG, "get chunk success!");
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
Client::~Client()
{
    delete net_handler_;
    delete socket_context_;
    delete rsync_service_;
    delete end_point_;
}

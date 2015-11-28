//
//
//
//
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include "end_point.h"
#include "stream_socket_context.h"
#include "task_queue.h"
#include "server_processor.h"
#include "log.h"
#include "task_node.h"
#include "common.h"
#include "rsync_service.h"
#include "file_utils.h"
#include "package.h"
#include "packet.h"
#include "time_utils.h"
using myspace::TaskQueue;
void ServerProcessor::Run(CThread* cthread, void* args)
{
    while (runnuing_) {
        Process(*(int32_t*)args);
        usleep(100);
    }
    delete (int32_t*)args;
}
void ServerProcessor::Process(int32_t thread_id)
{
    // WLOG(DEBUG, "from server processor, thread num %d", thread_id);
    TaskNode* node = task_queue_->Pop(100);
    if (node == NULL) {
        // WLOG(NOTICE, "no node");
        usleep(1000000);
        return;
    }
    WLOG(NOTICE, "have a task");
    if (node->req_ == GET_META) {
        GetMeta(node);
    } else if (node->req_ == GET_CHUNK) {
        TimeCounter time_counter;
        GetChunk(node);
        time_counter.AddNow();
        WLOG(NOTICE, "GetChunk cost %fms", time_counter.GetTimeCosts(1));
    }
}
void ServerProcessor::Init(int32_t thread_num, TaskQueue<TaskNode*>* task_queue)
{
    thread_num_ = thread_num;
    task_queue_ = task_queue;
    rsync_service_ = new RsyncService();
    for (int32_t i = 0; i < thread_num_; ++i) {
        threads_.push_back(new CThread());
    }
}
void ServerProcessor::GetMeta(TaskNode* node)
{
    Packet* packet = node->packet_;
    Package* package = new Package();
    EndPoint* end_point = packet->end_point();
    char* head = NULL;
    if (!FileUtils::is_exists_file(node->path_->c_str())) {
        WLOG(WARN, "file %s did not exist", node->path_->c_str());
        head = new char[sizeof(uint32_t) + 1];
        package->set_data(head, sizeof(uint32_t) + 1);
        package->SetUint32_t(htonl(static_cast<uint32_t>(FILE_DID_NOT_EXIST)));
        delete [] packet->data();
        delete [] packet->head_data();
        delete node->head_;
        delete node->path_;
        delete node;
        packet->set_head(head, sizeof(uint32_t));
        packet->set_packet(new char[1], 1);
        StreamSocketContext* socket_context = end_point->socket_context();
        socket_context->AsyncSendPacket(packet);
        return;
    }
    int32_t head_length = sizeof(int32_t) * 3 + 1;
    head = new char[head_length + 2];
    package->set_data(head, head_length);

    // response type
    package->SetInt32_t(htonl(static_cast<int32_t>(OK)));

    list<ChunkInfo*>* meta = rsync_service_->GenerateMetaList(node->path_->c_str());

    // meta size
    package->SetInt32_t(htonl(static_cast<int32_t>(meta->size())));

    // if ignore last
    package->SetInt32_t(htonl(static_cast<int32_t>(1)));
    delete [] node->packet_->data();
    delete [] node->packet_->head_data();
    delete node->head_;
    delete node->path_;
    delete node;
    packet->set_head(head, head_length);
    WLOG(NOTICE, "here2");

    // wkhash(uint)+offset(int)+length(int)+sthash(32 char)
    int32_t data_length = (meta->size()) * (sizeof(uint32_t) + sizeof(int32_t) * 2 + 32) + 10;
    char* data = new char[data_length];
    package->set_data(data, data_length);
    for (auto iter = meta->begin(); iter != meta->end(); ++iter) {
        WLOG(DEBUG, "wkhash: %u, of: %d, len:%d, st:%s",
                (*iter)->weak_sum_, (*iter)->offset_, (*iter)->length_,
                (*iter)->strong_sum_->c_str());
        package->SetUint32_t(htonl((*iter)->weak_sum_));
        package->SetInt32_t(htonl((*iter)->offset_));
        package->SetInt32_t(htonl((*iter)->length_));
        package->SetString((*iter)->strong_sum_->c_str(), (*iter)->strong_sum_->length());
    }
    packet->set_packet(data, data_length);
    StreamSocketContext* socket_context = end_point->socket_context();
    delete package;
    socket_context->AsyncSendPacket(packet);
}
void ServerProcessor::GetChunk(TaskNode* node)
{
    WLOG(DEBUG, "getting chunk request");
    Packet* packet = node->packet_;
    Package* package = new Package();
    EndPoint* end_point = packet->end_point();
    char* head = NULL;
    if (!FileUtils::is_exists_file(node->path_->c_str())) {
        WLOG(WARN, "file %s did not exist", node->path_->c_str());
        head = new char[sizeof(uint32_t) + 1];
        package->set_data(head, sizeof(uint32_t) + 1);
        package->SetUint32_t(htonl(static_cast<uint32_t>(FILE_DID_NOT_EXIST)));
        delete [] packet->data();
        delete [] packet->head_data();
        delete node->head_;
        delete node->path_;
        delete node;
        packet->set_head(head, sizeof(uint32_t));
        packet->set_packet(new char[1], 1);
        StreamSocketContext* socket_context = end_point->socket_context();
        socket_context->AsyncSendPacket(packet);
        return;
    }
    int32_t chunk_offset = ntohl(node->head_->GetInt32_t());
    int32_t chunk_length = ntohl(node->head_->GetInt32_t());

    WLOG(DEBUG, "get chunk, path: %s, offset: %d, length:%d",
            node->path_->c_str(), chunk_offset, chunk_length);
    char* buf;
    rsync_service_->GetChunk((char*)node->path_->c_str(),
            &buf, chunk_offset, chunk_length);

    int32_t head_length = sizeof(int32_t) * 3 + 1;
    head = new char[head_length + 2];
    package->set_data(head, head_length);

    // response type
    package->SetInt32_t(htonl(static_cast<int32_t>(OK)));

    list<ChunkInfo*>* meta = rsync_service_->GenerateMetaList(node->path_->c_str());

    delete [] node->packet_->data();
    delete [] node->packet_->head_data();
    delete node->head_;
    delete node->path_;
    delete node;
    packet->set_head(head, head_length);

    packet->set_packet(buf, chunk_length);
    StreamSocketContext* socket_context = end_point->socket_context();
    delete package;
    socket_context->AsyncSendPacket(packet);
}
void ServerProcessor::Start()
{
    if (runnuing_) return;
    runnuing_  = true;
    for (int32_t i = 0; i < threads_.size(); ++i) {
        int32_t* which_thread = new int32_t(i);
        threads_[i]->Start(this, (void*)which_thread);
    }
}
void ServerProcessor::Stop()
{
    runnuing_ = false;
    for (int32_t i = 0; i < threads_.size(); ++i) {
        threads_[i]->Join();
    }
}

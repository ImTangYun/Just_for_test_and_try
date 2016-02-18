//
//
//
#include <stdio.h>
#include <arpa/inet.h>
#include "task_queue.h"
#include "task_node.h"
#include "chat_process.h"
#include "log.h"
#include "packet.h"
#include "package.h"
#include "user_connection.h"
#include "stream_socket_context.h"
using namespace myspace;
void ChatProcess::Run(CThread* cthread, void* args)
{
    while (runnuing_) {
        Process(*(int32_t*)args);
        usleep(100);
    }   
    delete (int32_t*)args;
}
void ChatProcess::Process(int32_t thread_id)
{
    TaskNode* node = task_queue_->Pop(100);
    if (node == NULL) {
        usleep(100000);
        return;
    }
    WLOG(INFO, "received msg");
    if (node->req_ == SEND_MSG) {
        SendMessage(node);
    } else if (node->req_ == LOGIN) {
        Login(node);
    }
}

void ChatProcess::Login(TaskNode* node)
{
    WLOG(DEBUG, "login...");
    Package* package = new Package();
    Packet* packet = node->packet_;
    package->set_data(packet->data(), packet->data_length());
    int32_t sender_length = (int32_t)ntohl(package->GetInt32_t());
    if (sender_length > packet->data_length() - sizeof(int32_t)) {
        // release tasknode here
        return;
    }
    WLOG(DEBUG, "sender_length %d, data length %d", sender_length, packet->data_length());
    string* sender = package->GetString(sender_length);
    // string* sender = package->GetString(5);
    user_connection_->addUser(*sender, packet->end_point());
    WLOG(INFO, "user %s login", sender->c_str());
    delete sender;
}
void ChatProcess::SendMessage(TaskNode* node)
{
    WLOG(DEBUG, "send message");
    Package* package = new Package();
    Packet* packet = node->packet_;
    package->set_data(packet->data(), packet->data_length());
    int32_t sender_length = (int32_t)ntohl(package->GetInt32_t());
    string* sender = package->GetString(sender_length);
    int32_t receiver_length = (int32_t)ntohl(package->GetInt32_t());
    string* receiver = package->GetString(receiver_length);
    int32_t message_length = (int32_t)ntohl(package->GetInt32_t());
    string* message = package->GetString(message_length);

    if (*receiver == "world") {
        user_connection_->sendToAllUser(packet);
    } else {
        if (user_connection_->contains(*receiver)) {
            SendToOne(*receiver, packet);
            WLOG(DEBUG, "%s sent msg:\"%s\" to %s", sender->c_str(),
                    message->c_str(), receiver->c_str());
        }
    }
}
void ChatProcess::SendToOne(const string& user, Packet* packet)
{
    EndPoint* end_point = user_connection_->getEndPointByUser(user);
    if (end_point == NULL) {
        return;
    }
    StreamSocketContext* socket_context = end_point->socket_context();
    socket_context->AsyncSendPacket(packet);
}
void ChatProcess::Init(TaskQueue<TaskNode*>* task_queue,
        UserConnection* user_connection)
{
    task_queue_ = task_queue;
    user_connection_ = user_connection;
    thread_ = new CThread();
}
void ChatProcess::Start()
{
    if (runnuing_) return;
    runnuing_ = true;
    int32_t *which_thread = new int32_t(1);
    thread_->Start(this, (void*)which_thread);
}
void ChatProcess::Stop()
{
    runnuing_ = false;
    thread_->Join();
}

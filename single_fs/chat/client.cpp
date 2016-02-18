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
#include "time_utils.h"

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
void Client::SendMessage(const string& receiver, const string& message)
{
    int32_t head_length = sizeof(int32_t) + 1;
    RequestType reqest_type = SEND_MSG;
    char* head = new char[head_length];
    Package* package = new Package();
    package->set_data(head, head_length);
    package->SetInt32_t(htonl(static_cast<int32_t>(reqest_type)));
    Packet* packet = new Packet();
    packet->set_head(head, head_length);
    int32_t data_length = receiver.length() + sizeof(int32_t) +
        user_name_.length() + sizeof(int32_t) +
        message.length() + sizeof(int32_t) + 1;
    char* data = new char[data_length];
    package->set_data(data, data_length); 
    package->SetInt32_t(htonl(static_cast<int32_t>(user_name_.length())));
    package->SetString(user_name_.c_str(), user_name_.length());
    package->SetInt32_t(htonl(static_cast<int32_t>(receiver.length())));
    package->SetString(receiver.c_str(), receiver.length());
    package->SetInt32_t(htonl(static_cast<int32_t>(message.length())));
    package->SetString(message.c_str(), message.length());
    packet->set_packet(data, data_length);
    net_machine_->AsyncSendPacket(end_point_, packet, net_handler_);
}
void Client::Login(const string& user_name)
{
    int32_t head_length = sizeof(int32_t) + 1;
    RequestType reqest_type = LOGIN;
    char* head = new char[head_length];
    Package* package = new Package();
    package->set_data(head, head_length);
    package->SetInt32_t(htonl(static_cast<int32_t>(reqest_type)));
    Packet* packet = new Packet();
    packet->set_head(head, head_length);
    char* data = new char[user_name.length() + sizeof(int32_t) + 1];
    package->set_data(data, user_name.length() + sizeof(int32_t) + 1); 
    package->SetInt32_t(htonl(static_cast<int32_t>(user_name.length())));
    package->SetString(user_name.c_str(), user_name.length());
    // data += sizeof(int32_t);
    // snprintf(data, user_name.length() + 1, "%s", user_name.c_str());
    packet->set_packet(data, user_name.length() + sizeof(int32_t));
    net_machine_->AsyncSendPacket(end_point_, packet, net_handler_);
    user_name_ = user_name;
}
Client::~Client()
{
    delete net_handler_;
    delete socket_context_;
    delete end_point_;
}

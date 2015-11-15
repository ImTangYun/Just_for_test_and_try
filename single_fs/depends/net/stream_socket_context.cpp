#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <errno.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include "task_queue.h"
#include "stream_socket_context.h"
#include "communicate_loop.h"
#include "packet.h"
#include "net_handler.h"
#include "net_machine.h"
int StreamSocketContext::Init()
{
    if (ip_ != NULL) {
        printf("fd: %d had been inited before\n", fd_);
        return 0;
    }
    if (ParseIpPort() < 0) {
        printf("parse ip and port failed!\n");
        return -1;
    }
    printf("parse ip and port over, ip:%s, port:%d\n", ip_, port_);
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port_);
    inet_pton(AF_INET, ip_, &servaddr.sin_addr);

    printf("connectiong....\n");
    if (connect(fd_, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("connect error");
        return -1;
    }
    SetNonblocking(fd_);

    printf("return code of AddEvent is %d\n",
            net_machine_->communicate_loop()->AddEvent(this, true, true));
    return 0;
}

int StreamSocketContext::AsyncSendPacket(Packet* packet)
{
    if (packet == NULL) return -1;
    packet_queue_->Push(packet);
    communicate_loop_->SetEvent(this, true, true);
    return 0;
}

int StreamSocketContext::HandleOutput()
{
    Packet* packet = packet_queue_->Pop(1000);
    if (packet == NULL) {
        return -1;
    }
    uint32_t channel_id = htonl(packet->channel_id());
    uint32_t data_length = htonl(packet->data_length());
    printf("sending channel_id:%d, data_length:%d\n", channel_id, packet->data_length());
    char* data = packet->data();
    uint32_t total_length = sizeof(channel_id) +
        sizeof(data_length) + packet->data_length();
    char* buff = new char[2 * sizeof(uint32_t) + sizeof(channel_id) +
        sizeof(data_length) + packet->data_length() + 1];
    uint32_t* buff_int = reinterpret_cast<uint32_t*>(buff);
    buff_int[0] = htonl(1);
    buff_int[1] = htonl(total_length);
    buff_int[2] = channel_id;
    buff_int[3] = data_length;
    char* buff_data = buff + sizeof(channel_id) + sizeof(data_length) + 2 * sizeof(uint32_t);
    memcpy(buff_data, data, packet->data_length());
    delete packet;
    Send(buff, total_length + 2 * sizeof(uint32_t));
    delete [] buff;
    return 0;
}

int StreamSocketContext::Send(char* data, uint32_t length)
{
    uint32_t sent_length = 0;
    while (true) {
        int send_buf = length - sent_length;
        if (send_buf > 4096) {
            send_buf = 4096;
        }
        int32_t ret = send(fd_, data + sent_length, send_buf, 0);
        if (ret < 0) {
            // printf("sending error!\n");
            if (sent_length == length) {
                return 0;
            }
            usleep(10);
            continue;
        }
        printf("sent length: %u\n", sent_length);
        sent_length += ret;
        if (sent_length == length) {
            return 0;
        }
    }
}

int StreamSocketContext::ParseHeader(uint32_t &total_len)
{
    uint32_t head[2];
    int ret = recv(fd_, head, 2 * sizeof(uint32_t), 0);
    printf("recv head len = %d\n", ret);
    if (ret <= 0) {
        return -1;
    }
    uint32_t flag = ntohl(head[0]);
    if (flag != 1) {
        printf("can not recognize stream\n");
        return -2;
    }
    total_len = ntohl(head[1]);
    return 0;
}

int StreamSocketContext::HandleInput() {
    uint32_t received_length = 0;
    uint32_t total_length = 0;
    int ret = ParseHeader(total_length);
    if (ret == -1) {
        printf("disconnected\n");
        return -1;
    }
    if (ret < 0) return ret;
    ReallocBuffer(total_length + 10, 0);
    while (true) {
        int ret = recv(fd_, recv_buffer_ + received_length,
                recv_buffer_length_ - received_length, 0);
        // printf("ret=%d\n", ret);
        if (ret <= 0) {
            if (received_length == total_length) break;
            continue;
        }
        received_length += ret;
        // AdjustBuffer(received_length);
    }
    uint32_t* head = reinterpret_cast<uint32_t*>(recv_buffer_);
    uint32_t channel_id = ntohl(head[0]);
    uint32_t data_length = ntohl(head[1]);
    char* data = new char[received_length + 2];
    printf("recevied length = %d\n", received_length);
    printf("data length = %d\n", data_length);
    memcpy(data, recv_buffer_ + sizeof(channel_id) +
            sizeof(data_length), received_length);
    Packet* packet = new Packet(channel_id);
    packet->set_packet(data, data_length);
    packet->set_end_point(end_point_);
    if (received_length == 0) {
        printf("disconnected\n");
        return -1;
    }
    if (need_ack_) {
        *ret_buf_ = (void*)packet;
        replied_ = true;
    }
    net_handler_->OnReceived(packet);
    return 0;
}

int StreamSocketContext::AdjustBuffer(int received_length)
{
    if (received_length >= (recv_buffer_length_ / 2) &&
            recv_buffer_length_ < 1024 * 1024 * 10) {
        ReallocBuffer(2 * recv_buffer_length_, received_length);
    } else if (received_length < recv_buffer_length_ / 3 &&
            recv_buffer_length_ > 111) {
        ReallocBuffer(recv_buffer_length_ / 2, received_length);
    }
    return 0;
}
int StreamSocketContext::ReallocBuffer(int new_length, int cp_length)
{
        char* new_buffer = new char[new_length];
        memcpy(new_buffer, recv_buffer_, cp_length);
        delete [] recv_buffer_;
        recv_buffer_ = new_buffer;
        recv_buffer_length_ = new_length;
        printf("new alloced recv_buffer_length_=%d\n", recv_buffer_length_);
        return 0;
}

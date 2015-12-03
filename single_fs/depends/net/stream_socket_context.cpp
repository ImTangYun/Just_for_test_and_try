#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include "task_queue.h"
#include "stream_socket_context.h"
#include "packet.h"
#include "net_machine.h"
#include "log.h"


int StreamSocketContext::Init()
{
    if (ip_ != NULL) {
        WLOG(INFO, "fd: %d had been inited before", fd_);
        return 0;
    }
    if (ParseIpPort() < 0) {
        WLOG(ERROR, "parse ip and port failed!");
        return -1;
    }
    WLOG(DEBUG, "parse ip and port over, ip:%s, port:%d", ip_, port_);
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port_);
    inet_pton(AF_INET, ip_, &servaddr.sin_addr);

    WLOG(DEBUG, "connectiong....");
    if (connect(fd_, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("connect error");
        return -1;
    }
    SetNonblocking(fd_);
    net_handler_->OnConnected(fd_);

    WLOG(DEBUG, "return code of AddEvent is %d",
            net_machine_->communicate_loop()->AddEvent(this, true, true));
    return 0;
}

StreamSocketContext::~StreamSocketContext()
{
    communicate_loop_->ClearEvent(this);
    close(fd());
    net_handler_->OnDisconnected(fd());
    delete packet_queue_;
    packet_queue_ = NULL;
    delete [] recv_buffer_;
    recv_buffer_ = NULL;
    delete end_point_;
    end_point_ = NULL;
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

    WLOG(DEBUG, "the fd of the connection is %d", fd_);

    // send protocool code
    uint32_t protocool_code = 1;
    uint32_t net_protocool_code = htonl(protocool_code);
    Send((char*)(&net_protocool_code), sizeof(net_protocool_code));

    // send whole packet size channel_id-head_length-head_data-data_length-data
    uint32_t whole_length = sizeof(uint32_t) + sizeof(uint32_t) + 
        packet->head_length() + sizeof(uint32_t) + packet->data_length();
    uint32_t net_whole_length = htonl(whole_length);
    Send((char*)(&net_whole_length), sizeof(net_whole_length));

    // send channel id
    uint32_t channel_id = htonl(packet->channel_id());
    Send((char*)(&channel_id), sizeof(channel_id));

    // send packet head data length
    uint32_t head_length = htonl(packet->head_length());
    Send((char*)(&head_length), sizeof(head_length));

    // send head data
    Send(packet->head_data(), packet->head_length());

    //send data length
    uint32_t data_length = htonl(packet->data_length());
    Send((char*)(&data_length), sizeof(data_length));

    // send packet data
    Send(packet->data(), packet->data_length());

    net_handler_->OnSent(packet);
    delete packet;
    return 0;
}

int StreamSocketContext::Send(char* data, uint32_t length)
{
    if (length == 0) {
        return 0;
    }
    uint32_t sent_length = 0;
    while (true) {
        int send_buf = length - sent_length;
        if (send_buf > 4096) {
            send_buf = 4096;
        }
        int32_t ret = send(fd_, data + sent_length, send_buf, 0);
        if (ret < 0) {
            // WLOG(DEBUG, "sending error!");
            if (sent_length == length) {
                return 0;
            }
            usleep(10);
            continue;
        }
        sent_length += ret;
        if (sent_length == length) {
            return 0;
        }
    }
}

int StreamSocketContext::ParseHeader(uint32_t &total_len)
{
    uint32_t net_protocool;
    int ret = recv(fd_, &net_protocool, sizeof(uint32_t), 0);
    WLOG(DEBUG, "recv head len = %d", ret);
    if (ret <= 0) {
        return -1;
    }
    uint32_t flag = ntohl(net_protocool);
    if (flag != 1) {
        WLOG(WARN, "can not recognize stream");
        return -2;
    }

    uint32_t* len = (uint32_t*)Recv(sizeof(uint32_t));
    if (len == NULL) {
        WLOG(WARN, "recv time out");
        return -3;
    }
    total_len = ntohl(len[0]);
    delete [] len;
    return 0;
}

void* StreamSocketContext::Recv(uint32_t length)
{
    char* data = new char[length];
    uint32_t received = 0;
    uint32_t time = 0;
    while (true) {
        int ret = recv(fd_, data + received, length - received, 0);
        if (ret <= 0) {
            if (received == length) {
                return data;
            }
            usleep(10);
            time += 10;
            if (time > 1000000) {
                delete [] data;
                return NULL;
            }
            continue;
        }
        received += ret;
        time = 0;
        if (received == length) {
            return data;
        }
    }
}

int StreamSocketContext::HandleInput() {
    uint32_t received_length = 0;
    uint32_t total_length = 0;
    int ret = ParseHeader(total_length);
    WLOG(DEBUG, "total_length: %u", total_length);
    if (ret == -1) {
        WLOG(INFO, "disconnected");
        return -1;
    }
    if (ret < 0) return ret;
    if (total_length > recv_buffer_length_)
        ReallocBuffer(total_length + 10, 0);
    else if (total_length < recv_buffer_length_ / 3 && total_length > 111)
        ReallocBuffer(recv_buffer_length_ / 2, 0);

    clock_t start, finish;
    start = clock();
    while (true) {
        int ret = recv(fd_, recv_buffer_ + received_length,
                recv_buffer_length_ - received_length, 0);
        // WLOG(DEBUG, "ret=%d", ret);
        if (ret <= 0) {
            if (received_length == total_length) break;
            continue;
        }
        received_length += ret;
        // AdjustBuffer(received_length);
    }
    finish = clock();
    WLOG(NOTICE, "recv data cost %fs ", (double)(finish - start) / CLOCKS_PER_SEC);

    uint32_t* head = reinterpret_cast<uint32_t*>(recv_buffer_);
    uint32_t channel_id = ntohl(head[0]);

    uint32_t head_length = ntohl(head[1]);

    char* head_ptr = recv_buffer_ + sizeof(channel_id) + sizeof(head_length);
    char* head_data = new char[head_length];
    memcpy(head_data, head_ptr, head_length);

    uint32_t* data_length_ptr = (uint32_t*)(head_ptr + head_length);
    uint32_t data_length = ntohl(*data_length_ptr);

    char* data_ptr = head_ptr + head_length + sizeof(data_length);
    char* data = new char[data_length + 5];
    memcpy(data, data_ptr, data_length);

    Packet* packet = new Packet();
    packet->set_head(head_data, head_length);
    packet->set_packet(data, data_length);
    packet->set_end_point(end_point_);

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
    WLOG(DEBUG, "new alloced recv_buffer_length_=%d", recv_buffer_length_);
    return 0;
}

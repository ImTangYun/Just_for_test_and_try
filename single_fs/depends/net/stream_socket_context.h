//
//
//

#ifndef STREAM_SOCKET_CONTEXT
#define STREAM_SOCKET_CONTEXT
#include <string>
#include "task_queue.h"
#include "socket_context.h"
#include "end_point.h"
#include "communicate_loop.h"
using std::string;
using myspace::TaskQueue;
class Packet;
class NetHandler;
class NetMachine;
class StreamSocketContext: public SocketContext
{
    public:
        StreamSocketContext(const string &ip_port, NetHandler* net_handler, NetMachine* net_machine):
                SocketContext(ip_port, NULL), net_machine_(net_machine),
                packet_queue_(new TaskQueue<Packet*>()), recv_buffer_(new char[111]),
                recv_buffer_length_(111),  ret_buf_(NULL), replied_(false), need_ack_(false),
                net_handler_(net_handler){}
        StreamSocketContext(const string &ip_port,
                CommunicateLoop* communicate_loop, NetHandler* net_handler):
                SocketContext(ip_port, communicate_loop),
                packet_queue_(new TaskQueue<Packet*>()), recv_buffer_(new char[111]),
                recv_buffer_length_(111), ret_buf_(NULL), replied_(false), need_ack_(false),
                net_handler_(net_handler){}
        ~StreamSocketContext()
        {
            communicate_loop_->ClearEvent(this);
            delete packet_queue_;
            packet_queue_ = NULL;
            delete [] recv_buffer_;
            recv_buffer_ = NULL;
            delete end_point_;
            end_point_ = NULL;
        }
        int Init();
        int AsyncSendPacket(Packet* packet);
        virtual int HandleOutput();
        virtual int HandleInput();
        virtual void OnReceived() {}
        int AdjustBuffer(int received_length);
        int ParseHeader(uint32_t &total_len);
        int ReallocBuffer(int new_length, int cp_length);
        void set_end_point(EndPoint* end_point)
        {
            end_point_ = end_point;
        }
        EndPoint* end_point()
        {
            return end_point_;
        }
        void set_sync(void** ret_buf)
        {
            ret_buf_ = ret_buf;
            replied_ = false;
            need_ack_ = true;
        }
        bool replied()
        {
            return replied_;
        }
    private:
        void* Recv(uint32_t length);
        int Send(char* data, uint32_t length);
        TaskQueue<Packet*>* packet_queue_;
        char* recv_buffer_;
        int recv_buffer_length_;
        NetHandler* net_handler_;
        NetMachine* net_machine_;
        EndPoint* end_point_;
        void** ret_buf_;
        bool replied_;
        bool need_ack_;
};

#endif

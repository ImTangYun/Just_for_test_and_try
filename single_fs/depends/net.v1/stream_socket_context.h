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
#include "net_handler.h"

#define DEFAULT_BUFFER_SIZE 1024 * 1024 * 4
#define HEAD 1

using std::string;
using myspace::TaskQueue;
class Packet;
class NetMachine;
class StreamSocketContext: public SocketContext
{
    public:
        StreamSocketContext(const string &ip_port, NetHandler* net_handler, NetMachine* net_machine):
                SocketContext(ip_port, NULL), net_machine_(net_machine),
                packet_queue_(new TaskQueue<Packet*>()), recv_buffer_(new char[DEFAULT_BUFFER_SIZE]),
                recv_buffer_length_(DEFAULT_BUFFER_SIZE),  ret_buf_(NULL), replied_(false), need_ack_(false),
                buffer_offset_(0), net_handler_(net_handler){}
        StreamSocketContext(const string &ip_port,
                CommunicateLoop* communicate_loop, NetHandler* net_handler):
                SocketContext(ip_port, communicate_loop),
                packet_queue_(new TaskQueue<Packet*>()), recv_buffer_(new char[DEFAULT_BUFFER_SIZE]),
                recv_buffer_length_(DEFAULT_BUFFER_SIZE), ret_buf_(NULL), replied_(false), need_ack_(false),
                buffer_offset_(0), net_handler_(net_handler){}
        ~StreamSocketContext();
        int Init();
        int AsyncSendPacket(Packet* packet);
        virtual int HandleOutput();
        virtual int HandleInput();
        int HandleInputImpl();
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
        int Send(char* data, int32_t length);
        void Chunking();
        TaskQueue<Packet*>* packet_queue_;
        char* recv_buffer_;
        int recv_buffer_length_;
        int buffer_offset_;
        NetHandler* net_handler_;
        NetMachine* net_machine_;
        EndPoint* end_point_;
        void** ret_buf_;
        bool replied_;
        bool need_ack_;
};

#endif

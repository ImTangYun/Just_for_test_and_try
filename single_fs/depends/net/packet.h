//
//
//
//
#ifndef PACKET
#define PACKET
#include <stdint.h>
#include <string.h>
#include "singleton.h"
#include "channel.h"
class EndPoint;
class Packet
{
    public:
        Packet():channel_id_(Singleton<Channel>::Instance()->max_id()),
            head_length_(0), data_length_(0), head_data_(NULL), data_(NULL){}
        Packet(uint32_t channel_id):channel_id_(channel_id) {}
        ~Packet()
        {
            delete [] data_;
            data_ = NULL;
            delete [] head_data_;
            head_data_ = NULL;
        }
        void set_head(char* head_data, uint32_t head_length)
        {
            head_data_ = head_data;
            head_length_ = head_length;
        }
        char* head_data()
        {
            return head_data_;
        }
        uint32_t head_length()
        {
            return head_length_;
        }
        void set_packet(char* data, uint32_t data_length)
        { 
            data_ = data;
            data_length_ = data_length;
        }

        void set_channel_id(uint32_t channel_id) {channel_id_ = channel_id;}
        uint32_t channel_id() {return channel_id_;}
        void set_end_point(EndPoint* end_point)
        {
            end_point_ = end_point;
        }

        char* data() {return data_;}
        int64_t data_length() {return data_length_;}
        EndPoint* end_point() {return end_point_;}
        Packet* GetCopy() 
        {
            char* head_data = new char[head_length_];
            memmove(head_data, head_data_, head_length_);
            char* data = new char[data_length_];
            memmove(data, data_, data_length_);
            Packet* packet = new Packet();
            packet->set_head(head_data, head_length_);
            packet->set_packet(data, data_length_);
            packet->set_channel_id(channel_id_);
            packet->set_end_point(end_point_);
            return packet;
        }
    private:
        char* head_data_;
        uint32_t head_length_;
        char* data_;
        uint32_t data_length_;
        uint32_t channel_id_;
        EndPoint* end_point_;
};

#endif

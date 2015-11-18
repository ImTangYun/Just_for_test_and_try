//
//
//
//
#ifndef PACKET
#define PACKET
#include <stdint.h>
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
    private:
        char* head_data_;
        uint32_t head_length_;
        char* data_;
        uint32_t data_length_;
        uint32_t channel_id_;
        EndPoint* end_point_;
};

#endif

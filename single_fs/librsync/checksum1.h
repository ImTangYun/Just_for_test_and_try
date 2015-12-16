//
//
//
//
#ifndef CHECKSUM1_H_
#define CHECKSUM1_H_
#include <stdint.h>
#include <queue>
#include <string>
#include "basesum.h"
#define schar unsigned char
#define CHAR_OFFSET 0
#define PRIME 49667
using std::queue;
using std::string;
class Checksum1: public Basesum
{
    public:
        Checksum1():power_map_(NULL), length_(0), tmp_sum_(0){}
        uint32_t Sum1(char* buf1, int32_t length);
        void Init(char* buf, int32_t length);
        uint32_t tmp_sum();
        uint32_t Update(char next);
        string* StrongSum(char* data, int32_t length);
        // static uint32_t Sum1(char* buf1, int32_t length);
        // void Init(char* buf, int32_t length);
        // uint32_t tmp_sum();
        // uint32_t Update(char next);
        // static string* StrongSum(char* data, int32_t length);
        ~Checksum1()
        {
            delete [] power_map_;
        }
    private:
        void Clear();
        queue<char> buf_list_;
        uint32_t *power_map_;
        int32_t length_;
        uint32_t tmp_sum_;
};
#endif

//
//
//
//
#ifndef CHECKSUM3_H_
#define CHECKSUM3_H_
#include <stdint.h>
#include <openssl/md5.h>
#include <openssl/md4.h>
#include <queue>
#include <string>
#include "basesum.h"
#include "circle_queue.h"
#include "common.h"
#define schar unsigned char
#define CHAR_OFFSET 0
#define PRIME 49667
#define MDX_CTX MD4_CTX
using type::CircleQueue;
using std::string;
class Checksum3: public Basesum
{
    public:
        Checksum3():length_(0), tmp_sum_(0), buf_list_(1024* 1024 * 4){}
        uint32_t Sum1(char* buf1, int32_t length);
        void Init(char* buf, int32_t length);
        uint32_t tmp_sum();
        uint32_t Update(char next);
        string* StrongSum(char* data = NULL, int32_t length = 0);
        // static uint32_t Sum1(char* buf1, int32_t length);
        // void Init(char* buf, int32_t length);
        // uint32_t tmp_sum();
        // uint32_t Update(char next);
        // static string* StrongSum(char* data, int32_t length);
        ~Checksum3()
        {
        }
    private:
        void Clear();
        CircleQueue<char> buf_list_;
        int32_t length_;
        uint32_t tmp_sum_;
        MDX_CTX ctx_;
};
#endif

//
//
//
//
#include <stdio.h>
#include <openssl/md5.h>
#include <openssl/md4.h>
#include <openssl/md2.h>
#include <string.h>
#include "checksum3.h"

uint32_t Checksum3::Sum1(char* buf1, int32_t length)
{
    int32_t i;
    uint32_t s1, s2;
    schar *buf = (schar *)buf1;

    s1 = s2 = 0;
    for (i = 0; i < ( length - 4 ); i+=4) {
        s2 += 4*(s1 + buf[i]) + 3*buf[i+1] + 2*buf[i+2] + buf[i+3] +
            10*CHAR_OFFSET;
        s1 += (buf[i+0] + buf[i+1] + buf[i+2] + buf[i+3] + 4*CHAR_OFFSET);
    }
    for (; i < length; i++) {
        s1 += (buf[i]+CHAR_OFFSET);
        s2 += s1;
    }
    return (s1 & 0xffff) + (s2 << 16);
}
void Checksum3::Init(char* buf, int32_t length)
{
    length_ = length;
    Clear();
    uint32_t ret = static_cast<schar>(buf[0]);
    buf_list_.push(buf[0]);
    for (int i = 1; i < length_; ++i) {
        buf_list_.push(buf[i]);
    }
    tmp_sum_ = Sum1(buf, length);
}

void Checksum3::Clear()
{
    buf_list_.clear();
}

uint32_t Checksum3::tmp_sum()
{
    return tmp_sum_;
}
uint32_t Checksum3::Update(char next)
{
    uint32_t s1, s2;
    s1 = tmp_sum_ & 0xffff;
    s2 = tmp_sum_ >> 16;
    buf_list_.push(next);
    s1 += static_cast<schar>(next) + CHAR_OFFSET;
    s1 -= static_cast<schar>(buf_list_.front()) + CHAR_OFFSET;
    s2 += s1;
    s2 -= (length_ & 0xffff) * (static_cast<schar>(buf_list_.front()) + CHAR_OFFSET);
    buf_list_.pop();
    tmp_sum_ = (s1 & 0xffff) + (s2 << 16);
    return tmp_sum_;
}

string* Checksum3::StrongSum(char* data, int32_t length)
{
    unsigned char md[16];
    char buf[33]={'\0'};
    char tmp[3]={'\0'};
    int i;
    MD4_Init(&ctx_);
    if (data != NULL) {
        MD4_Update(&ctx_, data, length);
    } else {
        MD4_Update(&ctx_, buf_list_.GetData(), buf_list_.size());
    }
    MD4_Final(md,&ctx_);
    for ( i=0; i<16; i++ ) {
        sprintf(tmp, "%02X", md[i]);
        strcat(buf, tmp);
    }   
    return new string(buf);
}

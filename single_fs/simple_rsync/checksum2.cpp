//
//
//
//
#include <stdio.h>
#include <openssl/md5.h>
#include <openssl/md4.h>
#include <openssl/md2.h>
#include <string.h>
#include "checksum2.h"

/*
uint32_t Checksum::Sum1(char* buf1, int32_t length)
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
        s1 += (buf[i]+CHAR_OFFSET); s2 += s1;
    }
    return (s1 & 0xffff) + (s2 << 16);
}
*/
uint32_t Checksum2::Sum1(char* buf1, int32_t length)
{
    uint32_t ret = static_cast<schar>(buf1[0]);
    for (int i = 1; i < length; ++i) {
        ret *= PRIME;
        ret += static_cast<schar>(buf1[i]);
    }
    return ret;
}
void Checksum2::Init(char* buf, int32_t length)
{
    length_ = length;
    if (NULL == power_map_) {
        power_map_ = new uint32_t[256];
        uint32_t length_power_prime = 1;
        for (int i = 0; i < length_; ++i) {
            length_power_prime *= PRIME;
        }
        for (int i = 0; i < 256; ++i) {
            power_map_[i] = (uint32_t)i * length_power_prime;
        }
    }
    Clear();
    uint32_t ret = static_cast<schar>(buf[0]);
    buf_list_.push(buf[0]);
    for (int i = 1; i < length_; ++i) {
        ret *= PRIME;
        buf_list_.push(buf[i]);
        ret += static_cast<schar>(buf[i]);
    }
    tmp_sum_ = ret;
}

void Checksum2::Clear()
{
    buf_list_.clear();
}

uint32_t Checksum2::tmp_sum()
{
    return tmp_sum_;
}
uint32_t Checksum2::Update(char next)
{
    buf_list_.push(next);
    tmp_sum_ *= PRIME;
    tmp_sum_ += static_cast<schar>(next);
    tmp_sum_ -= power_map_[static_cast<schar>(buf_list_.front())];
    buf_list_.pop();
    return tmp_sum_;
}

string* Checksum2::StrongSum(char* data, int32_t length)
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

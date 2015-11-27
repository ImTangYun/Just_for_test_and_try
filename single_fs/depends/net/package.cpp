//
//
//
//
#include <stdint.h>
#include <string.h>
#include <string>
#include "package.h"
using std::string;

int32_t Package::GetInt32_t()
{
    if (left_length_ >= sizeof(int32_t)) {
        int32_t* p = (int32_t*)pointer_;
        pointer_ += sizeof(int32_t);
        left_length_ -= sizeof(int32_t);
        return p[0];
    }
    return 0;
}
int32_t Package::SetInt32_t(int32_t int_data)
{
    if (left_length_ >= sizeof(int32_t)) {
        int32_t* p = (int32_t*)pointer_;
        pointer_ += sizeof(int32_t);
        left_length_ -= sizeof(int32_t);
        p[0] = int_data;
        return int_data;
    }
    return 0;
}

string* Package::GetString(int32_t length)
{
    if (left_length_ < length) return NULL;
    string* str = new string();
    for (int i = 0; i < length; ++i) {
        *str += *pointer_;
        ++pointer_;
    }
    left_length_ -= length;
    return str;
}
void Package::SetString(const string& str)
{
}
void Package::SetString(const char* str, int32_t length)
{
    if (length > left_length_) {
        return;
    }
    memmove(pointer_, str, length);
    pointer_ += length;
    left_length_ -= length;
}

uint32_t Package::GetUint32_t()
{
    if (left_length_ >= sizeof(uint32_t)) {
        uint32_t* p = (uint32_t*)pointer_;
        pointer_ += sizeof(uint32_t);
        left_length_ -= sizeof(uint32_t);
        return p[0];
    }
    return 0;
}
uint32_t Package::SetUint32_t(uint32_t uint_data)
{
    if (left_length_ >= sizeof(uint32_t)) {
        uint32_t* p = (uint32_t*)pointer_;
        pointer_ += sizeof(uint32_t);
        left_length_ -= sizeof(uint32_t);
        p[0] = uint_data;
        return uint_data;
    }
    return 0;
}

char Package::GetChar()
{
    return '0';
}
void Package::SetChar(char c)
{
}

void Package::set_data(char* data, int32_t length)
{
    data_ = data;
    data_length_ = length;
    left_length_ = length;
    pointer_ = data_;
}

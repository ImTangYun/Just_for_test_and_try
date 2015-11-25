//
//
//
//
#ifndef BASESUM_H_
#define BASESUM_H_
#include <stdint.h>
#include <string>
#define schar unsigned char
using std::list;
using std::string;
class Basesum
{
    public:
        virtual uint32_t Sum1(char* buf1, int32_t length) = 0;
        virtual void Init(char* buf, int32_t length) = 0;
        virtual uint32_t tmp_sum() = 0;
        virtual uint32_t Update(char next) = 0;
        virtual string* StrongSum(char* data, int32_t length) = 0;
};
#endif

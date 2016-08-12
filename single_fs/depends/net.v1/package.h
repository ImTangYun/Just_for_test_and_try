//
//
//
//
#ifndef PACKAGE_H_
#define PACKAGE_H_
#include <stdint.h>
#include <string>

using std::string;
class Package
{
    public:
        Package():data_(NULL), pointer_(NULL), data_length_(0), left_length_(0){}
        int32_t GetInt32_t();
        int32_t SetInt32_t(int32_t int_data);

        string* GetString(int32_t length);
        void SetString(const string& str);
        void SetString(const char* str, int32_t length);

        uint32_t GetUint32_t();
        uint32_t SetUint32_t(uint32_t uint_data);

        char GetChar();
        void SetChar(char c);

        void set_data(char* data, int32_t length);
        
        char* data()
        {
            return data_;
        }

        int32_t length()
        {
            return data_length_;
        }
        int32_t left_length()
        {
            return left_length_;
        }
    private:
        char* data_;
        char* pointer_;
        int32_t data_length_;
        int32_t left_length_;
};

#endif

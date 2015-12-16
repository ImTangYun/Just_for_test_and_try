//
//
//
#ifndef WRITE_FILE_MAP_H_
#define WRITE_FILE_MAP_H_
#include <stdint.h>
#include <stdio.h>
#define BUFFER_LENGTH (1024 * 1024 * 4)

class WriteFileMap
{
    public:
        WriteFileMap(const char* file):fp_(NULL), offset_(0), length_(0),
            capacity_(BUFFER_LENGTH)
        {
            buffer_ = new char[capacity_];
            fp_ = fopen(file, "wb");
            if (fp_ == NULL) {
                printf("open file  %s failed\n", file);
            }
        }
        WriteFileMap(FILE* file):fp_(NULL), offset_(0), length_(0),
            capacity_(BUFFER_LENGTH)
        {
            buffer_ = new char[capacity_];
            fp_ = file;
            if (fp_ == NULL) {
                printf("open file  %s failed\n", file);
            }
        }
        void Flush();
        char* MapToFile(int32_t offset, int32_t length);
        void AppendData(const char* data, int32_t length);
        int32_t offset()
        {
            return offset_;
        }
        int32_t length()
        {
            return length_;
        }
        ~WriteFileMap()
        {
            Flush();
            delete [] buffer_;
            fclose(fp_);
        }

    private:
        int32_t offset_;
        int32_t length_;
        FILE* fp_;
        char* buffer_;
        int32_t capacity_;
};

#endif

//
//
//
//
#ifndef FILE_MAP_H_
#define FILE_MAP_H_
#define DEFAULT_BUFFER_LENGTH (1024 * 1024 * 20)
#include <stdio.h>
#include <stdint.h>
#include "log.h"
#include "file_utils.h"
class FileMap
{
    public:
        FileMap(char* file):offset_(0), fp_(NULL)
        {
            file_size_ = FileUtils::get_file_size(file);
            buffer_length_ = DEFAULT_BUFFER_LENGTH;
            if (buffer_length_ > file_size_) {
                length_ = file_size_;
                buffer_length_ = length_;
            }
            data_ = new char[buffer_length_];
            fp_ = fopen(file, "rb");
            if (fp_ == NULL) {
                WLOG(ERROR, "open file %s failed", file);
            }
            length_ = fread(data_, 1, length_, fp_);
        }

        char* MapFileContent(int32_t offset, int32_t length);

        ~FileMap()
        {
            if (fp_ != NULL)
                fclose(fp_);
            fp_ = NULL;
            delete [] data_;
        }
    private:
        char* data_;
        int32_t offset_;
        int32_t buffer_length_;
        int32_t length_;
        FILE* fp_;
        int32_t file_size_;
};
#endif

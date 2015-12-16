//
//
//
//
#include "file_map.h"

char* FileMap::MapFileContent(int32_t offset, int32_t length)
{
    // WLOG(DEBUG, "offset:%d, length:%d", offset, length);
    if (offset > file_size_) return NULL;
    if (offset >= offset_ && offset + length < length_ + offset_) {
        return data_ + offset - offset_;
    }

    offset_ = offset;
    length_ = buffer_length_;
    if (length_ + offset_ > file_size_) {
        length_ = file_size_ - offset_;
    }
    fseek(fp_, offset, 0);
    length_ = fread(data_, 1, length_, fp_);
    return data_;
}

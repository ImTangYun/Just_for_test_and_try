//
//
//
//
#include <string.h>
#include "write_file_map.h"
#include "log.h"


void WriteFileMap::Flush()
{
    // WLOG(DEBUG, "write from %d to %d", offset_, length_ + offset_);
    fwrite(buffer_, 1, length_, fp_);
    offset_ += length_;
    length_ = 0;
}
char* WriteFileMap::MapToFile(int32_t offset, int32_t length)
{
    return NULL;
}
void WriteFileMap::AppendData(const char* data, int32_t length)
{
    if (capacity_ - length_ <= length) {
        Flush();
    }
    memmove(buffer_ + length_, data, length);
    length_ += length;
}

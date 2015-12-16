//
//
//
//
#ifndef COMMON_H_
#define COMMON_H_
#include "checksum2.h"
#include "checksum3.h"

// #define SUMMER Checksum2
#define SUMMER Checksum3
#define CHUNK_SIZE (1024 * 200 * 1) // 1kb
#define SCAN_BUFFER_SIZE (1024 * 1024 * 20)

struct ChunkInfo
{
    uint32_t weak_sum_;
    // true: src, false: dst
    int32_t from_;
    int32_t offset_;
    int32_t length_;
    string* strong_sum_;
    ~ChunkInfo()
    {
        delete strong_sum_;
    }
};
enum MagicNum
{
    SIG_MAGIC = 0x1234567,
    DELTA_MAGIC = 0x1234568,
};

#endif

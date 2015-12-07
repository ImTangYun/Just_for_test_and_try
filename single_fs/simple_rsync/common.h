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
#define CHUNK_SIZE (1024 * 2 * 1) // 1kb
#define SCAN_BUFFER_SIZE (1024 * 1024 * 20)
enum RequestType
{
    GET_META = 1,
    GET_CHUNK,
    PUT_CHUNK,
    COMMIT
};

enum Response
{
    FILE_DID_NOT_EXIST = -1,
    OK = 1,
};

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
#endif

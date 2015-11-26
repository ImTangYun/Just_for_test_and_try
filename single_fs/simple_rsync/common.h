//
//
//
//
#ifndef COMMON
#define COMMON
#include "checksum2.h"

#define SUMMER Checksum2
#define CHUNK_SIZE (1024 * 1024 * 4) // 1kb
#define SCAN_BUFFER_SIZE (1024 * 1024 * 20)
enum RequestType
{
    GET_META = 1,
    GET_CHUNK,
    PUT,
};

enum Response
{
    FILE_DID_NOT_EXIST = -1,
    OK = 1,
};

struct ChunkInfo
{
    // true: src, false: dst
    bool from_;
    int32_t offset_;
    int32_t length_;
    string* strong_sum_;
};
#endif
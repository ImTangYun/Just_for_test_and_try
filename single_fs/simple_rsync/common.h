//
//
//
//
#ifndef COMMON
#define COMMON

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

#endif

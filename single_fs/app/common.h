//
//
//
//
#ifndef COMMON
#define COMMON

enum RequestType
{
    GET = 1,
    PUT,
    DELETE
};

enum Response
{
    FILE_DID_NOT_EXIST = -1,
    SUCCESS = 1,
    FILE_IS_TOO_BIG = -2,
    OTHER_ERROR = -3
};

#endif

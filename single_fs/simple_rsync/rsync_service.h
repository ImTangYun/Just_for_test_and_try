//
//
//
//
#ifndef RSYNC_SERVICE_H_
#define RSYNC_SERVICE_H_
#include <list>
#include "common.h"
using std::list;
class Basesum;

class RsyncService
{
    public:
        RsyncService():summer_(new SUMMER()){}
        list<ChunkInfo*>* GenerateMetaList(char* path);
        void GetChunk(char* path, char** buf, int offset, int length);
    private:

        Basesum* summer_;
};
#endif

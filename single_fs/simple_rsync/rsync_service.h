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
class ChunkInfo;

class RsyncService
{
    public:
        RsyncService():summer_(new SUMMER()){}
        list<ChunkInfo*>* GenerateMetaList(const char* path);
        void ScanFile(list<ChunkInfo*>* meta, char* dst_file);
        void GetChunk(char* path, char** buf, int offset, int length);
    private:

        Basesum* summer_;
};
#endif

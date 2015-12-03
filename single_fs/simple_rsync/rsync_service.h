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
class FileMap;

class RsyncService
{
    public:
        RsyncService():summer_(new SUMMER()){}
        list<ChunkInfo*>* GenerateMetaList(const char* path);
        void FreeMeta(list<ChunkInfo*>* meta);
        void ScanFile(list<ChunkInfo*>* meta, char* dst_file);
        void ScanFile1(list<ChunkInfo*>* meta, char* dst_file);
        void GetChunk(char* path, char** buf, int offset, int length);
        ~RsyncService()
        {
            delete summer_;
        }
    private:

        FileMap* file_map_;
        Basesum* summer_;
};
#endif

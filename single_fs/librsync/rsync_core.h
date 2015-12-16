//
//
//
//
#ifndef RSYNC_CORE_H_
#define RSYNC_CORE_H_
#include <stdio.h>
#include <stdint.h>
#include <list>
#include "common.h"
using std::list;
#define WRITE_LENGTH_OF_OLD_FILE 4096

class Signature
{
    public:
    private:
};

struct OffsetNode
{
    OffsetNode(bool from, int32_t offset, int32_t length):from_(from),
            offset_(offset), length_(length){}
    bool from_;
    int32_t offset_;
    int32_t length_;
};

class RsyncCore
{
    public:
        int32_t GenerateSigFile(const char* old_file, const char* sig_file);
        int32_t GenerateDeltaFile(const char* new_file, const char* sig_file,
                const char* delta_file);
        int32_t PatchFile(const char* old_file, const char* delta_file,
                const char* result_file);
    private:
        list<ChunkInfo*>* GenerateMetaList(const char* old_file);
        void FreeMeta(list<ChunkInfo*>* meta);
        list<ChunkInfo*>* ParseMetaList(char* buf);
        list<OffsetNode*>* ScanNewFile(list<ChunkInfo*>* meta, char* new_file);
        void FreeOffsetList(list<OffsetNode*>* offset_list);
        void DumpOffsetList(list<OffsetNode*>* list);
};
#endif

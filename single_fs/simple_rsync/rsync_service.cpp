//
//
//
//
#include "rsync_service.h"
#include "file_utils.h"

list<ChunkInfo*>* RsyncService::GenerateMetaList(char* path)
{
    list<ChunkInfo*>* file_meta = new list<ChunkInfo*>();
    int32_t src_size = FileUtils::get_file_size(path);
    int32_t count = src_size / CHUNK_SIZE;
    char* buf = NULL;
    int i = 0;
    int length = CHUNK_SIZE;
    for (; i < count; ++i) {
        FileUtils::read(&buf, i * CHUNK_SIZE, length, path);
        ChunkInfo* chunk_info = new ChunkInfo();
        chunk_info->from_ = true;
        chunk_info->offset_ = i * CHUNK_SIZE;
        chunk_info->length_ = CHUNK_SIZE;
        string* strong_sum = summer_->StrongSum(buf, CHUNK_SIZE);
        chunk_info->strong_sum_ = strong_sum;
        file_meta->push_back(chunk_info);
        delete [] buf;
    }
    
    int32_t last = src_size % CHUNK_SIZE;
    if (last > 0) {
        ChunkInfo* chunk_info = new ChunkInfo();
        chunk_info->from_ = true;
        chunk_info->offset_ = i * CHUNK_SIZE;
        chunk_info->length_ = last;
        file_meta->push_back(chunk_info);
    }
    return file_meta;
}
void RsyncService::GetChunk(char* path, char** buf, int offset, int length)
{
    FileUtils::read(buf, offset, length, path);
}

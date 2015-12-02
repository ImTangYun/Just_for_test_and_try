//
//
//
//
#include <string.h>
#include <unordered_map>
#include "rsync_service.h"
#include "file_utils.h"
#include "common.h"
#include "basesum.h"
#include "checksum2.h"
#include "log.h"
#include "file_map.h"

using std::unordered_map;

list<ChunkInfo*>* RsyncService::GenerateMetaList(const char* path)
{
    list<ChunkInfo*>* file_meta = new list<ChunkInfo*>();
    int32_t src_size = FileUtils::get_file_size(path);
    int32_t count = src_size / CHUNK_SIZE;
    char* buf = NULL;
    int i = 0;
    int length = CHUNK_SIZE;
    for (; i < count; ++i) {
        FileUtils::read(&buf, i * CHUNK_SIZE, length, (char*)path);
        ChunkInfo* chunk_info = new ChunkInfo();
        chunk_info->from_ = true;
        chunk_info->offset_ = i * CHUNK_SIZE;
        chunk_info->length_ = CHUNK_SIZE;
        string* strong_sum = summer_->StrongSum(buf, CHUNK_SIZE);
        chunk_info->strong_sum_ = strong_sum;
        file_meta->push_back(chunk_info);
        chunk_info->weak_sum_ = summer_->Sum1(buf, CHUNK_SIZE);
        delete [] buf;
    }
    
    int32_t last = src_size % CHUNK_SIZE;
    if (last > 0) {
        ChunkInfo* chunk_info = new ChunkInfo();
        chunk_info->weak_sum_ = 0;
        chunk_info->from_ = true;
        chunk_info->offset_ = i * CHUNK_SIZE;
        chunk_info->length_ = last;
        chunk_info->strong_sum_ = summer_->StrongSum(buf, last);
        file_meta->push_back(chunk_info);
    }
    return file_meta;
}
void RsyncService::GetChunk(char* path, char** buf, int offset, int length)
{
    // char* ptr = file_map_->MapFileContent(offset, length);
    // *buf = new char[length];
    // memmove(*buf, ptr, length);
    FileUtils::read(buf, offset, length, path);
}
void RsyncService::ScanFile(list<ChunkInfo*>* meta, char* dst_file)
{
    SUMMER summer;
    unordered_map<uint32_t, ChunkInfo*> sum_offset;
    uint32_t map[4096];
    memset(map, 0, sizeof(map));
    for (auto iter = meta->begin(); iter != meta->end(); ++iter) {
        ChunkInfo* chunk_info = *(iter);
        map[chunk_info->weak_sum_ % 4096] = 1;
        sum_offset[chunk_info->weak_sum_] = chunk_info;
    }
    int32_t dst_size = FileUtils::get_file_size(dst_file);
    int32_t len = CHUNK_SIZE;
    char* buf;
    FileUtils::read(&buf, 0, len, dst_file);
    int32_t curr = CHUNK_SIZE;
    summer.Init(buf, CHUNK_SIZE);
    auto iter = sum_offset.find(summer.tmp_sum());
    int32_t total_left = dst_size - CHUNK_SIZE;
    if (iter != sum_offset.end()) {
        WLOG(INFO, "first chunk is the same");
        string* strong_sum = summer.StrongSum(buf, CHUNK_SIZE);
        if (*strong_sum == *(iter->second->strong_sum_)) {
            iter->second->from_ = false;
            iter->second->offset_ = 0;
        }
        WLOG(INFO, "md5 sum of src chunk %s", iter->second->strong_sum_->c_str());
        WLOG(INFO, "md5 sum of dst chunk %s", strong_sum->c_str());
        WLOG(INFO, "%d is replaced with %d", iter->second->offset_, 0);
    }
    delete [] buf;
    int32_t sum_time = 0;
    while (true) {
        WLOG(DEBUG, "total_left: %d", total_left);
        if (total_left <= 0) break;
        int32_t buf_len = SCAN_BUFFER_SIZE;
        if (buf_len > total_left) {
            buf_len = total_left;
        }
        int32_t read_len = buf_len + CHUNK_SIZE;
        FileUtils::read(&buf, curr - CHUNK_SIZE, read_len, dst_file);
        for (int i = CHUNK_SIZE; i < read_len; ++i) {
            uint32_t new_sum = summer.Update(buf[i]);

            ++curr;
            // a simple and weak hash map
            if (map[new_sum % 4096] != 1) continue;
            auto iter = sum_offset.find(summer.tmp_sum());
            if (iter != sum_offset.end()) {
                ++sum_time;
                string* strong_sum = summer.StrongSum(buf + i - CHUNK_SIZE + 1, CHUNK_SIZE);
                if (*strong_sum == *(iter->second->strong_sum_)) {
                    WLOG(INFO, "md5 sum of src chunk %s", iter->second->strong_sum_->c_str());
                    WLOG(INFO, "md5 sum of dst chunk %s", strong_sum->c_str());
                    WLOG(INFO, "%d is replaced with %d, curr: %d",
                            iter->second->offset_, curr - CHUNK_SIZE, curr);
                    iter->second->from_ = false;
                    iter->second->offset_ = curr - CHUNK_SIZE;
                }
            }
        }   
        delete [] buf;
        total_left -= buf_len;
    }
}

void RsyncService::ScanFile1(list<ChunkInfo*>* meta, char* dst_file)
{
    SUMMER summer;
    unordered_map<uint32_t, ChunkInfo*> sum_offset;
    uint32_t map[4096];
    memset(map, 0, sizeof(map));
    for (auto iter = meta->begin(); iter != meta->end(); ++iter) {
        ChunkInfo* chunk_info = *(iter);
        map[chunk_info->weak_sum_ % 4096] = 1;
        sum_offset[chunk_info->weak_sum_] = chunk_info;
    }


    int32_t dst_size = FileUtils::get_file_size(dst_file);
    FileMap* file_map = new FileMap(dst_file);
    int32_t curr = 0;
    char* buf = NULL;
    while (true) {
        // WLOG(DEBUG, "scanning, curr + CHUNK_SIZE%d, dst_size %d", curr + CHUNK_SIZE, dst_size);
        if (curr + CHUNK_SIZE > dst_size) break;
        buf = file_map->MapFileContent(curr, CHUNK_SIZE);
        summer.Init(buf, CHUNK_SIZE);
        curr += CHUNK_SIZE;
        uint32_t sum1 = summer.tmp_sum();
        if (1 == map[sum1 % 4096]) {
            // judge
            auto iter = sum_offset.find(sum1);
            if (iter != sum_offset.end()) {
                buf = file_map->MapFileContent(curr - CHUNK_SIZE, CHUNK_SIZE);
                string* strong_sum = summer.StrongSum(buf, CHUNK_SIZE);
                if (*strong_sum == *(iter->second->strong_sum_)) {
                    WLOG(INFO, "md5 sum of src chunk %s", iter->second->strong_sum_->c_str());
                    WLOG(INFO, "md5 sum of dst chunk %s", strong_sum->c_str());
                    WLOG(INFO, "%d is replaced with %d, curr: %d",
                            iter->second->offset_, curr - CHUNK_SIZE, curr);
                    iter->second->from_ = false;
                    iter->second->offset_ = curr - CHUNK_SIZE;
                    continue;
                }
            }
        }
        int32_t map_length = dst_size - curr;
        if (map_length > CHUNK_SIZE) {
            map_length = CHUNK_SIZE;
        }
        buf = file_map->MapFileContent(curr, map_length);
        for (int32_t i = 0; i < CHUNK_SIZE; ++i) {
            uint32_t sum1 = summer.Update(buf[i]);
            if (1 == map[sum1 % 4096]) {
                // judge
                auto iter = sum_offset.find(sum1);
                if (iter != sum_offset.end()) {
                    WLOG(INFO, "find equal sum1, offset%d", curr - CHUNK_SIZE);
                    buf = file_map->MapFileContent(curr - CHUNK_SIZE + 1, CHUNK_SIZE);
                    string* strong_sum = summer.StrongSum(buf, CHUNK_SIZE);
                    if (*strong_sum == *(iter->second->strong_sum_)) {
                        WLOG(INFO, "md5 sum of src chunk %s", iter->second->strong_sum_->c_str());
                        WLOG(INFO, "md5 sum of dst chunk %s", strong_sum->c_str());
                        WLOG(INFO, "%d is replaced with %d, curr: %d, i:%d",
                                iter->second->offset_, curr - CHUNK_SIZE, curr, i);
                        iter->second->from_ = false;
                        iter->second->offset_ = curr - CHUNK_SIZE + 1;
                        break;
                    }
                }
            }
            ++curr;
        }
    }
    delete file_map;
}

//
//
//
//
#include <stdio.h>
#include <time.h>
#include <unordered_map>
#include <list>
#include <string>
#include "checksum.h"
#include "file_utils.h"
#include "log.h"

#define CHUNK_SIZE (1024 * 1024 * 4) // 1kb
#define SCAN_BUFFER_SIZE (1024 * 1024 * 20)

using std::unordered_map;
using std::list;
using std::string;

struct ChunkInfo
{
    // true: src, false: dst
    bool from_;
    int32_t offset_;
    int32_t length_;
    string* strong_sum_;
};

void construct_file(char* src_file, char* dst_file,
        list<ChunkInfo*> file_meta, char* cst_file)
{
    FILE* fp = fopen(cst_file, "w");
    char* buf = NULL;
    for (auto iter = file_meta.begin(); iter != file_meta.end(); ++iter) {
        ChunkInfo* chunk_info = *iter;
        if (chunk_info->from_ == true) {
            FileUtils::read(&buf, chunk_info->offset_, chunk_info->length_, src_file);
        } else {
            FileUtils::read(&buf, chunk_info->offset_, chunk_info->length_, dst_file);
        }
        fwrite(buf, 1, chunk_info->length_, fp);
        delete [] buf;
    }
}

void cp4(char* src_file, char* dst_file)
{
    Checksum summer;
    unordered_map<uint32_t, ChunkInfo*> sum_offset;
    list<ChunkInfo*> file_meta;
    int32_t src_size = FileUtils::get_file_size(src_file);
    int32_t count = src_size / CHUNK_SIZE;
    char* buf = NULL;
    int length = CHUNK_SIZE;
    int i = 0;
    for (; i < count; ++i) {
        FileUtils::read(&buf, i * CHUNK_SIZE, length, src_file);
        ChunkInfo* chunk_info = new ChunkInfo();
        chunk_info->from_ = true;
        chunk_info->offset_ = i * CHUNK_SIZE;
        chunk_info->length_ = CHUNK_SIZE;
        string* strong_sum = summer.StrongSum(buf, length);
        chunk_info->strong_sum_ = strong_sum;
        sum_offset[summer.Sum1(buf, length)] = chunk_info;
        file_meta.push_back(chunk_info);
        delete [] buf;
    }
    
    int32_t last = src_size % CHUNK_SIZE;
    if (last > 0) {
        ChunkInfo* chunk_info = new ChunkInfo();
        chunk_info->from_ = true;
        chunk_info->offset_ = i * CHUNK_SIZE;
        chunk_info->length_ = last;
        file_meta.push_back(chunk_info);
    }

    for (auto iter = sum_offset.begin(); iter != sum_offset.end(); ++iter) {
        WLOG(DEBUG, "sum : %u, off: %d len: %d", iter->first,
                iter->second->offset_, iter->second->length_);
    }
    int32_t dst_size = FileUtils::get_file_size(dst_file);
    int32_t len = CHUNK_SIZE;
    FileUtils::read(&buf, 0, len, dst_file);
    int32_t curr = CHUNK_SIZE;
    summer.Init(buf, CHUNK_SIZE);
    auto iter = sum_offset.find(summer.tmp_sum());
    int32_t total_left = dst_size - CHUNK_SIZE;
    if (iter != sum_offset.end()) {
        WLOG(INFO, "%s and %s first chunk is the same", src_file, dst_file);
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
    clock_t start, end;
    start = clock();
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
            auto iter = sum_offset.find(summer.tmp_sum());
            if (iter != sum_offset.end()) {
                ++sum_time;
                string* strong_sum = summer.StrongSum(buf + i - CHUNK_SIZE + 1, CHUNK_SIZE);
                WLOG(INFO, "md5 sum of src chunk %s", iter->second->strong_sum_->c_str());
                WLOG(INFO, "md5 sum of dst chunk %s", strong_sum->c_str());
                WLOG(INFO, "%d is replaced with %d, curr: %d",
                        iter->second->offset_, curr - 1 * CHUNK_SIZE + 1, curr);
                if (*strong_sum == *(iter->second->strong_sum_)) {
                    iter->second->from_ = false;
                    iter->second->offset_ = curr - 1 * CHUNK_SIZE + 1;
                }
            }
            ++curr;
        }
        delete [] buf;
        total_left -= buf_len;
    }
    end = clock();
    WLOG(NOTICE, "find cost %f s, sum time:%d", (double)(end - start) / CLOCKS_PER_SEC, sum_time);

    construct_file(src_file, dst_file, file_meta, "client_data/test.dat");
}

void cp3(char* src_file, char* dst_file)
{
    Checksum summer;
    unordered_map<uint32_t, ChunkInfo*> sum_offset;
    list<ChunkInfo*> file_meta;
    int32_t src_size = FileUtils::get_file_size(src_file);
    int32_t count = src_size / CHUNK_SIZE;
    char* buf = NULL;
    int length = CHUNK_SIZE;
    int i = 0;
    for (; i < count; ++i) {
        FileUtils::read(&buf, i * CHUNK_SIZE, length, src_file);
        ChunkInfo* chunk_info = new ChunkInfo();
        chunk_info->from_ = true;
        chunk_info->offset_ = i * CHUNK_SIZE;
        chunk_info->length_ = CHUNK_SIZE;
        string* strong_sum = summer.StrongSum(buf, length);
        chunk_info->strong_sum_ = strong_sum;
        sum_offset[summer.Sum1(buf, length)] = chunk_info;
        file_meta.push_back(chunk_info);
        delete [] buf;
    }
    
    int32_t last = src_size % CHUNK_SIZE;
    if (last > 0) {
        ChunkInfo* chunk_info = new ChunkInfo();
        chunk_info->from_ = true;
        chunk_info->offset_ = i * CHUNK_SIZE;
        chunk_info->length_ = last;
        file_meta.push_back(chunk_info);
    }

    for (auto iter = sum_offset.begin(); iter != sum_offset.end(); ++iter) {
        WLOG(DEBUG, "sum : %u, off: %d len: %d", iter->first,
                iter->second->offset_, iter->second->length_);
    }
    int32_t dst_size = FileUtils::get_file_size(dst_file);
    int32_t len = CHUNK_SIZE;
    FileUtils::read(&buf, 0, len, dst_file);
    int32_t curr = CHUNK_SIZE;
    summer.Init(buf, CHUNK_SIZE);
    auto iter = sum_offset.find(summer.tmp_sum());
    int32_t total_left = dst_size - CHUNK_SIZE;
    if (iter != sum_offset.end()) {
        WLOG(INFO, "%s and %s first chunk is the same", src_file, dst_file);
        iter->second->from_ = false;
        WLOG(INFO, "%d is replaced with %d", iter->second->offset_, 0);
        iter->second->offset_ = 0;
    }
    WLOG(DEBUG, "before delete");
    delete [] buf;
    WLOG(DEBUG, "test");
    while (true) {
        if (total_left <= 0) break;
        int32_t buf_len = SCAN_BUFFER_SIZE;
        if (buf_len > total_left) {
            buf_len = total_left;
        }
        FileUtils::read(&buf, curr, buf_len, dst_file);
        for (int i = 0; i < buf_len; ++i) {
            uint32_t new_sum = summer.Update(buf[i]);
            auto iter = sum_offset.find(summer.tmp_sum());
            if (iter != sum_offset.end()) {
                iter->second->from_ = false;
                WLOG(INFO, "%d is replaced with %d, curr: %d",
                        iter->second->offset_, curr - 1 * CHUNK_SIZE + 1, curr);
                iter->second->offset_ = curr - 1 * CHUNK_SIZE + 1;
            }
            ++curr;
        }
        delete [] buf;
        total_left -= buf_len;
    }
    construct_file(src_file, dst_file, file_meta, "client_data/test.dat");
}

void cp5(char* src_file, char* dst_file)
{
    Checksum summer;
    unordered_map<uint32_t, ChunkInfo*> sum_offset;
    list<ChunkInfo*> file_meta;
    int32_t src_size = FileUtils::get_file_size(src_file);
    int32_t count = src_size / CHUNK_SIZE;
    char* buf = NULL;
    int length = CHUNK_SIZE;
    int i = 0;
    for (; i < count; ++i) {
        FileUtils::read(&buf, i * CHUNK_SIZE, length, src_file);
        ChunkInfo* chunk_info = new ChunkInfo();
        chunk_info->from_ = true;
        chunk_info->offset_ = i * CHUNK_SIZE;
        chunk_info->length_ = CHUNK_SIZE;
        string* strong_sum = summer.StrongSum(buf, length);
        chunk_info->strong_sum_ = strong_sum;
        sum_offset[summer.Sum1(buf, length)] = chunk_info;
        file_meta.push_back(chunk_info);
        delete [] buf;
    }
    
    int32_t last = src_size % CHUNK_SIZE;
    if (last > 0) {
        ChunkInfo* chunk_info = new ChunkInfo();
        chunk_info->from_ = true;
        chunk_info->offset_ = i * CHUNK_SIZE;
        chunk_info->length_ = last;
        file_meta.push_back(chunk_info);
    }

    for (auto iter = sum_offset.begin(); iter != sum_offset.end(); ++iter) {
        WLOG(DEBUG, "sum : %u, off: %d len: %d", iter->first,
                iter->second->offset_, iter->second->length_);
    }
    int32_t dst_size = FileUtils::get_file_size(dst_file);
    int32_t len = CHUNK_SIZE;
    FileUtils::read(&buf, 0, len, dst_file);
    int32_t curr = CHUNK_SIZE;
    summer.Init(buf, CHUNK_SIZE);
    auto iter = sum_offset.find(summer.tmp_sum());
    int32_t total_left = dst_size - CHUNK_SIZE;
    if (iter != sum_offset.end()) {
        WLOG(INFO, "%s and %s first chunk is the same", src_file, dst_file);
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
    clock_t start, end;
    start = clock();
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
            auto iter = sum_offset.find(summer.tmp_sum());
            if (iter != sum_offset.end()) {
                ++sum_time;
                string* strong_sum = summer.StrongSum(buf + i - CHUNK_SIZE + 1, CHUNK_SIZE);
                WLOG(INFO, "md5 sum of src chunk %s", iter->second->strong_sum_->c_str());
                WLOG(INFO, "md5 sum of dst chunk %s", strong_sum->c_str());
                WLOG(INFO, "%d is replaced with %d, curr: %d",
                        iter->second->offset_, curr - 1 * CHUNK_SIZE + 1, curr);
                if (*strong_sum == *(iter->second->strong_sum_)) {
                    iter->second->from_ = false;
                    iter->second->offset_ = curr - 1 * CHUNK_SIZE + 1;
                }
            }
            ++curr;
        }
        delete [] buf;
        total_left -= buf_len;
    }
    end = clock();
    WLOG(NOTICE, "find cost %f s, sum time:%d", (double)(end - start) / CLOCKS_PER_SEC, sum_time);

    construct_file(src_file, dst_file, file_meta, "client_data/test.dat");
}
int main(int argc, char** argv)
{
    Log::logger_.set_level(INFO);
    Log::logger_.set_level(INFO);
    WLOG(DEBUG, "main");
    clock_t start, finish;
    start = clock();
    cp4(argv[1], argv[2]);
    finish = clock();
    WLOG(NOTICE, "find cost %f",
            (double)(finish - start) / CLOCKS_PER_SEC);

    return 0;
}

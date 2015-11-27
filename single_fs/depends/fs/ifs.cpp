#include <stdint.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <unordered_map>
#include "common.h"
#include "ifs.h"
#include "file_utils.h"

using std::unordered_map;
Ifs::Ifs():latest_data(-1, 0, 0)
{
    meta_datas = new unordered_map<uint64_t, MetaNode>();
}

int32_t Ifs::init()
{
    int32_t file_size = FileUtils::get_file_size(META_STORAGE);
    if (file_size <= 4) {
        MetaNode meta_node(0, 0, 0);
        latest_data = meta_node;
        return -1;
    }
    int32_t fd = ::open(META_STORAGE, O_RDONLY);
    int32_t size;
    MetaNode meta_node;
    ::read(fd, &size, sizeof(size));
    int64_t max_file_id = -1;
    for (int32_t j = 0; j < size; ++j) {
        ::read(fd, &meta_node, sizeof(meta_node));
        (*meta_datas)[meta_node._file_id] = meta_node;
        if ((int32_t)max_file_id < (int32_t)meta_node._file_id) {
            max_file_id = meta_node._file_id;
        }
        printf("max_file_id:%d,file_id: %d\n", max_file_id, meta_node._file_id);
    }
    close(fd);
    if (max_file_id == -1) {
        MetaNode meta_node(0, 0, 0);
        latest_data = meta_node;
    } else {
        latest_data = meta_datas->find(max_file_id)->second;
    }
    printf("file num is: %d max file_id is %d\n", size, max_file_id);
    return 0;
}

int32_t Ifs::write(char* buffer, int64_t length)
{
    int64_t new_offset = latest_data._start + latest_data._length;
    FILE* fp = fopen(DATA_STORAGE, "ab");
    int32_t ret = fseek(fp, new_offset, SEEK_SET);
    if (ret == -1) {
        printf("fseek failed\n");
        return ret;
    }
    ret = fwrite(buffer, 1, length, fp);
    if (ret == -1) {
        printf("write failed\n");
        return ret;
    }
    int64_t file_id = latest_data._file_id + 1;
    MetaNode meta_node(file_id, new_offset, length);
    (*meta_datas)[file_id] = meta_node;
    latest_data = meta_node;
    fclose(fp);
    return file_id;
}

int64_t Ifs::get_length(uint64_t file_id)
{
    unordered_map<uint64_t, MetaNode>::iterator iter
         = meta_datas->find(file_id);
    if (iter != meta_datas->end()) {
        return (iter->second)._length;
    }
    return -1;
}

int64_t Ifs::read(uint64_t file_id, char* buffer, int64_t &length)
{
    
    FILE* fp = fopen(DATA_STORAGE, "r");
    if (fp == NULL) {
        printf("open %s failed\n", DATA_STORAGE);
        return -1;
    }
    unordered_map<uint64_t, MetaNode>::iterator iter = meta_datas->find(file_id);
    if (iter == meta_datas->end()) {
        printf("file did not exist, read failed\n");
        return -1;
    }
    int32_t ret = fseek(fp, (iter->second)._start, SEEK_SET);
    printf("meta_node: %s\n", (iter->second).to_string().c_str());
    if (ret == -2) {
        printf("fseek failed\n");
        return ret;
    }
    ret = fread(buffer, 1, (iter->second)._length, fp);
    printf("read length: %d\n", ret);
    printf("buffer:%s\n", buffer);
    fclose(fp);
    return ret;
}
int32_t Ifs::save_metadata()
{
    unordered_map<uint64_t, MetaNode>::iterator iter = meta_datas->begin();
    int32_t fd = ::open(META_STORAGE, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        printf("open %s failed\n", META_STORAGE);
        return fd;
    }
    int32_t size = meta_datas->size();
    printf("meta_datas size:%d\n", size);
    int32_t ret = ::write(fd, &(size), sizeof(size));
    if (ret == -1) {
        printf("save metadata failed, can not write\n");
        return ret;
    }
    for (; iter != meta_datas->end(); ++iter) {
        int32_t ret = ::write(fd, (void*)(&(iter->second)), sizeof(MetaNode));
    }
    ::close(fd);
    return 0;
}

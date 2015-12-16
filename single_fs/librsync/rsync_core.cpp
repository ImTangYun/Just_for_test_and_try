//
//
//

#include <arpa/inet.h>
#include <unordered_map>
#include "rsync_core.h"
#include "file_utils.h"
#include "write_file_map.h"
#include "file_map.h"

using std::unordered_map;

int32_t RsyncCore::GenerateSigFile(const char* old_file, const char* sig_file)
{
    WLOG(DEBUG, "generating sig");
    list<ChunkInfo*>* sig_list = GenerateMetaList(old_file);
    WriteFileMap* writer = new WriteFileMap(sig_file);

    uint32_t magic = htonl(uint32_t(SIG_MAGIC));
    writer->AppendData((char*)(&magic), sizeof(magic));

    int32_t chunk_size = htonl(uint32_t(CHUNK_SIZE));
    writer->AppendData((char*)(&chunk_size), sizeof(int32_t));

    uint32_t sig_list_size = htonl(sig_list->size());
    writer->AppendData((char*)(&sig_list_size), sizeof(sig_list_size));
    for (auto iter = sig_list->begin(); iter != sig_list->end(); ++iter) {
        ChunkInfo* chunk_info = *iter;

        char from = 0xff;
        if (chunk_info->from_ == false)
            from = 0x00;
        writer->AppendData(&from, 1);

        uint32_t tmp = htonl(chunk_info->weak_sum_);
        writer->AppendData((char*)(&tmp), sizeof(tmp));

        tmp = htonl(chunk_info->offset_);
        writer->AppendData((char*)(&tmp), sizeof(tmp));

        tmp = htonl(chunk_info->length_);
        writer->AppendData((char*)(&tmp), sizeof(tmp));

        writer->AppendData(chunk_info->strong_sum_->c_str(),
                chunk_info->strong_sum_->length());
    }
    WLOG(DEBUG, "generated sig OK");
    delete writer;
    FreeMeta(sig_list);
    return 0;
}
int32_t RsyncCore::GenerateDeltaFile(const char* new_file,
        const char* sig_file, const char* delta_file)
{
    WLOG(DEBUG, "generating delta file");
    int32_t sig_size = FileUtils::get_file_size(sig_file);
    char* sig_buf = NULL;
    FileUtils::read(&sig_buf, 0, sig_size, (char*)sig_file);
    if (sig_buf == NULL) {
        WLOG(ERROR, "read sig file %s failed", sig_file);
        return -1;
    }
    list<ChunkInfo*>* sig_list = ParseMetaList(sig_buf);
    list<OffsetNode*>* offset_list = ScanNewFile(sig_list, (char*)new_file);
    FreeMeta(sig_list);
    int32_t new_file_size = FileUtils::get_file_size((char*)new_file);
    if (new_file_size < CHUNK_SIZE) {
        return 0;
    }
    FileMap* reader = new FileMap((char*)new_file);
    WriteFileMap* writer = new WriteFileMap(delta_file);

    uint32_t magic = htonl((uint32_t)DELTA_MAGIC);
    writer->AppendData((char*)(&magic), sizeof(magic));

    uint32_t count = htonl((uint32_t)(offset_list->size()));
    writer->AppendData((char*)(&count), sizeof(count));
    for (auto iter = offset_list->begin(); iter != offset_list->end(); ++iter) {
        OffsetNode* offset_node = *iter;
        if (offset_node->from_ == true) {
            char from = 0x00;
            writer->AppendData(&from, 1);
            uint32_t length = htonl(offset_node->length_);
            writer->AppendData((char*)(&length), sizeof(length));
            char* data = reader->MapFileContent(offset_node->offset_,
                    offset_node->length_);
            writer->AppendData(data, offset_node->length_);
        } else {
            char from = 0xff;
            writer->AppendData(&from, 1);
            
            uint32_t offset = htonl(offset_node->offset_);
            writer->AppendData((char*)(&offset), sizeof(offset));

            uint32_t length = htonl(offset_node->length_);
            writer->AppendData((char*)(&length), sizeof(length));
        }
    }
    FreeOffsetList(offset_list);
    delete reader;
    delete writer;
    WLOG(DEBUG, "generated delta file ok");
}
int32_t RsyncCore::PatchFile(const char* old_file, const char* delta_file,
        const char* result_file)
{

    WLOG(INFO, "patching file");
    FileMap* delta_file_reader = new FileMap((char*)delta_file);

    int32_t offset = 0;
    char* delta_buf = delta_file_reader->MapFileContent(offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    MagicNum magic = (MagicNum)ntohl(*(uint32_t*)delta_buf);
    if (magic != DELTA_MAGIC) {
        WLOG(ERROR, "wrong delta file magic num, %s may not a delta file", delta_file);
        delete delta_file_reader;
        return -1;
    }

    delta_buf = delta_file_reader->MapFileContent(offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    int32_t count = htonl(*(uint32_t*)delta_buf);

    FileMap* old_file_reader = new FileMap((char*)old_file);
    WLOG(INFO, "old file %s", old_file);
    WriteFileMap* writer = new WriteFileMap(result_file);

    char* old_file_buf;
    int32_t old_file_offset = 0;
    for (int32_t i = 0; i < count; ++i) {
        delta_buf = delta_file_reader->MapFileContent(offset, sizeof(char));
        offset += sizeof(char);

        // from old file
        if (char(0xff) == *delta_buf) {
            delta_buf = delta_file_reader->MapFileContent(offset, sizeof(uint32_t));
            offset += sizeof(uint32_t);
            int32_t data_offset = ntohl(*(uint32_t*)delta_buf);

            delta_buf = delta_file_reader->MapFileContent(offset, sizeof(uint32_t));
            offset += sizeof(uint32_t);
            int32_t data_length = ntohl(*(uint32_t*)delta_buf);

            // WLOG(DEBUG, "old,reoff %d, relen %d, offset %d, length %d",
            //         writer->offset(), writer->length(), data_offset, data_length);
            
            int32_t write_count = data_length / WRITE_LENGTH_OF_OLD_FILE;
            for (int32_t j = 0; j < write_count; ++j) {
                old_file_buf = old_file_reader->MapFileContent(data_offset,
                        WRITE_LENGTH_OF_OLD_FILE);
                data_offset += WRITE_LENGTH_OF_OLD_FILE;

                writer->AppendData(old_file_buf, WRITE_LENGTH_OF_OLD_FILE);
            }

            int32_t last = data_length % WRITE_LENGTH_OF_OLD_FILE;
            if (last > 0) {
                
                old_file_buf = old_file_reader->MapFileContent(data_offset,
                        last);
                data_offset += last;

                writer->AppendData(old_file_buf, last);
            }

        // from delta file
        } else if (char(0x00) == *delta_buf) {

            delta_buf = delta_file_reader->MapFileContent(offset, sizeof(uint32_t));
            offset += sizeof(uint32_t);
            int32_t data_length = ntohl(*(uint32_t*)delta_buf);

            // WLOG(DEBUG, "delta,reoff %d, relen %d, length %d",
            //         writer->offset(), writer->length(), data_length);

            int32_t write_count = data_length / WRITE_LENGTH_OF_OLD_FILE;
            for (int32_t j = 0; j < write_count; ++j) {
                delta_buf = delta_file_reader->MapFileContent(offset,
                        WRITE_LENGTH_OF_OLD_FILE);
                offset += WRITE_LENGTH_OF_OLD_FILE;

                writer->AppendData(delta_buf, WRITE_LENGTH_OF_OLD_FILE);
            }

            int32_t last = data_length % WRITE_LENGTH_OF_OLD_FILE;
            if (last > 0) {
                delta_buf = delta_file_reader->MapFileContent(offset, last);
                offset += last;

                writer->AppendData(delta_buf, last);
            }

        } else {
            WLOG(ERROR, "wrong format of delta file: %s, from: %x ?",
                    delta_file, *delta_buf);
            delete old_file_reader;
            delete delta_file_reader;
            delete writer;
            return -1;
        }
    }

    // writer->Flush();
    delete old_file_reader;
    delete delta_file_reader;
    WLOG(INFO, "patched file ok");
    delete writer;
}

list<ChunkInfo*>* RsyncCore::GenerateMetaList(const char* old_file)
{
    list<ChunkInfo*>* file_meta = new list<ChunkInfo*>();
    int32_t src_size = FileUtils::get_file_size(old_file);
    int32_t count = src_size / CHUNK_SIZE;
    char* buf = NULL;
    int i = 0;
    int length = CHUNK_SIZE;
    SUMMER* summer = new SUMMER();
    FileMap* file_map = new FileMap((char*)old_file);
    for (; i < count; ++i) {
        buf = file_map->MapFileContent(i * CHUNK_SIZE, length);
        ChunkInfo* chunk_info = new ChunkInfo();
        chunk_info->from_ = true;
        chunk_info->offset_ = i * CHUNK_SIZE;
        chunk_info->length_ = CHUNK_SIZE;
        string* strong_sum = summer->StrongSum(buf, CHUNK_SIZE);
        chunk_info->strong_sum_ = strong_sum;
        file_meta->push_back(chunk_info);
        chunk_info->weak_sum_ = summer->Sum1(buf, CHUNK_SIZE);
    }

    int32_t last = src_size % CHUNK_SIZE;
    if (last > 0 && false) {
        ChunkInfo* chunk_info = new ChunkInfo();
        chunk_info->weak_sum_ = 0;
        chunk_info->from_ = true;
        chunk_info->offset_ = i * CHUNK_SIZE;
        chunk_info->length_ = last;
        chunk_info->strong_sum_ = summer->StrongSum(buf, last);
        file_meta->push_back(chunk_info);
    }
    delete file_map;
    return file_meta;
}
void RsyncCore::FreeMeta(list<ChunkInfo*>* meta)
{
    for (auto iter = meta->begin(); iter != meta->end(); ++iter) {
        delete *(iter);
    }
    delete meta;
}

list<ChunkInfo*>* RsyncCore::ParseMetaList(char* buf)
{
    list<ChunkInfo*>* meta = new list<ChunkInfo*>();
    MagicNum magic = (MagicNum)ntohl(*(uint32_t*)buf);
    buf += sizeof(uint32_t);
    if (magic != SIG_MAGIC) {
        WLOG(ERROR, "wrong magic number, not a sig file");
        delete meta;
        return NULL;
    }

    int32_t chunk_size = ntohl(*(uint32_t*)buf);
    buf += sizeof(uint32_t);

    int32_t list_size = ntohl(*(uint32_t*)buf);
    buf += sizeof(uint32_t);

    for (int32_t i = 0; i < list_size; ++i) {
        ChunkInfo* chunk_info = new ChunkInfo();

        chunk_info->from_ = true;
        if (*buf == 0x00) chunk_info->from_ = false;
        ++buf;

        chunk_info->weak_sum_ = ntohl(*(uint32_t*)buf);
        buf += sizeof(uint32_t);

        chunk_info->offset_ = ntohl(*(uint32_t*)buf);
        buf += sizeof(uint32_t);

        chunk_info->length_ = ntohl(*(uint32_t*)buf);
        buf += sizeof(uint32_t);

        chunk_info->strong_sum_ = new string(buf, 0, 32);
        buf += 32;
        meta->push_back(chunk_info);
    }
    return meta;
}

list<OffsetNode*>* RsyncCore::ScanNewFile(list<ChunkInfo*>* meta, char* new_file)
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


    int32_t dst_size = FileUtils::get_file_size(new_file);
    FileMap* file_map = new FileMap(new_file);
    int32_t curr = 0;
    char* buf = NULL;

    list<OffsetNode*>* offset_list = new list<OffsetNode*>;
    int32_t offset = 0;
    while (true) {
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
                    iter->second->from_ = false;
                    int32_t tmp_offset = curr - CHUNK_SIZE;
                    if (offset != tmp_offset) {
                        WLOG(DEBUG, "off %d, tmp_off %d", offset, tmp_offset);
                        OffsetNode* offset_node = new OffsetNode(true, offset, tmp_offset - offset);
                        offset_list->push_back(offset_node);
                    }
                    OffsetNode* offset_node = new OffsetNode(false, iter->second->offset_, CHUNK_SIZE);
                    offset_list->push_back(offset_node);
                    offset = tmp_offset + CHUNK_SIZE;
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
                    buf = file_map->MapFileContent(curr - CHUNK_SIZE + 1, CHUNK_SIZE);
                    string* strong_sum = summer.StrongSum(buf, CHUNK_SIZE);
                    if (*strong_sum == *(iter->second->strong_sum_)) {
                        iter->second->from_ = false;
                        int32_t tmp_offset = curr - CHUNK_SIZE + 1;
                        if (offset != tmp_offset) {
                            OffsetNode* offset_node = new OffsetNode(true, offset, tmp_offset - offset);
                            offset_list->push_back(offset_node);
                        }
                        OffsetNode* offset_node = new OffsetNode(false, iter->second->offset_, CHUNK_SIZE);
                        offset_list->push_back(offset_node);
                        offset = tmp_offset + CHUNK_SIZE;
                        break;
                    }
                }
            }
            ++curr;
        }
    }
    if (offset < dst_size) {
        OffsetNode* offset_node = new OffsetNode(true, offset, dst_size - offset);
        offset_list->push_back(offset_node);
    }
    delete file_map;
    return offset_list;
}

void RsyncCore::FreeOffsetList(list<OffsetNode*>* offset_list)
{
    for (auto iter = offset_list->begin(); iter != offset_list->end(); ++iter) {
        delete *iter;
    }
    delete offset_list;
}
void RsyncCore::DumpOffsetList(list<OffsetNode*>* list)
{
    for (auto iter = list->begin(); iter != list->end(); ++iter) {
        OffsetNode* offset_node = *iter;
        WLOG(INFO, "from:%d offset:%d length:%d", offset_node->from_,
                offset_node->offset_, offset_node->length_);
    }
}

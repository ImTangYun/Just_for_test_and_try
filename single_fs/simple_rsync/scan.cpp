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
#include "checksum1.h"
#include "checksum2.h"
#include "checksum3.h"
#include "file_utils.h"
#include "common.h"
#include "log.h"
#include "time_utils.h"
#include "file_map.h"

#define CHUNK_SIZE (1024 * 1024 * 1) // 1kb
#define SCAN_BUFFER_SIZE (1024 * 1024 * 20)

using std::unordered_map;
using std::list;
using std::string;

void scan(char* file_name) {
    SUMMER summer;
    int32_t dst_size = FileUtils::get_file_size(file_name);
    int32_t len = CHUNK_SIZE;
    char* buf;
    FileMap* file_map = new FileMap(file_name);
    buf = file_map->MapFileContent(0, len);
    int32_t curr = CHUNK_SIZE;
    summer.Init(buf, CHUNK_SIZE);
    int32_t total_left = dst_size - CHUNK_SIZE;
    while (true) {
        if (total_left <= 0) break;
        int32_t buf_len = CHUNK_SIZE;
        if (buf_len > total_left) {
            buf_len = total_left;
        }
        buf = file_map->MapFileContent(curr, buf_len);
        for (int i = 0; i < buf_len; ++i) {
            uint32_t new_sum = summer.Update(buf[i]);
            ++curr;
        }
        total_left -= buf_len;
    }
    delete file_map;
}
 
int main(int argc, char** argv)
{
    TimeCounter time_counter;
    scan(argv[1]);
    time_counter.AddNow();
    WLOG(DEBUG, "scan file %s cost %.02f", argv[1], time_counter.GetTimeCosts(1));
    return 0;
}

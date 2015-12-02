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

#define CHUNK_SIZE (1024 * 1024 * 4) // 1kb
#define SCAN_BUFFER_SIZE (1024 * 1024 * 20)

using std::unordered_map;
using std::list;
using std::string;

void scan(char* file_name) {
    SUMMER summer;
    int32_t dst_size = FileUtils::get_file_size(file_name);
    int32_t len = CHUNK_SIZE;
    char* buf;
    FileUtils::read(&buf, 0, len, file_name);
    int32_t curr = CHUNK_SIZE;
    summer.Init(buf, CHUNK_SIZE);
    int32_t total_left = dst_size - CHUNK_SIZE;
    WLOG(DEBUG, "before delete");
    delete [] buf;
    WLOG(DEBUG, "test");
    while (true) {
        if (total_left <= 0) break;
        int32_t buf_len = SCAN_BUFFER_SIZE;
        if (buf_len > total_left) {
            buf_len = total_left;
        }
        FileUtils::read(&buf, curr, buf_len, file_name);
        for (int i = 0; i < buf_len; ++i) {
            uint32_t new_sum = summer.Update(buf[i]);
            ++curr;
        }
        delete [] buf;
        total_left -= buf_len;
    }
}
 
int main(int argc, char** argv)
{
    TimeCounter time_counter;
    scan(argv[1]);
    time_counter.AddNow();
    WLOG(DEBUG, "scan file %s cost %.02f", argv[1], time_counter.GetTimeCosts(1));
    return 0;
}

#include <stdio.h>
#include "file_map.h"
#include "write_file_map.h"
#include "log.h"
#include "file_utils.h"

#define COPY_LENGTH 4096

void copy_part_of_file(char* src, int offset, int length, char* dst)
{
    int32_t file_size = FileUtils::get_file_size(src);
    if (offset > file_size) {
        WLOG(ERROR, "the copying content offset is out of file size");
        return;
    }
    if (length + offset > file_size) {
        length = file_size - offset;
        WLOG(WARN, "the content you want to copy is out of file, only copy content in file");
    }
    FileMap* reader = new FileMap(src);
    WriteFileMap* writer = new WriteFileMap(dst);
    int count = length / COPY_LENGTH;
    char* data = NULL;
    
    for (int32_t i = 0; i < count; ++i) {
        data = reader->MapFileContent(offset, COPY_LENGTH);
        offset += COPY_LENGTH;
        writer->AppendData(data, COPY_LENGTH);
    }
    int32_t last = length % COPY_LENGTH;
    if (last > 0) {
        data = reader->MapFileContent(offset, last);
        offset += last;
        writer->AppendData(data, last);
    }
    delete reader;
    delete writer;
}

void do_command(int argc, char** argv)
{
    if (argc < 6) {
        WLOG(ERROR, "wrong parameter");
        WLOG(INFO, "Usage: command [-option] <src_file> <offset> <length> <dst_file>");
        return;
    }
    copy_part_of_file(argv[2], atoi(argv[3]), atoi(argv[4]), argv[5]);
}

int main(int argc, char** argv)
{
    do_command(argc, argv);
    return 0;
}

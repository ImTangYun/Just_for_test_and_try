#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "client.h"
#include "file_utils.h"


void test_init_save()
{
    Client client;
    unordered_map<uint64_t, MetaNode>* meta_datas = client.get_meta_datas();
    for (uint64_t i = 0; i < 10; ++i) {
        MetaNode metanode(i, (int64_t)i * 10, 10);
        (*meta_datas)[i] = metanode;
    }
    printf("meta data: %s\n", client.to_string().c_str());
    client.save_metadata();
    Client client1;
    client1.init();
    printf("meta data: %s\n", client1.to_string().c_str());
    printf("hello test\n");
}

void write(char* buf, int64_t length, Client &client)
{
    uint64_t file_id = client.write(buf, length);
    printf("write file_id %d\n", file_id);
}

uint64_t write(const char* file_name, Client &client)
{
    if (file_name == NULL) {
        printf("file name is null");
        return -1;
    }
    int64_t flength = (int64_t)FileUtils::get_file_size(file_name);
    if (flength <= 0) {
        printf("file %s did not exist!, file length: %d", file_name, flength);
        return -1;
    }
    FILE* fp = fopen(file_name, "rb");
    char* buff = new char[flength + 1];
    fseek(fp, 0L, SEEK_SET);
    fread(buff, 1, flength, fp);
    buff[flength] = '\n';
    printf("test.dat:%s\n", buff);
    uint64_t file_id = client.write(buff, flength);
    delete [] buff;
    printf("file %s is wrote as file_id %d\n", file_name, file_id);
    return file_id;
}

void write_file_test()
{
    Client client;
    client.init();
    printf("meta data: %s\n", client.to_string().c_str());
    uint64_t file_id = write("test.dat", client);
    int64_t file_length = client.get_length(file_id);
    char *buf = new char[file_length];
    int64_t ret = client.read(file_id, buf, file_length);
    buf[file_length] = '\0';
    printf("file_id:%d,file_length:%d, data:%s\n", (int32_t)file_id,
            (int32_t)file_length, buf);
    client.save_metadata();
}

int32_t main(int argc, char** argv)
{
    Client client;
    client.init();
    printf("meta data: %s\n", client.to_string().c_str());
    uint64_t file_id = write("test/main", client);
    int64_t file_length = client.get_length(12);
    char *buf = new char[file_length];
    int64_t ret = client.read(12, buf, file_length);
    FileUtils::write(buf, file_length, "test/main.bak");
    client.save_metadata();
    delete [] buf;
    return 0;
}



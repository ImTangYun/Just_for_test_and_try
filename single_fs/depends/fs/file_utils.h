#ifndef FILE_UTILS_H_
#define FILE_UTILS_H_
#include <stdint.h>

class FileUtils
{
    public:
        static int32_t get_file_size(const char* file_name);

        static bool is_exists_file(const char* file_name);

        static int32_t write(char* buff, int64_t length, char* file_name);

        static int32_t read(char** buff, int64_t &length, char* file_name);

        static int32_t write(char* buff, int length, char* file_name, bool cover = false);

        static int32_t read(char** buff, int &length, char* file_name, int reserve_size = 0);
};


#endif

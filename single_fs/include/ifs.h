#ifndef _IFS_H_
#define _IFS_H_
#include <stdint.h>
#include <unordered_map>
#include <string>

using std::unordered_map;
using std::string;
struct MetaNode
{
    uint64_t _file_id;
    int64_t _start;
    int64_t _length;
    MetaNode()
    {
    
    }
    MetaNode(uint64_t file_id, int64_t start, int64_t length)
    {
        _file_id = file_id;
        _start = start;
        _length = length;
    }
    string to_string()
    {
        char buffer[100];
        snprintf(buffer, 100, "[file_id:%d, start:%d, length:%d]",
                _file_id, _start, _length);
        return buffer;
    }
};
class Ifs
{
    public:
        Ifs();

        int32_t init();

        int32_t write(char* buffer, int64_t length);

        int64_t get_length(uint64_t file_id);

        int64_t read(uint64_t file_id, char* buffer, int64_t &length);
        int32_t save_metadata();
        
        // for test
        unordered_map<uint64_t, MetaNode>* get_meta_datas()
        {
            return meta_datas;
        }

        string to_string()
        {
            string ret;
            ret += "{";
            char buffer[100];
            unordered_map<uint64_t, MetaNode>::iterator iter = meta_datas->begin();
            for (; iter != meta_datas->end(); ++iter) {
                snprintf(buffer, 100, "file_id:%d|start:%d|length:%d,",
                        (int32_t)iter->second._file_id,
                        (int32_t)iter->second._start, (int32_t)iter->second._length);
                ret += buffer;
            }
            ret += "}";
            return ret;
        }

    private:
        unordered_map<uint64_t, MetaNode> *meta_datas;
        MetaNode latest_data;
};

#endif

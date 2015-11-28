//
//
//
//
#ifndef CLIENT_H_
#define CLIENT_H_

#include <stdint.h>
#include <string>
#include <list>

using std::string;
using std::list;
class ChunkInfo;
class EndPoint;
class NetHandler;
class NetMachine;
class StreamSocketContext;
class ServerProcessor;
class RsyncService;
class Client
{
    public:
        Client(string ip_port):ip_port_(ip_port){}
        int32_t GetFile(char* src_path, char* dst_path, char* tmp_path);
        int32_t PutFile(char* src_path, char* dst_path, char* tmp_path);
        int32_t Init();
        ~Client();

    private:
        list<ChunkInfo*>* GetMeta(char* path);
        list<ChunkInfo*>* ParseMeta(char* buf, int32_t node_num);
        int32_t GetChunk(char** buf, int32_t offset, int32_t length, char* path);
        int32_t PutChunk(char* buf, int32_t offset, int32_t length);
        int32_t CommitFile();
        void ConstructFile(char* src_file, char* dst_file,
                list<ChunkInfo*>* file_meta, char* cst_file);

    private:
        string ip_port_;
        EndPoint* end_point_;
        NetMachine* net_machine_;
        NetHandler* net_handler_;
        StreamSocketContext* socket_context_;
        RsyncService* rsync_service_;
};

#endif

//
//
//
//
#ifndef SERVER_PROCESSOR_H_
#define SERVER_PROCESSOR_H_
#include <stdint.h>
#include <vector>
#include "cthread.h"

class TaskNode;
namespace myspace
{
template<typename T>
class TaskQueue;
};
class RsyncService;
using std::vector;
using namespace cthread;
class ServerProcessor: public Runnuble
{
    public:
        ServerProcessor():runnuing_(false){}
        virtual void Run(CThread* cthread, void* args);
        void Process(int32_t thread_id);
        void Init(int32_t thread_num, myspace::TaskQueue<TaskNode*>* task_queue);
        void Start();
        void Stop();

    private:
        void GetMeta(TaskNode* node);
        void GetChunk(TaskNode* node);

        vector<CThread*> threads_;
        bool runnuing_;
        myspace::TaskQueue<TaskNode*>* task_queue_;
        int32_t thread_num_;
        RsyncService* rsync_service_;
};
#endif

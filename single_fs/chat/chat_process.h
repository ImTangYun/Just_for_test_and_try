//
//
//
#ifndef _CHAT_PROCESS_H_
#define _CHAT_PROCESS_H_
#include "cthread.h"
class TaskNode;
class UserConnection;
namespace myspace
{
template<typename T>
    class TaskQueue;
};
using namespace cthread;
class ChatProcess: public Runnuble
{
    public:
        ChatProcess():runnuing_(false){}
        virtual void Run(CThread* cthread, void* args);
        void Process(int32_t thread_id);
        void Init(myspace::TaskQueue<TaskNode*>* task_queue,
                UserConnection* user_connection);
        void Start();
        void Stop();

    private:
        void SendMessage(TaskNode* node);
        void Login(TaskNode* node);
        void SendToOne(const string& user, Packet* packet);
    private:
        bool runnuing_;
        myspace::TaskQueue<TaskNode*> *task_queue_;
        CThread* thread_;
        UserConnection* user_connection_;
};
#endif

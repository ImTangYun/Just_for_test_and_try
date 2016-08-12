//
//
//
#ifndef COMMUNICATE_LOOP
#define COMMUNICATE_LOOP
#include <sys/epoll.h>
#include "cthread.h"
#include "socket_context.h"
#include "simple_list.h"
using namespace cthread;
class CommunicateLoop: public Runnuble
{

    public:
        virtual void Run(CThread* cthread, void* args);
        CommunicateLoop():efd_(epoll_create(256)) {}
        void Process(int thread_id);
        void Init();
        void Start();
        void Stop();
        int AddEvent(SocketContext* socket_context,
                bool readable, bool writable);
        int SetEvent(SocketContext* socket_context, bool writable, bool readable);
        int ClearEvent(SocketContext* socket_context);
        void HandleEvent();
        virtual ~CommunicateLoop()
        {
            Stop();
            delete event_loop_thread_;
            event_loop_thread_ = NULL;
            IteratorFun<SocketContext*>* fun = new IteratorDel(this);
            context_list_.HandleEveryNode(fun);
            delete fun;
        }
        int efd() {return efd_;}
    private:
        SimpleList<SocketContext*> context_list_;
        bool is_run_;
        CThread *event_loop_thread_;
        int efd_;
        class IteratorDel: public IteratorFun<SocketContext*>
        {
                CommunicateLoop* communicate_loop_;
            public:
                IteratorDel(CommunicateLoop* communicate_loop):communicate_loop_(communicate_loop) {}
                virtual void action(Node<SocketContext*>* node)
                {
                    printf("deleting socket_context\n");
                    epoll_ctl(communicate_loop_->efd(), EPOLL_CTL_DEL, node->data_->fd(), NULL);
                    close(node->data_->fd());
                    delete node->data_;
                    node->data_ = NULL;
                }
        };
};

#endif

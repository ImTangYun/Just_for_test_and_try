#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include "taskqueue.h"
using myspace::TaskNode;
using myspace::TaskQueue;
TaskQueue<TaskNode> q(100);
int i = 0;
void* pfunc_generate_task(void* arg)
{
    while (true) {
        ++i;
        char* p = new char[100];
        snprintf(p, 100, "this is data %d\n", i);
        TaskNode tmp;
        tmp.data_ = (void*)p;
        tmp.length_ = 100;
        q.Push(tmp);
        printf("pushed: %d\n", i);
        usleep(1);
    } 
    return (void*)NULL;
}
void* pfunc_process_task(void* arg)
{
    while (true) {
        printf("poped: %s", (char*)q.Pop().data_);
        usleep(200000);
    } 
    return (void*)NULL;
}
int main()
{
    for (int i = 0; i < 1; ++i) {
        char* p = new char[100];
        snprintf(p, 100, "this is data %d\n", i);
        TaskNode tmp;
        tmp.data_ = (void*)p;
        tmp.length_ = 100;
        q.Push(tmp);
    }
    pthread_t pid1;
    pthread_create(&pid1, NULL, pfunc_generate_task, NULL);
    assert(pid1 > 0);
    pthread_t pid2;
    pthread_create(&pid2, NULL, pfunc_process_task, NULL);
    assert(pid2 > 0);
    pthread_t pid3;
    pthread_create(&pid3, NULL, pfunc_process_task, NULL);
    assert(pid3 > 0);
    while (true);
    return 0;
}

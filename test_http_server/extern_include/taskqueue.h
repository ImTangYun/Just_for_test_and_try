#ifndef _TASK_QUEUE_H_
#define _TASK_QUEUE_H_
#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <queue>
#include <unistd.h>

using std::queue;
namespace myspace
{
struct TaskNode
{
    void* data_;
    int length_; // byte length of data_;
};
template<typename T>
class TaskQueue
{
public:
    TaskQueue();
    TaskQueue(uint32_t limit);
    ~TaskQueue();
    void Push(T node);
    T Pop();
    bool Empty();
private:
    queue<T> data_queue_;
    pthread_mutex_t lock_;
    uint32_t limit_;
};

template<typename T>
TaskQueue<T>::TaskQueue():lock_(PTHREAD_MUTEX_INITIALIZER),limit_(10)
{
}
template<typename T>
TaskQueue<T>::TaskQueue(uint32_t limit):lock_(PTHREAD_MUTEX_INITIALIZER),limit_(limit)
{
}
template<typename T>
TaskQueue<T>::~TaskQueue()
{
    pthread_mutex_lock(&lock_);
    while (!Empty()) {
        char* p = (char*)Pop().data_;
        if (p != NULL) {
            delete p;
        }
    }
    pthread_mutex_destroy(&lock_);
}
template<typename T>
void TaskQueue<T>::Push(T node)
{
    bool success = false;
    while (!success) {
        pthread_mutex_lock(&lock_);
        if (data_queue_.size() < limit_) {
            data_queue_.push(node);
            success = true;
        }
        pthread_mutex_unlock(&lock_);
        if (!success) usleep(1000);
    }
}
template<typename T>
T TaskQueue<T>::Pop()
{
    bool success = false;
    printf("taskqueue size: %lu\n", data_queue_.size());
    while (true) {
        pthread_mutex_lock(&lock_);
        T tmp; 
        if (!Empty()) {
            tmp = data_queue_.front();
            data_queue_.pop();
            success = true;
        }
        pthread_mutex_unlock(&lock_);
        if (!success) usleep(1000);
        else return tmp;
    }
}
template<typename T>
bool TaskQueue<T>::Empty()
{
    return data_queue_.size() == 0;
}
} // namespace myspace

#endif // _TASK_QUEUE_H_

#ifndef _TASK_QUEUE_H_
#define _TASK_QUEUE_H_
#include <pthread.h>
#include <stdint.h>
#include <queue>
#include <unistd.h>

using std::queue;
namespace myspace
{
template<typename T>
class TaskQueue
{
public:
    TaskQueue();
    TaskQueue(uint32_t limit);
    ~TaskQueue(){}
    void Push(T node);
    T Pop();
    T Pop(int timeout);
    bool Empty();
private:
    queue<T> data_queue_;
    pthread_mutex_t lock_;
    uint32_t limit_;
};

template<typename T>
TaskQueue<T>::TaskQueue():limit_(10)
{
    pthread_mutex_init (&lock_, NULL);
}
template<typename T>
TaskQueue<T>::TaskQueue(uint32_t limit):limit_(limit)
{
    pthread_mutex_init (&lock_, NULL);
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
        if (!success) sleep(1);
    }
}
template<typename T>
T TaskQueue<T>::Pop(int timeout)
{
    bool success = false;
    int time_cost = 0;
    while (true) {
        pthread_mutex_lock(&lock_);
        T tmp; 
        if (!Empty()) {
            tmp = data_queue_.front();
            data_queue_.pop();
            success = true;
        }
        pthread_mutex_unlock(&lock_);
        if (!success) {
            usleep(10);
            time_cost += 10;
            if (time_cost >= timeout) {
                return (T)0;
            }
        }
        else return tmp;
    }
}
template<typename T>
T TaskQueue<T>::Pop()
{
    bool success = false;
    while (true) {
        pthread_mutex_lock(&lock_);
        T tmp; 
        if (!Empty()) {
            tmp = data_queue_.front();
            data_queue_.pop();
            success = true;
        }
        pthread_mutex_unlock(&lock_);
        if (!success) {
            usleep(100);
        }
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

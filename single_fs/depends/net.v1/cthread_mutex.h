//
//
//
//
#ifndef CTHREAD_MUTEX
#define CTHREAD_MUTEX
#include <pthread.h>
#include <stdio.h>

class CthreadMutex
{
    public:
        CthreadMutex();
        ~CthreadMutex();
        void Lock();
        void Unlock();
    private:
        pthread_mutex_t lock_;
};

#endif

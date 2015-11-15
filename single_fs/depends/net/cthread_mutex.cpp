//
//
//
//
#include "cthread_mutex.h"

CthreadMutex::CthreadMutex()
{
    pthread_mutex_init(&lock_, NULL);
}
CthreadMutex::~CthreadMutex()
{
    pthread_mutex_destroy(&lock_);
}
void CthreadMutex::Lock()
{
    printf("lock");
    pthread_mutex_lock(&lock_);
}
void CthreadMutex::Unlock()
{
    pthread_mutex_unlock(&lock_);
}

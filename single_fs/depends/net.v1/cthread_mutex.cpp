//
//
//
//
#include "cthread_mutex.h"
#include "log.h"

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
    WLOG(DEBUG, "lock");
    pthread_mutex_lock(&lock_);
}
void CthreadMutex::Unlock()
{
    WLOG(DEBUG, "ulock");
    pthread_mutex_unlock(&lock_);
}

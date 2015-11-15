//
//
//
//
#ifndef SINGLETON
#define SINGLETON
#include "cthread_mutex.h"

template<typename T>
class Singleton
{
    public:

        static T* Instance();

    private:
        static T* instance_;
        static CthreadMutex* cthread_mutex_;

        class Freer
        {
            public:
                ~Freer()
                {
                    if (instance_) {
                        delete instance_;
                        instance_ = NULL;
                    }
                }
        };
        static Freer freer_;
};

template<typename T>
T* Singleton<T>::instance_ = NULL;
template<typename T>
CthreadMutex* Singleton<T>::cthread_mutex_ = new CthreadMutex();

template<typename T>
T* Singleton<T>::Instance()
{
    if (instance_ == NULL) {
        cthread_mutex_->Lock();
        if (instance_ == NULL) {
            instance_ = new T();
            cthread_mutex_->Unlock();
            return instance_;
        }
        cthread_mutex_->Unlock();
    }
    return instance_;
}

#endif

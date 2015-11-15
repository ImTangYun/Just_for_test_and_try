#ifndef CTHREAD_H_
#define CTHREAD_H_
#include <pthread.h>
namespace cthread
{
class CThread;
class Runnuble
{
public:
    virtual ~Runnuble(){}
    virtual void Run(CThread* cthread, void* args) = 0;
};
class CThread
{
public:
    CThread();
    int Start(Runnuble* r, void* a);
    Runnuble* runnuble(){return runnuble_;}
    void* args(){return args_;}
    static void* Hook(void* args);
    void Join();
private:
    Runnuble* runnuble_;
    void* args_;
    pthread_t tid_;
};
} // namespace cthread

#endif // CTHREAD_H_

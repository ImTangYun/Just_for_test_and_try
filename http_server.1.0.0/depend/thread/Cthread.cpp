// yuntang
// file:Cthread.cpp
#include "Cthread.h"
namespace cthread
{
CThread::CThread()
{
	args_ = NULL;
	runnuble_ = NULL;
}
int CThread::Start(Runnuble* r, void* a)
{
	runnuble_ = r;
	args_ = a;
	int ret = pthread_create(&tid_, NULL, CThread::Hook, this);
	return ret;
}
void* CThread::Hook(void* args)
{
	CThread* thread = (CThread*) args;
	if (thread->runnuble()) {
		thread->runnuble()->Run(thread, thread->args());
	}
	return (void*)NULL;
}
void CThread::Join()
{
	pthread_join(tid_, NULL);	
}
} // namespace cthread


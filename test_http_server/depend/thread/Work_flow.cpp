// yuntang
// file:work_flow.cpp
#include <unistd.h>
#include <stdio.h>
#include "Work_flow.h"
namespace cthread
{
bool WorkFlow::Init(int thread_num)
{
	thread_num_ = thread_num;
	return true;
}
void WorkFlow::Start()
{
	running_ = true;
	CThread* thread;
	for (int i = 0; i < thread_num_; ++i) {
		thread = new CThread;
		int* which_thread = new int(i);
		thread->Start(this, (void*)which_thread);
		thread_[i] = thread;
	}
}
void WorkFlow::Run(CThread* thread, void* args)
{
	while (running_) {
		int *which_thread = (int*)args;
		Process(*which_thread);
		sleep(10);
	}
}
void WorkFlow::Stop()
{
	running_ = false;
	for (auto iter = thread_.begin();
			iter != thread_.end(); ++iter) {
		iter->second->Join();
		delete iter->second;
	}
}
void WorkFlow::Process(int which_thread)
{
	printf("this is thread %d of WorkFlow\n", which_thread);
}
WorkFlow::~WorkFlow()
{
	if (running_) {
		Stop();
	}
}
} // namespace cthread

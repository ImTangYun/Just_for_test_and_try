#ifndef COMMUNICATE_H_
#define COMMUNICATE_H_
#include "Cthread.h"

namespace myspace
{
using cthread::CThread;
using cthread::Runnuble;
class Communicate: public Runnuble
{
public:
	Communicate();
	~Communicate();
	void Start();
	void EventLoop();
	virtual void Run(CThread* cthread, void* args);
private:
	CThread* thread_;
};
} // namespace myspace

#endif // COMMUNICATE_H_

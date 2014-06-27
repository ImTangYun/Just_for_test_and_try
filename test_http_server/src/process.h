#ifndef PROCESS_H_
#define PROCESS_H_
#include "Work_flow.h"
namespace cthread
{
class Processor : public WorkFlow
{
public:
	Processor(){}
	virtual ~Processor(){}
	virtual void Process(int which_thread);
	void TestFun(int which_thread);
#if 0
#endif
};
} // namespace cthread

#endif // PROCESS_H_

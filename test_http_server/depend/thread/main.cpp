#include <stdio.h>
#include <unistd.h>
#include "Work_flow.h"
// #include "process.h"
using cthread::WorkFlow;
// using cthread::Processor;
int main()
{
	WorkFlow work;
	work.Init(5);
	work.Start();
	sleep(1);
	// Processor x;
	// x.Init(2);
	// x.Start();
	sleep(10);
	// x.Stop();
	work.Stop();
	sleep(5);
	return 0;
}


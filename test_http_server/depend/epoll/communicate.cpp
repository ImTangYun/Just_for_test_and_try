#include <stdio.h>
#include "communicate.h"

namespace myspace
{
Communicate::Communicate(){thread_ = new CThread;}
Communicate::~Communicate(){}
void Communicate::Start()
{
	thread_->Start(this, (void*)NULL);
}
void Communicate::Run(CThread* cthread, void* args)
{
	printf("this is a test program\n");
}
}



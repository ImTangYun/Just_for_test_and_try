/*
 *by:yuntang
 *filename:transmission.h
 *
 */
#include <stdio.h>
#include <unistd.h>
#include "transmission.h"
#include "Cthread.h"

namespace myspace
{
using cthread::CThread;
Transmission::Transmission(SocketContent* listen_content, EventPoller* event_poller)
{
	// trans_thread_ = new CThread;
}
Transmission::~Transmission()
{
}
bool Transmission::Start()
{
	// int* x = new int(0);
	// trans_thread_->Start(this, (void*)x);
	return true;
}
bool Transmission::Stop()
{
	return true;
}
void Transmission::EventLoop()
{
	printf("this is the main loop of transmission\n");
	sleep(1);
	EventLoop();
}
void Transmission::Run(CThread* cthread, void* args)
{
	EventLoop();
}
}

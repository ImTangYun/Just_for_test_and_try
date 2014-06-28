/*
 *by:yuntang
 *filename:transmission.h
 *
 */
#ifndef EPOLL_TRANSMISSION_H_
#define EPOLL_TRANSMISSION_H_
#include "Cthread.h"
namespace myspace
{
class EventPoller;
class SocketContent;
using cthread::Runnuble;
using cthread::CThread;
class Transmission: public Runnuble
{
public:
	Transmission(SocketContent* listen_content, EventPoller* event_poller);
	~Transmission();
	bool Start();
	bool Stop();
	void EventLoop();
	virtual void Run(CThread* cthread, void* args);
private:
	bool running_;
	CThread trans_thread_;
	EventPoller* event_poller_;
	SocketContent* listen_content_;
};
}

#endif // EPOLL_TRANSMISSION_H_

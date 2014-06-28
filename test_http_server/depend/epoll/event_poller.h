/*
 *by:yuntang 
 *filename:event_poller.h
 *
 */
#ifndef EPOLL_EVENT_POLLER_H_
#define EPOLL_EVENT_POLLER_H_

#define MAX_EVENT_NUM 255
#include <sys/epoll.h>
class SocketContent;

namespace myspace
{
enum EventCode
{
	Readable = EPOLLIN,
	Writable = EPOLLOUT,
	OtherThing = 2,
	NewConnect = 3
};
struct IoEvent
{
	SocketContent* socket_content;
	EventCode mask_ = OtherThing;
	int event_code_;
};
class EventPoller
{
public:
	EventPoller(){}
	virtual ~EventPoller(){}
	virtual bool AddEvent(SocketContent* content, bool eanble_read, bool eable_write) = 0;
	virtual bool SetEvent(SocketContent* content, bool eanble_read, bool eable_write) = 0;
	virtual bool ClearEvent() = 0;
	virtual int PollEvent(int timeout, IoEvent* io_event, int listenfd) = 0;
};

} // namespace myspace
#endif // EPOLL_EVENT_POLLER_H_

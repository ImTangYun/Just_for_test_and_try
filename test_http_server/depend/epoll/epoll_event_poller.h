/*
 *by:yuntang 
 *filename:epoll_event_poller.h
 *
 */
#ifndef EPOLL_EPOLL_EVENT_POLLER_H_
#define EPOLL_EPOLL_EVENT_POLLER_H_
#include <sys/epoll.h>
#include "event_poller.h"

#define MAX_EVENT_NUM 255

class SocketContent;

namespace myspace
{
class EpollEventPoller : public EventPoller
{
public:
	EpollEventPoller();
	virtual ~EpollEventPoller();
	virtual bool AddEvent(SocketContent* content, bool eanble_read, bool eable_write);
	virtual bool SetEvent(SocketContent* content, bool eanble_read, bool eable_write);
	virtual bool ClearEvent();
	virtual int PollEvent(int timeout, IoEvent* io_event, int listenfd);
private:
	int epoll_fd_;
};

} // namespace myspace
#endif // EPOLL_EPOLL_EVENT_POLLER_H_

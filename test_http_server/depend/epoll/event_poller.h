/*
 *by:yuntang 
 *filename:event_poller.h
 *
 */
#ifndef EPOLL_EVENT_POLLER_H_
#define EPOLL_EVENT_POLLER_H_

#define MAX_EVENT_NUM 255
class SocketContent;

namespace myspace
{
class EventPoller
{
public:
	EventPoller();
	virtual ~EventPoller();
	virtual bool AddEvent(SocketContent* content, bool eanble_read, bool eable_write) = 0;
	virtual bool SetEvent(SocketContent* content, bool eanble_read, bool eable_write) = 0;
	virtual bool ClearEvent() = 0;
	virtual int PollEvent(int timeout, int count) = 0;
};

} // namespace myspace
#endif // EPOLL_EVENT_POLLER_H_

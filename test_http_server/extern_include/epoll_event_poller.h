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
	virtual bool ClearEvent(SocketContent* content);
	virtual int PollEvent(int timeout, IoEvent* io_event, int listenfd);
	const int fd_num(){return fd_num_;}
protected:
	void SetNoBlocking(int fd);
	// void SetNonBlocking();
private:
	int epoll_fd_;
	int fd_num_;
};

} // namespace myspace
#endif // EPOLL_EPOLL_EVENT_POLLER_H_

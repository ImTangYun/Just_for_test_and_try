/*
 *by:yuntang 
 *filename:epoll_event_poller.cpp
 *
 */
#include <memory.h>
#include "socket_content.h"
#include "epoll_event_poller.h"
namespace myspace
{
EpollEventPoller::EpollEventPoller()
{
	epoll_fd_ = epoll_create(MAX_EVENT_NUM);
}
EpollEventPoller::~EpollEventPoller()
{

}
bool EpollEventPoller::AddEvent(SocketContent* content, bool eanble_read, bool eable_write)
{
	struct epoll_event ev;
	ev.data.fd = content->GetFd();
	if (eanble_read) ev.events |= EPOLLIN;
	if (eable_write) ev.events |= EPOLLOUT;
	epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, content->GetFd(), &ev);
	return true;
}
bool EpollEventPoller::SetEvent(SocketContent* content, bool eanble_read, bool eable_write)
{
	
	return true;
}
bool EpollEventPoller::ClearEvent()
{

	return true;
}
int EpollEventPoller::PollEvent(int timeout, IoEvent* io_event, int listenfd)
{
	struct epoll_event events[MAX_EVENT_NUM];
	int nfds = epoll_wait(epoll_fd_, events, MAX_EVENT_NUM, timeout);
	if (nfds > 0)
		memset(io_event, 0, sizeof(IoEvent) * MAX_EVENT_NUM);
	for (int i = 0; i < nfds; ++i) {
		if (events[i].data.fd == listenfd) {
			io_event[i].mask_ = NewConnect;
		} else if (events[i].events&EPOLLIN) {
			io_event[i].mask_ = Readable;
		} else if (events[i].events&EPOLLOUT) {
			io_event[i].mask_ = Writable;
		} else {
			io_event[i].mask_ = OtherThing;
		}
	}
	return nfds;
}
} // namespace myspace

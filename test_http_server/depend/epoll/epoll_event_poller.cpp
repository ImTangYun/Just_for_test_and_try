/*
 *by:yuntang 
 *filename:epoll_event_poller.cpp
 *
 */
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
int EpollEventPoller::PollEvent(int timeout, int count)
{
	return 0;
}
} // namespace myspace

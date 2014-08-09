/*
 *by:yuntang 
 *filename:epoll_event_poller.cpp
 *
 */
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include "socket_content.h"
#include "epoll_event_poller.h"
namespace myspace
{
EpollEventPoller::EpollEventPoller()
{
	epoll_fd_ = epoll_create(MAX_EVENT_NUM);

}
void EpollEventPoller::SetNoBlocking(int fd)
{
	int opt; 

	opt = fcntl(fd, F_GETFL); 
	if (opt < 0) { 
		printf("fcntl(F_GETFL) fail."); 
		exit(-1); 
	} 
	opt |= O_NONBLOCK; 
	if (fcntl(fd, F_SETFL, opt) < 0) { 
		printf("fcntl(F_SETFL) fail."); 
		exit(-1); 
	} 
}
EpollEventPoller::~EpollEventPoller()
{
}
bool EpollEventPoller::AddEvent(SocketContent* content, bool eanble_read, bool eable_write)
{
	if (fd_num_ >= MAX_EVENT_NUM) {
		return false;
	}
	struct epoll_event ev;
	ev.data.fd = content->GetFd();
	if (eanble_read) ev.events |= EPOLLIN;
	if (eable_write) ev.events |= EPOLLOUT;
	ev.data.ptr = content;
	epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, content->GetFd(), &ev);
	++fd_num_;
	return true;
}
bool EpollEventPoller::SetEvent(SocketContent* content, bool eanble_read, bool eable_write)
{
	if (content == NULL) {
		return false;
	}
	struct epoll_event ev;
	ev.events = 0;
	if (eable_write) ev.events |= EPOLLOUT | EPOLLET;
	if (eanble_read) ev.events |= EPOLLIN | EPOLLET;
	ev.data.ptr = content;
	epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, content->GetFd(), &ev);
	return true;
}
bool EpollEventPoller::ClearEvent(SocketContent* content)
{
	if (!content) return false;
	close(content->GetFd());
	struct epoll_event ev;
	epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, content->GetFd(), &ev);
	delete content;
	return true;
}
int EpollEventPoller::PollEvent(int timeout, IoEvent* io_event, int listenfd)
{
	struct epoll_event events[MAX_EVENT_NUM];
	int nfds = epoll_wait(epoll_fd_, events, MAX_EVENT_NUM, timeout);
	if (nfds > 0)
		memset(io_event, 0, sizeof(IoEvent) * nfds);
	for (int i = 0; i < nfds; ++i) {
		io_event[i].socket_content_ = static_cast<SocketContent*>(events[i].data.ptr);
		if (events[i].events&(EPOLLHUP | EPOLLERR)) {
			io_event[i].mask_ = IoClosed;
		} else {
			if (events[i].events&EPOLLIN) {
				io_event[i].mask_ = IoReadable;
			} else if (events[i].events&EPOLLOUT) {
				io_event[i].mask_ = IoWritable;
			}
		}
	}
	return nfds;
}
} // namespace myspace

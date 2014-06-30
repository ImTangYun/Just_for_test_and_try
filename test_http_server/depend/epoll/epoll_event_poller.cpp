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
bool EpollEventPoller::ClearEvent()
{

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
#if 0
int EpollEventPoller::PollEvent(int timeout, IoEvent* io_event, int listenfd)
{
	struct epoll_event ev, events[MAX_EVENT_NUM];
	int nfds = epoll_wait(epoll_fd_, events, MAX_EVENT_NUM, timeout);
	printf("%d\n", nfds);
	if (nfds > 0)
		memset(io_event, 0, sizeof(IoEvent) * MAX_EVENT_NUM);
	for (int i = 0; i < nfds; ++i) {
		if (events[i].data.fd == listenfd) {
			io_event[i].mask_ = NewConnect;

			int connfd = accept(listenfd, NULL, NULL);
			SetNoBlocking(connfd);	
			ev.data.fd = connfd;
			ev.events = EPOLLET | EPOLLIN;//  | EPOLLOUT; // |EPOLLET;
			// if (eanble_read) ev.events |= EPOLLIN;
			// if (eable_write) ev.events |= EPOLLOUT;

			epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, connfd, &ev);
			++fd_num_;
		} else if (events[i].events&EPOLLIN) {
			char buff[10000];

			int length;
			length = recv(events[i].data.fd, buff, sizeof(buff), 0);
			buff[length] = '\0';
			printf("epoll in length: %d %s\n", length, buff);
			// ev.events=EPOLLOUT|EPOLLET;
			ev.events = EPOLLET;
			// close(events[i].data.fd);
			epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, events[i].data.fd, &ev);
			// epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, events[i].data.fd, &ev);
			send(events[i].data.fd, buff, length, 0);

			// length = send(events[i].data.fd, "hello world!\n", length, 0);
			printf("epoll in 111111\n");
		} else if (events[i].events&EPOLLOUT) {
			printf("epoll out 111111\n");
			/*char buff[1000];
			snprintf(buff, sizeof(buff), "i have received some data\n");
			int flag = 0;
			flag |= MSG_NOSIGNAL;
			int length = send(events[i].data.fd, buff, 10, 0);
			if (length == -1) {
				printf("error at epoll_event_poller.cpp->EpollEventPoller->PollEvent()\n");
				exit(1);
			}
			ev.events=EPOLLIN|EPOLLET;
			epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, events[i].data.fd, &ev);
			printf("epoll out length: %d %s\n", length, buff);
			*/
		}
		
		// io_event[i].event_code_ = events[i].events;
		// io_event[i].fd_ = events[i].data.fd;
	}
	return nfds;
}
#endif
} // namespace myspace

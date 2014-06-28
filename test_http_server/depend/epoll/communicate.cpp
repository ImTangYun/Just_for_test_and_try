/*
 *by:yuntang
 *filename:communicate.cpp
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "communicate.h"
#include "epoll_event_poller.h"
#include "socket.h"
#include "socket_content.h"
namespace myspace
{
Communicate::Communicate(SocketContent* listen)
{
	listen_thread_ = new CThread;
	run_thread_ = new CThread;
	event_poller_ = new EpollEventPoller;
	listen_ = listen;
	event_poller_->AddEvent(listen_, true, true);
	running_ = true;
}
Communicate::~Communicate()
{

}
void Communicate::Start()
{
	int listenloop = ListenLoop;
	listen_thread_->Start(this, (void*)&listenloop);
	int mainloop = MainLoop;
	run_thread_->Start(this, (void*)&mainloop);
}
void Communicate::Run(CThread* cthread, void* args)
{
	int which = *(int*)args;
	if (which == ListenLoop)
		while (running_) {
				
			usleep(1000);
		}
	if (which == MainLoop) {
		EventLoop();
		usleep(1000);
	}
}
void Communicate::EventLoop()
{
	IoEvent io_event[MAX_EVENT_NUM];
	
	while (running_) {
		printf("epoll listenning....\n");
		int event_num = event_poller_->PollEvent(500,
				io_event, listen_->GetFd());
		for (int i = 0; i < event_num; ++i) {
			
			// a new connection request comming
			if (io_event[i].mask_ == NewConnect) {
				if (event_poller_->fd_num() >= MAX_EVENT_NUM)
					continue;
				int connfd = accept(listen_->GetFd(), NULL, NULL);
				if (connfd == -1) {
					printf("accept error in communicate.cpp->Communicate->EventLoop()\n");
					exit(1);
				}
				Socket* socket = new Socket;
				socket->SetFd(connfd);
				SocketContent* socket_content = new SocketContent(socket);
				event_poller_->AddEvent(socket_content, true, true);
				listen_->OnConnect();
				listen_->OnReceived(connfd);
				// DealWithFd(connfd);
				continue;
			}
			/*if ((io_event[i].event_code_ | Readable) || (io_event[i].event_code_ | Writable)) {
				listen_->OnReceived(io_event[i].socket_content->GetFd());
			}*/
		}
		usleep(1000);
	}
}
void Communicate::DealWithFd(int fd)
{

}
}



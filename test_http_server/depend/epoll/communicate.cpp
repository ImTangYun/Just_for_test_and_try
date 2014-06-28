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
Communicate::Communicate()
{
	listen_thread_ = new CThread;
	run_thread_ = new CThread;
	event_poller_ = new EpollEventPoller;
	Socket* socket = new Socket;
	socket->BuildListen();
	listen_ = new SocketContent(socket);
	event_poller_->AddEvent(listen_, true, true);
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
		int event_num = event_poller_->PollEvent(500,
				io_event, listen_->GetFd());
		for (int i = 0; i < event_num; ++i) {
			
			// a new connection request comming
			if (io_event[i].socket_content->GetFd() == listen_->GetFd()) {
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
				continue;
			}
		}
		usleep(1000);
	}
}
}



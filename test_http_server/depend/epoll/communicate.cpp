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
		// printf("epoll listenning....\n");
		int event_num = event_poller_->PollEvent(500,
				io_event, listen_->GetFd());
		for (int i = 0; i < event_num; ++i) {
			if (io_event[i].socket_content_->GetFd() == listen_->GetFd()) {
				int connfd = accept(listen_->GetFd(), NULL, NULL);
				Socket* socket = new Socket;
				socket->SetFd(connfd);
				SocketContent* socket_content = new SocketContent(socket);
				event_poller_->AddEvent(socket_content, true, false);
			} else if (io_event[i].mask_ == IoReadable) {
				char *buff = new char[MAX_RECV_LENGTH];
				int length = recv(io_event[i].socket_content_->GetFd(), buff, MAX_RECV_LENGTH, 0);
				printf("===length: %d\n", length);
				// listen_->OnReceived((void*)buff, length);
				Package pack(buff, length, io_event[i].socket_content_->GetFd());
				listen_->OnReceived(pack);
				// buff[length] = '\0';
				// printf("-----\n%s\n-------\n", buff);
				event_poller_->SetEvent(io_event[i].socket_content_, false, true);
			} else if (io_event[i].mask_ == IoWritable) {
				char buff[100] = "i have received some data\n";
				int length = send(io_event[i].socket_content_->GetFd(), buff, 26, 0);
				printf("send: %d\n", length);
				event_poller_->SetEvent(io_event[i].socket_content_, true, false);
			}
		}
		usleep(1000);
	}
}
void Communicate::DealWithFd(int fd)
{

}
}



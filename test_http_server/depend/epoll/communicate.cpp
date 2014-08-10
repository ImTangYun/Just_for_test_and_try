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
Communicate::Communicate(SocketContent* listen): send_pack_(NULL, 0, 0)
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
void Communicate::Init(TaskQueue<TaskNode>* taskqueue)
{
    taskqueue_ = taskqueue;
}
void Communicate::Start()
{
	int *listenloop = new int(ListenLoop);
	listen_thread_->Start(this, (void*)listenloop);
	int *mainloop = new int(MainLoop);
	run_thread_->Start(this, (void*)mainloop);
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
        printf("MainLoop\n");
		usleep(1000);
	}
    printf("MainLoop, %d\n", which);
}
void Communicate::EventLoop()
{
	IoEvent io_event[MAX_EVENT_NUM];
	
	while (running_) {
		int event_num = event_poller_->PollEvent(500,
				io_event, listen_->GetFd());
		for (int i = 0; i < event_num; ++i) {
 
	
	        if (io_event[i].socket_content_->GetFd() == listen_->GetFd()) {

	            int connfd = accept(listen_->GetFd(), NULL, NULL);
	            int* connfd_to_push =  new int(connfd);
	            printf("new connected %d\n", connfd);
	            TaskNode node;
	            node.data_ = (void*)connfd_to_push;
	            taskqueue_->Push(node);
				// Socket* socket = new Socket;
				// socket->SetFd(connfd);
				// SocketContent* socket_content = new SocketContent(socket);
				// event_poller_->AddEvent(socket_content, true, false);
			} else if (io_event[i].mask_ == IoReadable) {
				char *buff = new char[MAX_RECV_LENGTH];
				int length = recv(io_event[i].socket_content_->GetFd(),
                        buff, MAX_RECV_LENGTH, 0);
				if (length == 0) {
					event_poller_->ClearEvent(io_event[i].socket_content_);
					printf("closed fd: %d\n", io_event[i].socket_content_->GetFd());
					continue;
				}
				listen_->OnReceived((void*)buff, length);
				Package pack(buff, length, io_event[i].socket_content_->GetFd());
				listen_->OnReceived(pack, send_pack_);
				event_poller_->SetEvent(io_event[i].socket_content_, false, true);
			} else if (io_event[i].mask_ == IoWritable) {
				printf("out fd: %d\n", io_event[i].socket_content_->GetFd());

				event_poller_->SetEvent(io_event[i].socket_content_, true, false);
			} else if (io_event[i].mask_ == IoClosed) {
				printf("closed\n");
			}
		}
	}
}
void Communicate::DealWithFd(int fd)
{

}
}



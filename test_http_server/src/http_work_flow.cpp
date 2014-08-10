//
// by : yuntang
// file: http_work_flow.cpp
//
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "http_work_flow.h"
namespace http
{
HttpWorkFlow::HttpWorkFlow(TaskQueue<TaskNode>* taskqueue):taskqueue_(taskqueue)
{
}
HttpWorkFlow::~HttpWorkFlow()
{
}
void HttpWorkFlow::Process(int which_thread)
{
    printf("this is thread %d of HttpWorkFlow\n", which_thread);
    TaskNode task = taskqueue_->Pop();
    printf("poped %d \n", which_thread);
    Response(task);
}
void HttpWorkFlow::Response(TaskNode task)
{
    int connfd = *(int*)task.data_;
    delete (int*)task.data_;
    printf("HttpWorkFlow::Response(TaskNode task): %d\n", connfd);
    char *buff = new char[10240];
    int length = recv(connfd, buff, 10240, 0); 
    if (length == 0) {
        close(connfd);
        return;
    }   
    buff[length] = '\0';
    printf("%s\n", buff);
    // close(connfd);
    resp_.DealWithRequest(buff, length, connfd);
    delete buff;
    close(connfd);
    /*
    */
}
} // namespace http

#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "stream_socket_context.h"
#include "listen_socket_context.h"
#include "communicate_loop.h"
#include "listen_handler.h"
#include "end_point.h"
#include "log.h"
int ListenSocketContext::Init()
{
    struct sockaddr_in servaddr; 

    if ( (fd_ = socket(AF_INET, SOCK_STREAM, 0)) == -1 ) {
        WLOG(DEBUG, "create socket error: %s(errno: %d)",strerror(errno),errno);
        exit(0);
    }   

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(server_port_);

    if ( bind(fd_, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
        WLOG(DEBUG, "bind socket error: %s(errno: %d)",strerror(errno),errno);
        exit(0);
    }   

    if ( listen(fd_, 10) == -1) {
        WLOG(DEBUG, "listen socket error: %s(errno: %d)",strerror(errno),errno);
        exit(0);
    }
    int ret = communicate_loop_->AddEvent(this, true, false);
    WLOG(DEBUG, "AddEvent ret: %d", ret);
    return 0;
}
int ListenSocketContext::HandleInput()
{
    WLOG(DEBUG, "connecting request!");
    int connfd = accept(fd_, NULL, NULL);
    if (connfd < 0) {
        perror("accept error");
        exit(1);
    }
    socklen_t rsa_len = sizeof(struct sockaddr_in);
    struct sockaddr_in rsa;
    char *ip = NULL;
    int port = 0;
    if(getpeername(connfd, (struct sockaddr *)&rsa, &rsa_len) == 0)
    {
        ip = inet_ntoa(rsa.sin_addr);
        port = ntohs(rsa.sin_port);
    }
    WLOG(DEBUG, "client ip:%s, port:%d fd:%d", ip, port, connfd);
    SetNonblocking(connfd);
    char ip_port[30];
    snprintf(ip_port, 30, "%s:%d", ip, port);
    
    StreamSocketContext* socket_context = new
        StreamSocketContext(ip_port, communicate_loop_, net_handler_);
    EndPoint* end_point = new EndPoint(socket_context);
    socket_context->set_end_point(end_point);
    socket_context->ParseIpPort();
    socket_context->set_fd(connfd);
    net_handler_->OnAccepted(connfd);
    communicate_loop_->AddEvent(socket_context, true, false);
    return 0;
}

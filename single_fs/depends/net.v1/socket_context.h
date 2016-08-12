//
//
//
//
#ifndef SOCKET_CONTEXT
#define SOCKET_CONTEXT
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include "simple_list.h"
// #include "communicate_loop.h"
class CommunicateLoop;
using std::string;
class SocketContext
{
    public:
        SocketContext(CommunicateLoop* communicate_loop):communicate_loop_(communicate_loop){}
        SocketContext(const string ip_port, CommunicateLoop* communicate_loop):ip_port_(ip_port),
            ip_(NULL), port_(-1), fd_(-1), communicate_loop_(communicate_loop){}
        virtual ~SocketContext()
        {
            delete ip_;
            ip_ = NULL;
        }
        int fd() {return fd_;}
        void set_fd(int fd) {fd_ = fd;}
        virtual int HandleOutput() = 0;
        virtual int HandleInput() = 0;
        virtual void OnReceived() = 0;
        int ParseIpPort();
        void set_communicate_loop(CommunicateLoop* communicate_loop)
        {
            communicate_loop_ = communicate_loop;
        }
        void SetNonblocking(int sock)
        {
            int opts;
            opts = fcntl(sock, F_GETFL);
            if (opts < 0) {
                perror("set nonblocking\n");
                exit(1);
            }
            opts = opts | O_NONBLOCK;
            if (fcntl(sock, F_SETFL, opts) < 0) {
                perror("set nonblocking2\n");
                exit(1);
            }
        }
        void set_node(Node<SocketContext*>* node)
        {
            node_ = node;
        }
        Node<SocketContext*>* node()
        {
            return node_;
        }
    protected:
        CommunicateLoop* communicate_loop_;
        string ip_port_;
        char* ip_;
        int port_;
        int fd_;
        // need not release by this class, for it will be release by
        // class CommunicationLoop (recycled referenced pointor)
        Node<SocketContext*>* node_;
};
#endif

//
//
//
//
#include "packet.h"
#include "listen_handler.h"
#include "end_point.h"
#include "stream_socket_context.h"
#include "log.h"
int ListenHandler::OnReceived(Packet* packet)
{
    char* data = packet->data();
    EndPoint* end_point = packet->end_point();
    uint32_t length = packet->data_length();
    data[length] = '\0';
    WLOG(DEBUG, "received: %s, length:%d", data, length);
    StreamSocketContext* socket_context = end_point->socket_context();
    data[length] = ' ';
    socket_context->AsyncSendPacket(packet);
    return 0;
}
int ListenHandler::OnReceived(Packet* packet, EndPoint* end_point)
{
    char* data = packet->data();
    uint32_t length = packet->data_length();
    WLOG(DEBUG, "received: %s, length:%d", data, length);
    StreamSocketContext* socket_context = end_point->socket_context();
    socket_context->AsyncSendPacket(packet);
    return 0;
}
int ListenHandler::OnDisconnected(int fd)
{
    return 0;
}
int ListenHandler::OnConnected(int fd)
{
    return 0;
}
int ListenHandler::OnAccepted(int fd)
{
    WLOG(DEBUG, "accepted fd :%d", fd);
    return 0;
}
int ListenHandler::OnSent(Packet* packet)
{
    char* data = packet->data();
    uint32_t length = packet->data_length();
    WLOG(DEBUG, "sent: %s", data);
    return 0;
}
int ListenHandler::OnTimeOut()
{
    return 0;
}
int ListenHandler::OnClose(EndPoint* end_point)
{
}

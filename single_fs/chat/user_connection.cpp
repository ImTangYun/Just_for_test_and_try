//
//
//
#include "user_connection.h"
#include "end_point.h"
#include "log.h"
#include "packet.h"
#include "stream_socket_context.h"

void UserConnection::addUser(const string& user, EndPoint* end_point)
{
    cthread_mutex_->Lock();
    user_connections_[user] = end_point;
    connection_users_[end_point] = user;
    cthread_mutex_->Unlock();
}
void UserConnection::removeUser(const string& user)
{
    cthread_mutex_->Lock();
    auto iter = user_connections_.find(user);
    if (iter != user_connections_.end()) {
        connection_users_.erase(iter->second);
        user_connections_.erase(iter);
    }
    cthread_mutex_->Unlock();
}
void UserConnection::removeUser(EndPoint* end_point)
{
    WLOG(DEBUG, "before remove online user");
    cthread_mutex_->Lock();
    auto iter = connection_users_.find(end_point);
    if (iter != connection_users_.end()) {
        user_connections_.erase(iter->second);
        connection_users_.erase(iter);
        WLOG(DEBUG, "remove online user");
    }
    cthread_mutex_->Unlock();
}
bool UserConnection::contains(const string& user)
{
    return user_connections_.find(user) != user_connections_.end();
}
void UserConnection::sendToAllUser(Packet* packet)
{
    cthread_mutex_->Lock();
    for (auto iter = connection_users_.begin();
            iter != connection_users_.end(); ++iter) {
        StreamSocketContext* socket_context = iter->first->socket_context();
        socket_context->AsyncSendPacket(packet->GetCopy());
    }
    cthread_mutex_->Unlock();
    delete packet;
}
EndPoint* UserConnection::getEndPointByUser(const string& user)
{
    cthread_mutex_->Lock();
    EndPoint* end_point = NULL;
    auto iter = user_connections_.find(user);
    if (iter != user_connections_.end()) {
        WLOG(DEBUG, "find connection of user %s", user.c_str());
        end_point = iter->second;
    }
    cthread_mutex_->Unlock();
    return end_point;
}
CthreadMutex* UserConnection::cthread_mutex_ = new CthreadMutex();
UserConnection::UserConnection()
{
}
UserConnection::~UserConnection()
{
}

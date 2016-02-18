//
//
//
#ifndef _USER_CONNECTION_H_
#define _USER_CONNECTION_H_
#include <unordered_map>
#include <string>
#include "cthread_mutex.h"
using std::unordered_map;
using std::string;

class EndPoint;
class Packet;
class UserConnection
{
    public:
        UserConnection();
        ~UserConnection();
        void addUser(const string& user, EndPoint* end_point);
        void removeUser(const string& user);
        void removeUser(EndPoint* end_point);
        bool contains(const string& user);
        void sendToAllUser(Packet* packet);
        EndPoint* getEndPointByUser(const string& user);
    private:
        static CthreadMutex* cthread_mutex_;
        unordered_map<string, EndPoint*> user_connections_;
        unordered_map<EndPoint*, string> connection_users_;
};
#endif

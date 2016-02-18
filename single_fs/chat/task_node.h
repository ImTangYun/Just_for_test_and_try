//
//
//
//
#ifndef TASK_NODE_H_
#define TASK_NODE_H_

#include <string>
#include "common.h"
class Packet;
class Package;
using std::string;
struct TaskNode
{
        Packet* packet_;
        RequestType req_;
        ~TaskNode()
        {
        }
};
#endif

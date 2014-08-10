//
// by:yuntang
//file : http_work_flow.h
//
#ifndef SRC_HTTP_WORK_FLOW_H_
#define SRC_HTTP_WORK_FLOW_H_
#include "taskqueue.h"
#include "Work_flow.h"
#include "response.h"
namespace http
{
using myspace::TaskNode;
using myspace::TaskQueue;
using cthread::WorkFlow;
class HttpWorkFlow: public WorkFlow
{
public:
    HttpWorkFlow(TaskQueue<TaskNode>* taskqueue);
    ~HttpWorkFlow();
    virtual void Process(int which_thread);
    void Response(TaskNode task);
private:
    TaskQueue<TaskNode>* taskqueue_;
    http::Response resp_;
}; // HttpWorkFlow
} // namespace http
#endif // _HTTP_WORK_FLOW_H_

/*
 *by:yuntang
 *filename:task_queue.h
 *
 */
#ifndef SRC_TASK_QUEUE_H_
#define SRC_TASK_QUEUE_H_
#include <pthread.h>
#include <queue>
using std::queue;
namespace http
{
enum IoType
{
	In = 0,
	Out = 1,
	Other = 2
};
struct TaskNode
{
	void* data_;
	int length_;
	int fd_;
	IoType type_;
};

class TaskQueue
{
public:
	TaskQueue();
	~TaskQueue();
	bool Pop_front(TaskNode &node);
	bool Push_back(TaskNode node);
	int size() const;
private:
	pthread_rwlock_t rwlock_;
	queue<TaskNode> task_queue_;
	int size_;
};
}

#endif // SRC_TASK_QUEUE_H_

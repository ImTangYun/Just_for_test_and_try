/*
 *by:yuntang
 *filename:task_queue.cpp
 *
 */
#include "task_queue.h"
namespace http
{
TaskQueue::TaskQueue()
{
	size_ = 0;
}
TaskQueue::~TaskQueue()
{

}
bool TaskQueue::Pop_front(TaskNode &node)
{
	if (size_ > 0) {
		node = task_queue_.front();
		task_queue_.pop();
		return true;
		size_--;
	}
	return false;
}
bool TaskQueue::Push_back(TaskNode node)
{
	task_queue_.push(node);
	size_++;
	return true;
}
int TaskQueue::size() const
{
	return size_;
}
}


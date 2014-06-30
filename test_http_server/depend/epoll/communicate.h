#ifndef COMMUNICATE_H_
#define COMMUNICATE_H_
#include "Cthread.h"
#include "package.h"
#define MAX_RECV_LENGTH 10000
namespace myspace
{
using cthread::CThread;
using cthread::Runnuble;
class SocketContent;
class EpollEventPoller;
enum WhichThread
{
	MainLoop = 0,
	ListenLoop = 1
};
class Communicate: public Runnuble
{
public:
	Communicate(SocketContent* listen);
	~Communicate();
	void Start();
	void EventLoop();
	void DealWithFd(int fd);
	virtual void Run(CThread* cthread, void* args);
private:
	bool running_;
	CThread* listen_thread_;
	CThread* run_thread_;
	SocketContent* listen_;
	EpollEventPoller* event_poller_;
};
} // namespace myspace

#endif // COMMUNICATE_H_

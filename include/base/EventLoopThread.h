#ifndef __BASE_EVENTLOOPTHREAD_H__
#define __BASE_EVENTLOOPTHREAD_H__
//创建线程，并在线程里面创建EventLoop
//不可拷贝与复制
#include <memory>
#include "Mutex.h"
#include "Condition.h"
using namespace std;

class EventLoop;
class Thread;

class EventLoopThread{
public:
	EventLoopThread();
	EventLoop* startLoop();
	EventLoopThread(const EventLoopThread& rh) = delete;
	EventLoopThread& operator=(const EventLoopThread& rh) = delete;
	~EventLoopThread();
private:
	void threadFunc();
	unique_ptr<Thread> pThread_;
	EventLoop *loop_;
	MutexLock mutex_;
	Condition cond_;
};
#endif

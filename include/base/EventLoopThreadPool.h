#ifndef __BASE_EVENTLOOPTHREADPOOL_H__
#define __BASE_EVENTLOOPTHREADPOOL_H__
//事件循环池
//不可拷贝与复制
#include <vector>
using namespace std;

class EventLoop;

class EventLoopThreadPool{
public:
	EventLoopThreadPool(EventLoop *base,int count = 0);
	EventLoop* getNext();
	~EventLoopThreadPool();

	EventLoopThreadPool(const EventLoopThreadPool&) = delete;
	EventLoopThreadPool& operator=(const EventLoopThreadPool&) = delete;
private:
	vector<EventLoop*> loops_;
	int count_;
	int pos_;
	EventLoop *base_;
};

#endif

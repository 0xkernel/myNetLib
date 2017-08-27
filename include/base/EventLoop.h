#ifndef __BASE_EVENTLOOP_H
#define __BASE_EVENTLOOP_H
#include <vector>
#include <deque>
#include <memory>
#include <functional>
#include "Mutex.h"
using namespace std;
//不可复制与拷贝
struct epoll_event;
class Event;
class Poller;
class TimerQueue;

class EventLoop{
public:
	typedef function<void()> TimerCallback;//定时器回掉函数
	typedef function<void()> LoopCallback;//定时器回掉函数
	EventLoop(int activeNum = 1000);
	~EventLoop();
	void loop();//主循环
	bool inOwnThread();
	void updateEvent(Event *e);
	void removeEvent(Event *e);//删除事件
	void runInLoop(const LoopCallback& cb);//在IO线程中执行
	void runAfter(int sec,const TimerCallback& cb);//在sec秒后调用cb
	void runEvery(int sec,const TimerCallback& cb);//每隔sec秒调用cb
	void quit();//退出事件循环
	EventLoop(const EventLoop& rh) = delete;
	EventLoop& operator=(const EventLoop& rh) = delete;
private:
	void runWaitCall();//调用正在等待的任务
	void wakeup();//使其从阻塞在IO复用中返回

	int activeNum_;
	vector<Event*> activeEvents_;
	bool quit_;
	unique_ptr<Poller> poller_;

	MutexLock mutex_;//互斥锁

	unique_ptr<TimerQueue> timerQueue_; //定时器队列
	int timerQueueFd_;//timerfd
	unique_ptr<Event> timerEvent_;//与timerFd相关的Event
	
	deque<LoopCallback> waitCallQueue_;//等待在IO线程中调用的事件
	unique_ptr<Event> wakeupEvent_;//用于唤醒等待在IO复用上的系统调用
};
#endif

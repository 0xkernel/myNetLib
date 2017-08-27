#include "EventLoop.h"
#include "Log.h"
#include "Poller.h"
#include "Event.h"
#include "TimerQueue.h"
#include "Timer.h"
#include "Mutex.h"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include <sys/epoll.h>
#include <sys/timerfd.h>
#include <sys/eventfd.h>
#include <memory>
#include <functional>
#define gettid() syscall(__NR_gettid)
using namespace std;

__thread EventLoop* t_LoopInThisThread = nullptr;//每个线程最多只能有一个时间循环

void timerFdRead(int fd){
	uint64_t i;
	read(fd,&i,sizeof(i));
}

void eventFdRead(int fd){
	uint64_t i;
	read(fd,&i,sizeof(i));
}

int createEventFd(){
	int ret = eventfd(0,EFD_CLOEXEC|EFD_NONBLOCK);
	if(ret == -1){
		err_sys("create event fd error");
	}
	return ret;
}

EventLoop::EventLoop(int activeNum):activeNum_(activeNum),quit_(false),poller_(new Poller(activeNum)),\
								    timerQueue_(new TimerQueue()),timerQueueFd_(timerfd_create(CLOCK_MONOTONIC,O_CLOEXEC))\
									,timerEvent_(new Event(this,timerQueueFd_)), \
									wakeupEvent_(new Event(this,createEventFd())){
	if(t_LoopInThisThread!=nullptr){
		err_exit("current thread(tid = %d) already has EventLoop",gettid());
	}
	t_LoopInThisThread = this;
	activeEvents_.resize(activeNum_);
	
	if(timerQueueFd_ == -1){
		err_exit("EventLoop::EventLoop timerfd_create error");
	}

	timerEvent_->setReadCallback(bind(&timerFdRead,timerQueueFd_));
	wakeupEvent_->setReadCallback(bind(&eventFdRead,wakeupEvent_->fd()));
	wakeupEvent_->enableRead();
}

bool EventLoop::inOwnThread(){//判定在自己的线程
	if(t_LoopInThisThread == this) return true;

	return false;
}

void EventLoop::loop(){//事件主循环
	if(!inOwnThread()){
		err_exit("run event loop not in own thread");
	}

	timerEvent_->enableRead();
	timerQueue_->zeroTimeCache();
	
	while(!quit_){
		
		struct itimerspec its;
		its = timerQueue_->getTimeToExpires();
		timerfd_settime(timerQueueFd_,0,&its,nullptr);
		
		int count = (poller_)->dispatch(activeEvents_.data(),-1);
		
		timerQueue_->zeroTimeCache();
		timerQueue_->handleExpired();//处理到期的定时器
		
		for(int i = 0;i<count;i++){
			activeEvents_[i]->handleEvent();//处理活跃的IO事件
		}
		
		runWaitCall();//调用等待着的任务
	}
}

void EventLoop::updateEvent(Event *e){
	if(inOwnThread()){//在自己的线程中的话，就直接添加
		poller_->add(e);
	}
	else{//否则
		runInLoop(bind(&Poller::add,poller_.get(),e));	
	}
}

void EventLoop::removeEvent(Event *e){
	if(inOwnThread()){//在自己的线程中的话，就直接添加
		poller_->del(e);
	}
	else{//否则
		runInLoop(bind(&Poller::del,poller_.get(),e));	
	}
}

void EventLoop::wakeup(){//唤醒阻塞于IO复用上的系统调用
	uint64_t i = 1;
	write(wakeupEvent_->fd(),&i,sizeof(i));
}

void EventLoop::runInLoop(const LoopCallback& cb){
	if(inOwnThread()){//在IO线程中
		cb();//那就直接调用
		return;
	}

	{
		MutexLockGuard lock(mutex_);
		waitCallQueue_.push_back(cb);//否则等待下次调用
	}
	wakeup();
}

void EventLoop::runWaitCall(){//调用等待被处理的任务
	while(!waitCallQueue_.empty()){
		auto cb = waitCallQueue_.front();;
		waitCallQueue_.pop_front();;
		cb();
	}
}

void EventLoop::runAfter(int sec,const TimerCallback& cb){
	Timer t(sec,cb,Timer::ONCE);
	if(inOwnThread()){//在自己的线程中的话，就直接添加
		timerQueue_->addTimer(t);
	}
	else{//否则
		runInLoop(bind(&TimerQueue::addTimer,timerQueue_.get(),t));	
	}
}

void EventLoop::runEvery(int sec,const TimerCallback& cb){
	Timer t(sec,cb,Timer::PERSISTANT);
	if(inOwnThread()){//在自己的线程中的话，就直接添加
		timerQueue_->addTimer(t);
	}
	else{//否则
		runInLoop(bind(&TimerQueue::addTimer,timerQueue_.get(),t));	
	}
}

void EventLoop::quit(){//退出事件循环
	quit_ = true;
	if(!inOwnThread()){
		wakeup();
	}
}

EventLoop::~EventLoop(){
	t_LoopInThisThread = nullptr;
}

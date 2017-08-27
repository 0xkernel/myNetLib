#include "EventLoopThread.h"
#include "EventLoop.h"
#include "Thread.h"
#include <functional>
using namespace std;
EventLoopThread::EventLoopThread():pThread_(new Thread(bind(&EventLoopThread::threadFunc,this))),loop_(nullptr),cond_(mutex_){

}

EventLoop* EventLoopThread::startLoop(){
	pThread_->start();
	mutex_.lock();
	while(loop_ == nullptr){
		cond_.wait();
	}

	return loop_;
}

void EventLoopThread::threadFunc(){
	EventLoop loop;
	{
		MutexLockGuard lock(mutex_);
		loop_ = &loop;
	}
	cond_.notify();
	loop.loop();
}

EventLoopThread::~EventLoopThread(){
}

#include "EventLoopThreadPool.h"
#include "EventLoopThread.h"
#include "EventLoop.h"
EventLoopThreadPool::EventLoopThreadPool(EventLoop *base,int count):count_(count),base_(base),pos_(0){
	for(int i = 0;i!=count;i++){
		loops_.push_back(EventLoopThread().startLoop());
	}
}

EventLoop* EventLoopThreadPool::getNext(){
	if(count_ == 0){
		return base_;
	}
	
	EventLoop* ret = loops_[pos_++];
	
	if(pos_==count_){
		pos_ = 0;
	}

	return ret;
}

EventLoopThreadPool::~EventLoopThreadPool(){
	for(auto elem:loops_){
		elem->quit();
	}
}

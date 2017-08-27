#include "TimerQueue.h"
#include "Timer.h"
#include "Log.h"
#include <time.h>
#include <sys/timerfd.h>
#include <string.h>
#include <iostream>
using namespace std;

TimerQueue::TimerQueue():time_cache_(0){
}

void TimerQueue::handleExpired(){
	Timer nowTimer(gettime());
	
	while((!priQueue_.empty())&&(priQueue_.top() <= nowTimer)){
		Timer t = priQueue_.top();
		t.handle();
		priQueue_.pop();
		
		if(t.getFlag() == Timer::PERSISTANT){
			t.flushExpiresTime();
			priQueue_.push(t);
		}
	}
}

void TimerQueue::addTimer(const Timer& timer){
	priQueue_.push(timer);
}

int TimerQueue::gettime(){
	if(time_cache_ != 0) return time_cache_;

	struct timespec ts;
	if(clock_gettime(CLOCK_MONOTONIC,&ts)==-1){
		err_sys("TimerQueue::gettime clock_gettime error");
	}

	time_cache_ = ts.tv_sec;
	return time_cache_;
}

struct itimerspec TimerQueue::getTimeToExpires(){
	struct itimerspec its;
	bzero(&its,sizeof(its));

	int now = gettime();
	struct Timer nowTimer(now);

	if(priQueue_.empty()){
		return its;
	}

	if(priQueue_.top()<=nowTimer){
		its.it_value.tv_nsec = 1;
		return its;
	}

	its.it_value.tv_sec = priQueue_.top() - nowTimer;

	return its;
}

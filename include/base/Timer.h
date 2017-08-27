#ifndef __BASE_TIMER_H__
#define __BASE_TIMER_H__
#include <functional>
#include <time.h>
#include <string.h>
#include <iostream>
#include "Log.h"

using namespace std;

class Timer{
public:
	enum Flag{ONCE,PERSISTANT};
	typedef function<void()> Callback;
	explicit Timer(int t,const Callback& cb,Flag f=ONCE):time_(t),callback_(cb),flag_(f){
		struct timespec ts;
		
		if(clock_gettime(CLOCK_MONOTONIC,&ts)==-1){
			err_sys("Timer::Timer clock_gettime error");
		}
		timeToExpires = ts.tv_sec + time_;
	}
	
	explicit Timer(int t):timeToExpires(t){//创建用于比较timeToExpires的Timer
	}

	void flushExpiresTime(){//重新更新到期时间
		struct timespec ts;
		
		if(clock_gettime(CLOCK_MONOTONIC,&ts)==-1){
			err_sys("Timer::Timer clock_gettime error");
		}
		timeToExpires = ts.tv_sec + time_;
	
	}
	
	int handle(){
		callback_();
	}

	Flag getFlag(){return flag_;}

	bool operator>(const Timer & rh)const{
		return timeToExpires > rh.timeToExpires;
	}
	
	bool operator<(const Timer & rh)const{
		return timeToExpires < rh.timeToExpires;
	}
	
	bool operator<=(const Timer & rh)const{
		return timeToExpires <= rh.timeToExpires;
	}
	
	int operator-(const Timer & rh)const{
		return timeToExpires - rh.timeToExpires;
	}
	
	bool operator==(const Timer & rh)const{
		return timeToExpires == rh.timeToExpires;
	}
private:
	int timeToExpires;//到期的绝对时间
	int time_;
	Callback callback_;
	Flag flag_;//表示该定时器是响应一次，还是每隔一段相应一次
};

#endif

#ifndef __BASE_TIMERQUEUE_H__
#define __BASE_TIMERQUEUE_H__
#include <queue>
#include <vector>
#include "Timer.h"
//不可拷贝与复制
using namespace std;
struct itimerspec;

class TimerQueue{//定时器队列
public:
	TimerQueue();
	struct itimerspec getTimeToExpires();//返回距离下一次定时器到期的时间
	void addTimer(const Timer& timer);
	void handleExpired();//处理到期的定时器
	void zeroTimeCache(){time_cache_ = 0;}//清除缓冲的当前时间
	int gettime();//获取当前时间
	TimerQueue(const TimerQueue& rh) = delete;
	TimerQueue& operator=(const TimerQueue& rh) = delete;
private:
	int time_cache_;//缓存的当前时间
	priority_queue<Timer,vector<Timer>,greater<Timer>> priQueue_;//优先级队列
};
#endif

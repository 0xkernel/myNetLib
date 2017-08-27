#ifndef __BASE_POLLER_H__
#define __BASE_POLLER_H__
#include <map>
#include <vector>
using namespace std;
//IO demultiplexer
//禁止拷贝
class Event;
struct epoll_event;
class Poller{
public:
	Poller(int activeNum);
	void add(Event *e);//添加事件
	void del(Event *e);
	int dispatch(Event **active,int timeout);//调用IO复用，找出活跃的事件
	Poller(const Poller& rh) = delete;
	Poller& operator=(const Poller& rh) = delete;
private:
	int epfd_;
	int activeNum_;
	map<int,Event *> eventsmap_;
	vector<struct epoll_event> activeEpollEvents_;
};
#endif

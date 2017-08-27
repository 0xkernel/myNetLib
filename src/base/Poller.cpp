#include "Poller.h"
#include "sys/epoll.h"
#include "Log.h"
#include "Event.h"
#include <iostream>
using namespace std;

Poller::Poller(int activeNum):epfd_(-1),activeNum_(activeNum){
	epfd_ = epoll_create(3000);
	if(epfd_ == -1){
		err_sys("epoll_create error");
	}
	activeEpollEvents_.resize(activeNum_);
}

void Poller::add(Event *e){
	int op;
	decltype(eventsmap_)::iterator it = eventsmap_.find(e->fd());
	struct epoll_event epevent= {0,{0}};

	epevent.events = e->events();
	epevent.data.ptr = e;
	
	if(it == eventsmap_.end()){//添加
		op = EPOLL_CTL_ADD;
	}
	else{//修改
		op = EPOLL_CTL_MOD;
	}

	eventsmap_[e->fd()] = e;
	
	int ret = epoll_ctl(epfd_,op,e->fd(),&epevent);

	if(ret == -1){
		err_sys("Poller::add epoll_ctl error");
	}
}

void Poller::del(Event *e){
	decltype(eventsmap_)::iterator it = eventsmap_.find(e->fd());
	if(it == eventsmap_.end()) return;

	int ret = epoll_ctl(epfd_,EPOLL_CTL_DEL,e->fd(),nullptr);//从epfd中移除
	
	if(ret == -1){
		err_sys("Poller::del epoll delete error(fd == %d)",e->fd());
	}
	eventsmap_.erase(e->fd());//从eventsmap中移除
}

int Poller::dispatch(Event **active,int timeout){
	int count = epoll_wait(epfd_,activeEpollEvents_.data(),activeNum_,timeout);
	if(count == -1){
		err_sys("Poller::dispatch epoll wait error");
	}

	for(int i = 0;i<count;i++){//找到活跃的事件的 Event指针
		active[i] = static_cast<Event*>(activeEpollEvents_[i].data.ptr);
		active[i]->setRevents(activeEpollEvents_[i].events);
	}

	return count;
}

#include "Event.h"
#include "EventLoop.h"
#include <sys/epoll.h>

Event::Event(EventLoop* loop,int fd):loop_(loop),fd_(fd),events_(0),revents_(0){
}

void Event::update(){
	(loop_)->updateEvent(static_cast<Event*>(this));
}

void Event::enableRead(){
	events_ |= (EPOLLIN|EPOLLPRI|EPOLLRDHUP);update();
}

void Event::enableWrite(){
	events_ |= (EPOLLOUT);update();
}

void Event::enableError(){
	events_ |= (EPOLLERR);update();
}

void Event::disableRead(){
	events_ &= ~(EPOLLIN|EPOLLPRI|EPOLLRDHUP);update();
}

void Event::disableWrite(){
	events_ &= ~(EPOLLOUT);update();
}

void Event::disableError(){
	events_ &= ~(EPOLLERR);update();
}

void Event::disableAll(){
	disableRead();
	disableWrite();
	disableError();
}

void Event::handleEvent(){
	if(revents_ &(EPOLLIN|EPOLLPRI|EPOLLRDHUP)){
		readCallback_();
	}

	if(revents_ & EPOLLOUT){
		writeCallback_();
	}

	if(revents_ & EPOLLERR){
		errorCallback_();
	}
}
bool Event::isReading(){//已经启动读标志
	return events_ &= (EPOLLIN|EPOLLPRI|EPOLLRDHUP);
}

bool Event::isWriting(){//已经启动写标志
	return events_ &= (EPOLLOUT);
}

bool Event::isErroring(){//已经启动错误标志
	return events_ &= (EPOLLERR);
}

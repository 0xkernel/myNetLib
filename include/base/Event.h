#ifndef __BASE_EVENT_H__
#define __BASE_EVENT_H__
#include <functional>
#include <memory>
using namespace std;
//Reactor模式中的事件处理
//不可复制与拷贝
class EventLoop;

class Event{
public:
	typedef function<void ()> CallBack;
	explicit Event(EventLoop* loop,int fd);
	void setReadCallback(const CallBack& cb){readCallback_ = cb;}
	void setWriteCallback(const CallBack& cb){writeCallback_ = cb;}
	void setErrorCallback(const CallBack& cb){errorCallback_ = cb;}
	
	void enableRead();
	void enableWrite();
	void enableError();
	
	void disableRead();
	void disableWrite();
	void disableError();
	
	void disableAll();

	void handleEvent();
	int fd(){return fd_;}
	int events(){return events_;}
	int revents(){return revents_;}
	void setRevents(int e){revents_ = e;}

	bool isReading();//已经启动读标志
	bool isWriting();//已经启动写标志
	bool isErroring();//已经启动错误标志
	Event(const Event& rh) = delete;
	Event& operator=(const Event& rh) = delete;
private:
	void update();

	int events_;//用户关心的事件
	int revents_;//已经发生的事件
	EventLoop* loop_;
	int fd_;
	CallBack readCallback_;
	CallBack writeCallback_;
	CallBack errorCallback_;

};
#endif

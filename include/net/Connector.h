#ifndef __NET_CONNECTOR_H__
#define __NET_CONNECTOR_H__
//发起连接的类
//不可拷贝与复制
#include <memory>
#include <functional>
using namespace std;

class IpPortAddr;
class Socket;
class EventLoop;
class Event;

class Connector{
public:
	Connector(EventLoop* loop,const IpPortAddr& addr);

	void start();//开始发出连接
	~Connector();
	Connector(const Connector&) = delete;
	Connector& operator=(const Connector&) = delete;
private:
	void handleRead();//处理读

	EventLoop* loop_;
	unique_ptr<Socket> socket_;
	unique_ptr<IpPortAddr> addr_;
	unique_ptr<Event> event_;
};
#endif

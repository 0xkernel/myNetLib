#ifndef __NET_ACCEPTOR_H__
#define __NET_ACCEPTOR_H__
//接收器,接受新连接
//不可拷贝与复制
#include <memory>
#include <functional>
using namespace std;

class EventLoop;
class Event;
class IpPortAddr;
class Socket;

class Acceptor{
public:
	typedef function<void(int,const IpPortAddr&)> newConnectionCallback;//新连接时候的回调函数
	Acceptor(EventLoop *loop,const IpPortAddr& addr,const newConnectionCallback& cb);//创建一个接受新连接的接收器
	void listen();//开始监听
	~Acceptor();
	Acceptor(const Acceptor&) = delete;
	Acceptor& operator=(const Acceptor&) = delete;
private:
	void handleRead();//处理新来的连接

	EventLoop *loop_;
	unique_ptr<Socket> socket_;
	unique_ptr<IpPortAddr> addr_;
	unique_ptr<Event> event_;
	newConnectionCallback newConnCallback_;
};
#endif

#ifndef __NET_TCPSERVER_H__
#define __NET_TCPSERVER_H__
#include <memory>
#include <string>
#include <map>
using namespace std;
//拥有Acceptor ,暴露给我们使用的类，
//不可以拷贝与复制
class EventLoop;
class IpPortAddr;
class Acceptor;
class TcpConnection;
class Buffer;
class EventLoopThreadPool;

class TcpServer{
public:
	typedef function<void(const shared_ptr<TcpConnection>&)> connCallback;//当有新连接或断开连接时候调用的回调函数
	typedef function<void(const shared_ptr<TcpConnection>&,Buffer*)> newMsgCallback;//到有新消息的时候调用的回调函数
	typedef function<void(const shared_ptr<TcpConnection>&)> destroyConnCallback;//销毁连接
	TcpServer(EventLoop* loop,const IpPortAddr& addr,const connCallback& ccb,const newMsgCallback& mcb);
	
	void start();
	~TcpServer();

	TcpServer(const TcpServer&) = delete;
	TcpServer& operator=(const TcpServer&) = delete;
private:
	void newConn(int fd,const IpPortAddr& addr);//Acceptor中有新连接时候，会调用本函数创建TcpConnection
	void destroyConn(const shared_ptr<TcpConnection>& tcpConnPtr);//销毁连接
	void destroyConnInLoop(const shared_ptr<TcpConnection> tcpConnPtr);//在TcpServer中销毁连接

	EventLoop* loop_;
	connCallback connCallback_;
	newMsgCallback msgCallback_;
	unique_ptr<Acceptor> acceptor_;

	map<uint64_t,shared_ptr<TcpConnection>> tcpConnections_;//已经接受了的连接
	unique_ptr<EventLoopThreadPool> loopPool_;
};
#endif

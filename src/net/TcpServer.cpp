#include "TcpServer.h"
#include "IpPortAddr.h"
#include "Acceptor.h"
#include "TcpConnection.h"
#include "Log.h"
#include "EventLoopThreadPool.h"
#include "EventLoop.h"

using namespace std::placeholders;

TcpServer::TcpServer(EventLoop* loop,const IpPortAddr& addr,const connCallback& ccb,const newMsgCallback& mcb):loop_(loop),\
connCallback_(ccb),\
msgCallback_(mcb),\
acceptor_(nullptr),\
loopPool_(new EventLoopThreadPool(loop,2))\
{
	acceptor_.reset(new Acceptor(loop,addr,std::bind(&TcpServer::newConn,this,_1,_2)));
}

void TcpServer::newConn(int fd,const IpPortAddr& addr){//建立TcpConnection
	static uint64_t id = 0;
	EventLoop* ioLoop = loopPool_->getNext();//获得一个IO Loop
	shared_ptr<TcpConnection> tcpConnPtr(new TcpConnection(ioLoop,fd,addr,msgCallback_,connCallback_,bind(&TcpServer::destroyConn,this,_1),id++));
	tcpConnections_[id] = tcpConnPtr;
	
	ioLoop->runInLoop(bind(&TcpConnection::start,tcpConnPtr.get()));//让其在自己的IO线程中调用connCallback
}

void TcpServer::destroyConn(const shared_ptr<TcpConnection>& tcpConnPtr){
	loop_->runInLoop(bind(&TcpServer::destroyConnInLoop,this,tcpConnPtr));//在事件循环中销毁连接
}

void TcpServer::destroyConnInLoop(const shared_ptr<TcpConnection> tcpConnPtr){
	err_msg("TcpServer::destroyConn remove connection(%s)",tcpConnPtr->getAddr().c_str());
	tcpConnections_.erase(tcpConnPtr->getId());
}

void TcpServer::start(){
	acceptor_->listen();
}

TcpServer::~TcpServer(){
}

#include "Acceptor.h"
#include "Socket.h"
#include "IpPortAddr.h"
#include "Event.h"
#include "Log.h"
#include "EventLoop.h"
#include <sys/types.h>
#include <sys/socket.h>

Acceptor::Acceptor(EventLoop* loop,const IpPortAddr& addr,const newConnectionCallback& cb):loop_(loop)\
,socket_(new Socket(createBindFd(addr)))\
,addr_(new IpPortAddr(addr))\
,event_(new Event(loop,socket_->fd()))\
,newConnCallback_(cb)\
{
	event_->setReadCallback(bind(&Acceptor::handleRead,this));
}

void Acceptor::listen(){
	if(::listen(socket_->fd(),5) == -1){
		err_sys("listen error");
	}
	event_->enableRead();
}

void Acceptor::handleRead(){//处理新来的连接
	struct sockaddr_in sockaddr;
	socklen_t len = sizeof(sockaddr);
	int fd = accept(socket_->fd(),(struct sockaddr*)&sockaddr,&len);

	if(fd == -1){
		err_sys("accept error");
	}

	IpPortAddr addr(sockaddr.sin_addr.s_addr,sockaddr.sin_port);
	newConnCallback_(fd,addr);
}

Acceptor::~Acceptor(){
}

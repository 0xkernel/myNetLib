#include "Connector.h"
#include "Socket.h"
#include "IpPortAddr.h"
#include "Event.h"
Connector::Connector(EventLoop* loop,const IpPortAddr& addr):\
loop_(loop),\
socket_(new Socket(createConnFd())),\
addr_(new IpPortAddr(addr)),\
event_(new Event(loop,socket_->fd()))\
{
	event_->setReadCallback(bind(&Connector::handleRead,this));
}

void Connector::start(){

}

void Connector::handleRead(){

}

Connector::~Connector(){
}

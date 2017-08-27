#include "TcpConnection.h"
#include "EventLoop.h"
#include "Socket.h"
#include "IpPortAddr.h"
#include "Event.h"
#include "Log.h"
#include "Buffer.h"
#include <unistd.h>
#include <sys/socket.h>

TcpConnection::TcpConnection(EventLoop*loop,int fd,const IpPortAddr& addr,const newMessageCallback& mcb,const connectionCallback& ccb,const destroyConnCallback& dcb,uint64_t id):\
loop_(loop),\
socket_(new Socket(fd)),\
addr_(new IpPortAddr(addr)),\
mcb_(mcb),\
ccb_(ccb),\
dcb_(dcb),\
event_(new Event(loop,fd)),\
id_(id),\
outputBuffer_(new Buffer),\
inputBuffer_(new Buffer)\
{
	event_->setReadCallback(bind(&TcpConnection::handleRead,this));
	event_->setWriteCallback(bind(&TcpConnection::handleWrite,this));
}

void TcpConnection::start(){
	status_ = CONNECTED;
	event_->enableRead();

	ccb_(shared_from_this());
}

void TcpConnection::handleWrite(){//处理写
	int len = write(socket_->fd(),outputBuffer_->readAbleString().c_str(),outputBuffer_->readAbleBytes());
	
	if(len < 0){
		err_sys("TcpConnection::handleWrite write error");
	}

	if(len == outputBuffer_->readAbleBytes()){
		outputBuffer_->extractBytes(len);
		if(status_ == DISCONNECTING){
			::shutdown(socket_->fd(),SHUT_WR);//就关闭写
			event_->disableWrite();
		}
	}
	else{
		outputBuffer_->extractBytes(len);
	}
}

void TcpConnection::handleClose(){//关闭连接
	status_ = DISCONNECTED;
	ccb_(shared_from_this());//调用用户回调
	loop_->removeEvent(event_.get());//清理event
	dcb_(shared_from_this());//从TcpServer中清楚连接信息,！！！！必须放最后
}

void TcpConnection::handleRead(){
	int ret = inputBuffer_->readFd(socket_->fd());
	
	if(ret > 0){//接受数据成功
		mcb_(shared_from_this(),inputBuffer_.get());
	}
	else if(ret == 0){//关闭连接
		handleClose();
	}
	else{//出现错误

	}
}

string TcpConnection::getAddr()const{
	return addr_->getAddr();
}

TcpConnection::~TcpConnection(){
}

void TcpConnection::shutdownInLoop(){
	status_ = DISCONNECTING;
	if(outputBuffer_->readAbleBytes() == 0){//如果没有发送的数据
		::shutdown(socket_->fd(),SHUT_WR);//就关闭写
	}
}

void TcpConnection::shutdown(){//用户不再写数据
	if(status_ == CONNECTED){
		loop_->runInLoop(bind(&TcpConnection::shutdownInLoop,this));
	}
}

void TcpConnection::sendInLoop(const string data){
	if(outputBuffer_->readAbleBytes() == 0){//先直接调用write，
		int len = write(socket_->fd(),data.c_str(),data.size());
		if(len < 0){
			err_sys("TcpConnection::snedInLoop write error");
		}
		if(len != data.size()){//没有全部写完，则追加到outputBuffer_
			outputBuffer_->append(&(data.c_str()[len]),data.size() - len);
			if(!event_->isWriting()){
				event_->enableWrite();//开启写事件
			}
		}
	}
	else{//追加到outputBuffer_
		outputBuffer_->append(data);
		if(!event_->isWriting()){
			event_->enableWrite();//开启写事件
		}
	}
}

void TcpConnection::send(const string&data){//发送数据
	if(status_ == CONNECTED){
		loop_->runInLoop(bind(&TcpConnection::sendInLoop,this,data));
	}
}

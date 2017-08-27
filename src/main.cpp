#include <iostream>
#include <sys/timerfd.h>
#include <string.h>
#include <unistd.h>
#include <memory>
#include "EventLoop.h"
#include "TcpServer.h"
#include "IpPortAddr.h"
#include "TcpConnection.h"
#include "Buffer.h"
using namespace std;

void onConnection(const shared_ptr<TcpConnection>& conn){
	if(conn->connected()){
		cout<<"new connection:"<<conn->getAddr()<<endl;
		conn->send("hello,world\n");
	}
	else{
		cout<<"disconnection:"<<conn->getAddr()<<endl;
	}
}

void newMessage(const shared_ptr<TcpConnection>& conn,Buffer *buf){
	cout<<"addr: "<<conn->getAddr()<<" msg:"<<buf->readAbleString()<<endl;
	
	conn->send("received!\n");

	buf->extractBytes(buf->readAbleBytes());
}

int main(void){
	EventLoop loop;
	TcpServer server(&loop,IpPortAddr("127.0.0.1",9000),onConnection,newMessage);
	server.start();
	loop.loop();
	return 0;
}

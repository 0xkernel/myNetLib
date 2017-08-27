#include "Socket.h"
#include "Log.h"
#include "IpPortAddr.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>

Socket::Socket(int fd):fd_(fd){
	int flags = fcntl(fd_,F_GETFL,0);
	if(flags == -1){
		err_sys("fcntl error");
	}

	if(fcntl(fd_,F_SETFL,flags|O_NONBLOCK)==-1){
		err_sys("fcntl error");
	}
	
}

void Socket::setNonBlock(){
	int flags = fcntl(fd_,F_GETFL,0);
	if(flags == -1){
		err_sys("fcntl error");
	}

	if(fcntl(fd_,F_SETFL,flags|O_NONBLOCK)==-1){
		err_sys("fcntl error");
	}
}

Socket::~Socket(){
	close(fd_);
}

int createBindFd(const IpPortAddr& addr){//创建绑定地址的套接字
	int fd = socket(AF_INET,SOCK_STREAM,0);
	if(fd == -1){
		err_sys("createListenFd error");
	}

	int opt = 1;
	if(setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,(const void *)&opt,sizeof(opt))==-1){
		err_sys("setsockopt error");
	}
	
	struct sockaddr_in sockaddr = addr.getSockAddr();
	if(bind(fd,(struct sockaddr*)&(sockaddr),sizeof(struct sockaddr_in))==-1){
		err_sys("bind error");
	}

	return fd;
}
int createConnFd(){//创建发起连接的套接字
	int fd = socket(AF_INET,SOCK_STREAM,0);
	if(fd == -1){
		err_sys("createConnFd error");
	}
}

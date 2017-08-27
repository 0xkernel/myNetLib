#include "Buffer.h"
#include "Log.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

Buffer::Buffer():readAbleBegin(0),readAbleEnd(0){
}

int Buffer::readFd(int fd){
	char buffer[65536] = {0};
	int n = 0;
	n = read(fd,buffer,65535);//由于EPOLL使用的是LT模式，所以就算没读完也没有关系
	if(n <= 0){//出错或读取错误
		return n; 
	}

	buf_.append(buffer,n);
	readAbleEnd += n;
	return n;
}

string Buffer::readAbleString(){
	return string(buf_.begin()+readAbleBegin,buf_.begin()+readAbleEnd);
}

int Buffer::readAbleBytes()const{
	return readAbleEnd - readAbleBegin;
}

void Buffer::extractBytes(int n){
	if(n> readAbleEnd - readAbleBegin){
		err_msg("n > readAbleBytes");
	}
	
	readAbleBegin += n;
	if(readAbleEnd == readAbleBegin){
		readAbleEnd = readAbleBegin = 0;
		buf_ = string();
	}
}

void Buffer::append(const string & data){
	buf_.append(data);
	readAbleEnd += data.size();
}

void Buffer::append(const char* str,int len){
	buf_.append(str,len);
	readAbleEnd += len;
}

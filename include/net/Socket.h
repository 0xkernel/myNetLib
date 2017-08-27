#ifndef __NET_SOCKET_H__
#define __NET_SOCKET_H__
//不可拷贝与复制

class IpPortAddr;

class Socket{
public:
	Socket(int fd);
	int fd()const{return fd_;}
	~Socket();
	void setNonBlock();//将fd设置为非阻塞
	Socket(const Socket&) = delete;
	Socket& operator=(const Socket&) = delete;
private:
	int fd_;
};

int createBindFd(const IpPortAddr&);//创建绑定地址的套接字
int createConnFd();

#endif

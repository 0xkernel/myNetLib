#ifndef __NET_IPADDR_H__
#define __NET_IPADDR_H__
#include <string>
#include <arpa/inet.h>
using namespace std;
//存储IP,port端口地址信息

class IpPortAddr{
public:
	IpPortAddr(string ip,uint16_t port);
	IpPortAddr(uint32_t ip,uint16_t port);//ip与port 都为 网络中的表示
	struct sockaddr_in getSockAddr()const{return sockaddr;}
	string getAddr()const;//获取ip:port这样格式的string
private:
	string ip_;
	uint32_t ip_n_;//ip的数字表示
	uint16_t port_n_;//网络传输中使用的端口
	uint16_t port_h_;//主机使用的端口
	
	struct sockaddr_in sockaddr;
};

#endif

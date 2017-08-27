#include "IpPortAddr.h"
#include "Log.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

IpPortAddr::IpPortAddr(string ip,uint16_t port):ip_(ip),port_h_(port){
	if(inet_pton(AF_INET,ip.c_str(),&ip_n_)!=1){
		err_sys("inet_pton error");
	}
	port_n_ = htons(port);

	bzero(&sockaddr,sizeof(sockaddr));
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = port_n_;
	sockaddr.sin_addr.s_addr = ip_n_;
}

IpPortAddr::IpPortAddr(uint32_t ip,uint16_t port_n):ip_n_(ip),port_n_(port_n){
	char ip_str[32] = {0};
	if(inet_ntop(AF_INET,&ip,ip_str,32)==nullptr){
		err_sys("inet_ntop error");
	}
	ip_ = ip_str;
	port_h_ = ntohs(port_n);
	
	bzero(&sockaddr,sizeof(sockaddr));
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = port_n_;
	sockaddr.sin_addr.s_addr = ip_n_;
}

string IpPortAddr::getAddr()const{
	char buf[32] = {0};
	snprintf(buf,32,"%s:%d",ip_.c_str(),port_h_);

	return string(buf);
}

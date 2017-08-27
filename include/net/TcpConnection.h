#ifndef __NET_TCPCONNECTION_H__
#define __NET_TCPCONNECTION_H__
//TCP连接类
//不可拷贝与复制
#include <functional>
#include <memory>
#include <string>
using namespace std;
class EventLoop;
class IpPortAddr;
class Socket;
class Event;
class Buffer;

class TcpConnection:public enable_shared_from_this<TcpConnection>{
public:
	typedef function<void(const shared_ptr<TcpConnection>&,Buffer*)> newMessageCallback;
	typedef function<void(const shared_ptr<TcpConnection>&)> connectionCallback;
	typedef function<void(const shared_ptr<TcpConnection>&)> destroyConnCallback;
explicit TcpConnection(EventLoop* loop,int fd,const IpPortAddr& addr,const newMessageCallback& mcb,const connectionCallback& ccb,const destroyConnCallback& dcb,uint64_t id);
	
	string getAddr()const;//返回地址信息
	void start();//开始接受消息
	
	enum STATUS{CONNECTED,DISCONNECTED,DISCONNECTING};

	bool connected()const{return status_==CONNECTED;}//该连接是否建立成功
	uint64_t getId()const{return id_;}

	void shutdown();//用户不在写数据
	void send(const string &data);
	
	~TcpConnection();
	TcpConnection& operator=(const TcpConnection&) = delete;
	TcpConnection(const TcpConnection&) = delete;
private:
	void handleRead();//处理fd上的读（包括接受消息，断开连接,错误处理）
	void handleClose();//处理连接的断开
	void handleWrite();//处理写
	void shutdownInLoop();//
	void sendInLoop(const string data);
	
	EventLoop *loop_;
	unique_ptr<Socket> socket_;
	unique_ptr<IpPortAddr> addr_;
	newMessageCallback mcb_;
	connectionCallback ccb_;
	destroyConnCallback dcb_;
	unique_ptr<Event> event_;
	uint64_t id_;//代表本连接的标识符
	STATUS status_;

	unique_ptr<Buffer> outputBuffer_;//输出缓冲区
	unique_ptr<Buffer> inputBuffer_;//输入缓冲区
};

#endif

#ifndef __NET_BUFFER_H__
#define __NET_BUFFER_H__
#include <string>
using namespace std;

class Buffer{
public:
	Buffer();
	int readFd(int fd);//从Fd中读取数据到缓冲区
	string readAbleString();//返回可以读的字符串(不改变readAbleBegin的位置)
	int readAbleBytes()const;//可以读取的数据
	void append(const string& data);//追加数据
	void append(const char* str,int count);//追加数据
	void extractBytes(int n);//提取n个字节，（会改变readAbleBegin）
private:
	int readAbleBegin;//可以开始读的位置
	int readAbleEnd;//可以读的结束位置
	string buf_;//内部使用结构
};
#endif

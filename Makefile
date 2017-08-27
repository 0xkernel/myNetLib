IDIR = -I include/base -I include/net#include dir
ODIR = objs#objects dir
CXXC = g++ #C++ Compiler
CXXFLAGS = -c -g -std=c++11
LD = g++
LDFLAGS =  -lrt -l pthread
OBJS = $(ODIR)/main.o \
	   $(ODIR)/EventLoopThreadPool.o \
	   $(ODIR)/EventLoopThread.o \
	   $(ODIR)/EventLoop.o \
	   $(ODIR)/Event.o \
	   $(ODIR)/Poller.o \
	   $(ODIR)/TimerQueue.o \
	   $(ODIR)/Thread.o \
	   $(ODIR)/TcpServer.o \
	   $(ODIR)/TcpConnection.o \
	   $(ODIR)/Buffer.o \
	   $(ODIR)/Acceptor.o \
	   $(ODIR)/Connector.o \
	   $(ODIR)/Socket.o \
	   $(ODIR)/IpPortAddr.o \
	   $(ODIR)/Log.o

######################网络相关的代码
#Connector类
$(ODIR)/Connector.o : src/net/Connector.cpp
	$(CXXC) $(CXXFLAGS) $(IDIR) $< -o $@
#Buffer类
$(ODIR)/Buffer.o : src/net/Buffer.cpp
	$(CXXC) $(CXXFLAGS) $(IDIR) $< -o $@
#TcpConnection类
$(ODIR)/TcpConnection.o : src/net/TcpConnection.cpp
	$(CXXC) $(CXXFLAGS) $(IDIR) $< -o $@
#TcpServer类
$(ODIR)/TcpServer.o : src/net/TcpServer.cpp
	$(CXXC) $(CXXFLAGS) $(IDIR) $< -o $@
#Socket类
$(ODIR)/Socket.o : src/net/Socket.cpp
	$(CXXC) $(CXXFLAGS) $(IDIR) $< -o $@
#地址表示
$(ODIR)/IpPortAddr.o : src/net/IpPortAddr.cpp
	$(CXXC) $(CXXFLAGS) $(IDIR) $< -o $@
#接受新连接
$(ODIR)/Acceptor.o : src/net/Acceptor.cpp
	$(CXXC) $(CXXFLAGS) $(IDIR) $< -o $@
######################

######################基础代码
#线程或线程同步
$(ODIR)/Thread.o : src/base/Thread.cpp
	$(CXXC) $(CXXFLAGS) $(IDIR) $< -o $@

#定时器
$(ODIR)/TimerQueue.o : src/base/TimerQueue.cpp
	$(CXXC) $(CXXFLAGS) $(IDIR) $< -o $@

#事件管理器
$(ODIR)/Event.o : src/base/Event.cpp
	$(CXXC) $(CXXFLAGS) $(IDIR) $< -o $@

#IO demultiplexer
$(ODIR)/Poller.o : src/base/Poller.cpp
	$(CXXC) $(CXXFLAGS) $(IDIR) $< -o $@

#错误处理与日志
$(ODIR)/Log.o : src/base/Log.cpp
	$(CXXC) $(CXXFLAGS) $(IDIR) $< -o $@

#事件循环
$(ODIR)/EventLoop.o : src/base/EventLoop.cpp
	$(CXXC) $(CXXFLAGS) $(IDIR) $< -o $@

$(ODIR)/EventLoopThread.o : src/base/EventLoopThread.cpp
	$(CXXC) $(CXXFLAGS) $(IDIR) $< -o $@

$(ODIR)/EventLoopThreadPool.o : src/base/EventLoopThreadPool.cpp
	$(CXXC) $(CXXFLAGS) $(IDIR) $< -o $@

#############################

$(ODIR)/main.o : src/main.cpp
	$(CXXC) $(CXXFLAGS) $(IDIR) $< -o $@

main : $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@

all : main

clean:
	rm -rf objs/*

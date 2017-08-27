#ifndef MYNETLIB_THREAD_H
#define MYNETLIB_THREAD_H
#include <pthread.h>
#include <string>
#include <functional>

//禁止拷贝与复制
class Thread{
public:
    typedef std::function<void()> ThreadFunc;
    Thread(const ThreadFunc &func,const std::string& name=std::string());
    ~Thread();

    void join();
    void start();

    static void * runInThread(void *arg);

	Thread& operator=(const Thread& rh) = delete;
	Thread(const Thread& rh) = delete;
private:
    pthread_t pthreadID_;
    bool isRunning_;
    std::string name_;
    ThreadFunc func_;//
};
#endif //MYNETLIB_THREAD_H

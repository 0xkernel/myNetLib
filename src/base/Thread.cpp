#include "Thread.h"
#include <pthread.h>
#include <assert.h>
#include <iostream>

Thread::Thread(const ThreadFunc& func,const std::string& name):func_(func),name_(name),isRunning_(false){
}

Thread::~Thread() {
    if(isRunning_){
        pthread_detach(pthreadID_);
    }
}

void Thread::join(){
    assert(isRunning_);
    pthread_join(pthreadID_,NULL);
}

void Thread::start(){
    assert(!isRunning_);
    pthread_create(&pthreadID_,NULL,Thread::runInThread,this);
    isRunning_ = true;
}

void * Thread::runInThread(void *arg){
    Thread *t = static_cast<Thread*>(arg);
    t->func_();
}

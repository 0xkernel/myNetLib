#ifndef MYCODE_BASE_CONDITION_H
#define MYCODE_BASE_CONDITION_H

#include <sys/types.h>
#include <pthread.h>
#include "Mutex.h"

class Condition{//禁止拷贝与复制
public:
    explicit Condition(MutexLock &m):mutex_(m){
        pthread_cond_init(&cond_,NULL);
    }

    void wait(){
        pthread_cond_wait(&cond_,mutex_.getPthreadMuteax());
    }

    void notify(){
        pthread_cond_signal(&cond_);
    }

    void notifyAll(){
        pthread_cond_broadcast(&cond_);
    }

    ~Condition(){
        pthread_cond_destroy(&cond_);
    }
    Condition& operator=(const Condition& rh) = delete;
    Condition(const Condition & rh) = delete;
private:
    MutexLock &mutex_;
    pthread_cond_t cond_;
};

#endif //MYCODE_CONDITION_H

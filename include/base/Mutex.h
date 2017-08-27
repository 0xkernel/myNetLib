#ifndef MYCODE_BASE_MUTEX_H
#define MYCODE_BASE_MUTEX_H
#include <pthread.h>
#include <assert.h>

class MutexLock{//禁止拷贝构造与复制
public:
    MutexLock():holder_(0){
        pthread_mutex_init(&mutex_,NULL);
    }

    ~MutexLock(){
        assert(holder_ == 0);
        pthread_mutex_destroy(&mutex_);
    }

    bool isLockedByThisThread(){
        //未实现
        //return holder_ == CurrentThread::tid;
        return false;
    }

    void assertLocked(){
        assert(isLockedByThisThread());
    }

    void lock(){//仅供MutexLockGuard调用，严禁用户代码调用
        pthread_mutex_lock(&mutex_); //这两行顺序不能反
        //holder_ = CurrentThread::tid();
    }

    void unlock(){  //仅供MutexLockGuard调用，严禁用户代码调用
        holder_ = 0;    //这两行顺序不能反
        pthread_mutex_unlock(&mutex_);
    }

    pthread_mutex_t *getPthreadMuteax(){//仅供Condition调用，严禁用户代码调用
        return &mutex_;
    }

    MutexLock(const MutexLock& rh) = delete;
    MutexLock& operator=(const MutexLock& rh) = delete;
private:
    pthread_mutex_t mutex_;
    pid_t holder_;
};


class MutexLockGuard{//禁止拷贝构造与复制
public:
    explicit MutexLockGuard( MutexLock & m):mutex_(m){
        mutex_.lock();
    }
    ~MutexLockGuard(){
        mutex_.unlock();
    }
    MutexLockGuard(const MutexLockGuard& rh) = delete;
    MutexLockGuard& operator=(const MutexLockGuard&rh) = delete;
private:
    MutexLock &mutex_;
};

#define MutexLockGuard(x) static_assert(false,"missing mutex guard var name");

#endif //MYCODE_MUTEX_H

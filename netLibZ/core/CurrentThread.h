#ifndef _NETLIBZ_BASE_CURRENTTHREAD_H_
#define _NETLIBZ_BASE_CURRENTTHREAD_H_

#include <stdio.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <functional>

#include "netLibZ/core/Coroutine.h"
#include "netLibZ/core/ShareStack.h"

using namespace NetLibZ::Core;

namespace NetLibZ{

namespace CurrentThread {

typedef std::shared_ptr<Coroutine> CoroutinePtr;

extern __thread int t_cachedTid;
extern __thread char t_tidString[32];
extern __thread const char* t_threadName;
extern __thread Core::CoroutineEnv* coroutineEnv;
extern __thread Core::ShareStacks* shareStacks;

pid_t gettid();

//缓存该线程的线程id
void cacheTid();

//返回该线程的线程id
inline int tid() {
    if (__builtin_expect(t_cachedTid == 0, 0)) cacheTid();
    return t_cachedTid;//直接返回缓存的id
}

inline const char* tidString()  {
    return t_tidString;
}

inline const char* name() { 
    return t_threadName; 
}


//线程局部协程环境
void initCurThreadCorotinueEnv();

CoroutineEnv* getCoroutineEnvCurThread();

//线程局部共享栈
StackMemoryPtr getStackMemoryCurThread();

void initShareStacksCurThread(int stackSize,int stackNum);

//从线程局部环境中创建协程
CoroutinePtr createCoroutine(Function func);

//从线程局部环境中让出协程
void yieldCoroutine();

}



}
#endif
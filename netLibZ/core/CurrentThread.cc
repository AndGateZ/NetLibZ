
#include "netLibZ/core/CurrentThread.h"
#include "netLibZ/core/coctx.h"

using namespace NetLibZ::Core;

namespace NetLibZ{

namespace CurrentThread{

__thread int t_cachedTid = 0;
__thread char t_tidString[32];
__thread const char* t_threadName = "default";

__thread Core::CoroutineEnv* coroutineEnv = NULL;	//暂不清楚是否需要static，屏蔽上层对env的操作
__thread Core::ShareStacks* shareStacks = NULL;


pid_t gettid() { return static_cast<pid_t>(::syscall(SYS_gettid)); }

//缓存该线程的线程id
void cacheTid(){
    if (t_cachedTid == 0) {
    t_cachedTid = gettid();
  }
}

void initCurThreadCorotinueEnv(){
	if(!coroutineEnv){
		coroutineEnv = new CoroutineEnv();
		Coroutine* coroutineMain = new Coroutine();
		coctx_init(coroutineMain->ctx());
		//coroutineMain->allocStackMemory();
		coroutineEnv->pushToCallStack(coroutineMain);
	}
}

CoroutineEnv* getCoroutineEnvCurThread(){
	if(!coroutineEnv) initCurThreadCorotinueEnv();
	return coroutineEnv;
}

//自动在获取memory的时候调用
StackMemoryPtr getStackMemoryCurThread(){
	if(!shareStacks){
		shareStacks = new ShareStacks(1024*128,1);
		if(!shareStacks->initStack()){
			//LOG error
		}
	}
	return shareStacks->getStackMemory();
}

//手动调用
void initShareStacksCurThread(int stackSize,int stackNum){
	if(!shareStacks){
		shareStacks = new ShareStacks(stackSize,stackNum);
		if(!shareStacks->initStack()){
			//LOG error
		}
	}
}

CoroutinePtr createCoroutine(Function func){
	initCurThreadCorotinueEnv();
	CoroutinePtr co(new Coroutine(std::move(func)));
	coctx_init(co->ctx());
	co->allocStackMemory(true);
	return co;
}

void yieldCoroutine(){
	Coroutine* cur = coroutineEnv->getTopCo();
	coroutineEnv->popCallStack();
	Coroutine* last = coroutineEnv->getTopCo();
	swap(cur,last);
}

}

}

#ifndef _NETLIBZ_CORE_CORO_H_
#define _NETLIBZ_CORE_CORO_H_

#include <functional>
#include <memory>
#include <vector>

#include "netLibZ/base/uncopyable.h"
#include "netLibZ/core/ShareStack.h"
#include "netLibZ/core/coctx.h"

namespace NetLibZ{

namespace Core{

typedef std::function<void()> Function;
typedef std::shared_ptr<StackMemory> StackMemoryPtr;
typedef std::shared_ptr<Coroutine> CoroutinePtr;

//全局函数，协程调度的基石
void saveStackBuffer(Coroutine* curCor);
void swap(Coroutine* curCor, Coroutine* nextCo);
void runFunc(Coroutine* co,void *);


class Coroutine : uncopyable, public std::enable_shared_from_this<Coroutine>{
public:
	typedef std::function<void()> Function;
	typedef std::shared_ptr<StackMemory> StackMemoryPtr;

	enum State{	Ready,Running,Suspend,Terminated };	

	Coroutine();				//协程环境的协程构造
	Coroutine(Function func);	//子协程的协程构造
	~Coroutine();

	void allocStackMemory(bool isSharedMemory);

	void resume();
	void yield();

	coctx_t* ctx() {return &ctx_; }
	
	StackMemoryPtr getStackMemory() { return stackMemory_;}

	void setStackSp(char* sp) { stackSp_ = sp;}
	char* getStackSp() { return stackSp_;}
	
	char* getSaveBuffer() { return saveBuffer_;}
	void setSaveBuffer(char* buffer) { saveBuffer_ = buffer;}
	bool hasSaveBuffer() { return saveBuffer_!=NULL;}
	int getSaveBufferSize() const {return saveSize_;}
	void setSaveBufferSize(int len) {saveSize_ = len;}

	void setState(State s) { state_ = s;}
	bool isEnvCoroutine() const { return isEnvCoroutine_;}

	void func();

private:
	
	Function func_;			//协程任务函数

	coctx_t ctx_;			//上下文

	StackMemoryPtr stackMemory_;	//由线程分配的共享栈内存

	char* stackSp_;			//协程运行时的栈顶指针（可能是共享栈可能不是）,用于换出的时候保存栈，换入的时候恢复栈数据，是一个用于换出的临时变量
	int saveSize_;			//协程临时栈保存数据的长度
	char* saveBuffer_;		//协程临时栈

	State state_;			//协程状态
	bool isEnvCoroutine_;	//是否是协程环境的主协程

};


class CoroutineEnv : uncopyable{
public:
	CoroutineEnv();
	~CoroutineEnv();
	
	void pushToCallStack(Coroutine* Coroutine);
	Coroutine* getTopCo();
	void popCallStack();

	void setOccupyCoroutine( Coroutine* coroutine){ occupyCoroutine_ = coroutine; }
	void setPendingCoroutine( Coroutine* coroutine){ pendingCoroutine_ =  coroutine; }
	Coroutine* getOccupyCoroutine() {return occupyCoroutine_;}
	Coroutine* getPendingCoroutine() {return pendingCoroutine_;}

private:
	std::vector<Coroutine*> callStack_;	//协程调用链的栈
	int stackIndex_;

	//用于切换后恢复共享栈内存原有主人的数据,swap中用到
	//这意味着，协程即使被弹出callStack_,但仍然被occupyCoroutine_或者共享栈内存指向引用，真正内存释放的时机是不确定的
	//协程析构时机最正确的应当由用户来做，上层认为此协程完成了工作，在结束前把stackBuffer_、stackMemory_引用都去掉
	Coroutine* occupyCoroutine_;			//当前占用共享栈的协程对象指针
	Coroutine* pendingCoroutine_;			//即将占用共享栈的目标协程对象指针
	

};


}

}

//独立栈+协程池？协程太多了会内存不够
//共享栈+每次和寄存器切换栈，共享栈不足时使用临时栈
//方案1：ucontext
//方案2：libco √


#endif


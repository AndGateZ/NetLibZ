#include <string.h>
#include <iostream>

#include "netLibZ/core/Coroutine.h"
#include "netLibZ/core/CurrentThread.h"

extern "C"
{
	extern void coctx_swap( coctx_t *,coctx_t* ) asm("coctx_swap");
};

namespace NetLibZ{

namespace Core{

//typedef std::shared_ptr<Coroutine> CoroutinePtr;
typedef std::shared_ptr<StackMemory> StackMemoryPtr;

//全局函数，协程调度的基石
void saveStackBuffer(Coroutine* curCor){
	StackMemoryPtr stackMemory = curCor->getStackMemory();
	int len = stackMemory->stackBottomPtr() - curCor->getStackSp();	//共享栈空间栈底指针 - 共享栈当前栈顶指针 = 栈使用数据大小

	//如果临时栈存在，数据已经无用了，因为当前curCor正在运行，临时栈不可能需要恢复
	char* saveBuffer = curCor->getSaveBuffer();
	if(saveBuffer){
		free(saveBuffer);
		curCor->setSaveBuffer(NULL);
	}

	saveBuffer = (char*)malloc(len);
	curCor->setSaveBufferSize(len);
	curCor->setSaveBuffer(saveBuffer);
	//拷贝 共享栈数据->临时栈
	memcpy(saveBuffer,curCor->getStackSp(),len);

}

void swap(Coroutine* curCor, Coroutine* nextCo){
	//对于主协程，此刻运行在栈上，对于协程而言，此刻运行在堆的共享栈中
	CoroutineEnv* env = CurrentThread::getCoroutineEnvCurThread();

	//当前运行的协程的栈顶指针，给curCor配置，便于后面切回的时候从buffer里换入数据
	char c;
	curCor->setStackSp(&c);		
	
	//如果是主协程，即将切入的是系统栈，无需对共享栈考虑栈冲突的问题
	if(nextCo->isEnvCoroutine()){
		env->setOccupyCoroutine(NULL);
		env->setPendingCoroutine(NULL);
	}
	else{
		//配置协程环境、共享栈
		env->setPendingCoroutine(nextCo);		//即将换入共享栈的协程
		Coroutine* occupyStackMemoryCo = nextCo->getStackMemory()->getOccupyCoroutine();	//nextco的共享栈正在服务的协程
		nextCo->getStackMemory()->setOccupyCoroutine(nextCo);	//nextco的共享栈服务于自己
		env->setOccupyCoroutine(occupyStackMemoryCo);	//正在占据共享栈的协程,设置这个是为了后面occupyCo恢复换入方便
		
		//将占据共享栈的协程数据保存到其临时空间
		if(occupyStackMemoryCo && occupyStackMemoryCo!=nextCo){
			saveStackBuffer(occupyStackMemoryCo);
		}
	}


	coctx_swap(curCor->ctx(),nextCo->ctx()); ///当前通用寄存器 -> curr->ctx   pending_co->ctx->当前通用寄存器

	//恢复之前被换到临时空间的栈
	CoroutineEnv* envNow = CurrentThread::getCoroutineEnvCurThread();
	//之前被驱赶出去的协程需要恢复,但如果是切换到主协程，pendingCoNow occupyCoNow 都为NULL了,无需恢复
	Coroutine* pendingCoNow = envNow->getPendingCoroutine();
	Coroutine* occupyCoNow = envNow->getOccupyCoroutine();
	if(pendingCoNow && occupyCoNow && pendingCoNow != occupyCoNow){
		if(pendingCoNow->hasSaveBuffer() && pendingCoNow->getSaveBufferSize() > 0 ){
			//临时栈换回到共享栈
			memcpy( pendingCoNow->getStackSp(),pendingCoNow->getSaveBuffer(),pendingCoNow->getSaveBufferSize());
		}
	}
}

void runFunc(Coroutine* co,void *){
	co->func();
	co->setState(Coroutine::State::Terminated);
	CurrentThread::yieldCoroutine();
}

//Coroutine
//主协程构造
Coroutine::Coroutine()
	:stackMemory_(NULL),
	stackSp_(NULL),
	saveSize_(0),
	saveBuffer_(NULL),
	state_(Ready),
	isEnvCoroutine_(true)
{
	
}

//子协程构造
Coroutine::Coroutine(Function func)
	:func_(std::move(func)),
	stackMemory_(NULL),
	stackSp_(NULL),
	saveSize_(0),
	saveBuffer_(NULL),
	state_(Ready),
	isEnvCoroutine_(false)
{
	
}

Coroutine::~Coroutine(){
	//释放临时栈
	if(!saveBuffer_){
		free(saveBuffer_);
		saveBuffer_ = NULL;
	}
	//去除stackMemory_对本协程的引用
	if(stackMemory_->getOccupyCoroutine() == this){
		stackMemory_->setOccupyCoroutine(NULL);
	}

	//此时协程环境的协程栈不会有本协程的指针
}


void Coroutine::allocStackMemory(bool isSharedMemory){
	//分配独立栈
	if(!isSharedMemory){
		StackMemoryPtr stackMemory(new StackMemory(1024*128));
		stackMemory_ = stackMemory;			
		ctx_.ss_sp = stackMemory->stackPtr();
		ctx_.ss_size = stackMemory->stackSize();
	}
	//分配共享栈
	else{
		StackMemoryPtr stackMemory = CurrentThread::getStackMemoryCurThread();
		stackMemory_ = stackMemory;			//分配一块共享栈
		ctx_.ss_sp = stackMemory->stackPtr();
		ctx_.ss_size = stackMemory->stackSize();
	}
	
}


void Coroutine::resume(){
	//state_ == ready Suspend
	CoroutineEnv* env = CurrentThread::getCoroutineEnvCurThread();
	Coroutine* curCo = env-> getTopCo();
	if(state_ == Ready){
		coctx_make(&ctx_,(coctx_pfn_t)runFunc,this,NULL);
	}
	env->pushToCallStack(this);
	state_ = Running;
	swap(curCo,this);	//底层更换为裸指针，底层不会发生内存泄漏，上层智能指针来控制生命周期
}

void Coroutine::yield(){
	CurrentThread::yieldCoroutine();
}

void Coroutine::func(){
	if(func_){
		func_();
	}
}

//CoroutineEnv
CoroutineEnv::CoroutineEnv()
	:callStack_(128),//如果不列表初始化，对象会调用默认构造函数
	stackIndex_(0),	
	occupyCoroutine_(),
	pendingCoroutine_()
{
	
}

CoroutineEnv::~CoroutineEnv(){
	occupyCoroutine_ = NULL;
	pendingCoroutine_ = NULL;
}

void CoroutineEnv::pushToCallStack(Coroutine* coroutine){
	if(stackIndex_>=128){
		//LOG
	}
	callStack_[stackIndex_++] = coroutine;
}
Coroutine* CoroutineEnv::getTopCo() {
	return callStack_[stackIndex_-1];
}

void CoroutineEnv::popCallStack(){
	if(stackIndex_<0){
		//LOG
	}
	stackIndex_--;
}

}

}
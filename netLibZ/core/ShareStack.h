#ifndef _NETLIBZ_CORE_SHARESTACK_H_
#define _NETLIBZ_CORE_SHARESTACK_H_

#include <vector>
#include <memory>

#include "netLibZ/base/uncopyable.h"

namespace NetLibZ{

namespace Core{

class Coroutine;

class StackMemory : uncopyable{
public:

	StackMemory(int stackSize);
	~StackMemory();

	bool initMemory();		//二段式初始化，保证空间分配成功

	int stackSize() const  { return stackSize_; }
	char* stackPtr() const { return  stackPtr_; }
	char* stackBottomPtr() const { return  stackBottomPtr_;}

	Coroutine* getOccupyCoroutine();
	void setOccupyCoroutine(Coroutine* co);

private:
	Coroutine* occupyCoroutine_;				//正在使用此栈空间的协程

	int stackSize_;				//尺寸
	char* stackBottomPtr_; 		//栈底指针  stackBottomPtr_ = stackPtr_ + stackSize_
	char* stackPtr_;			//栈顶指针，栈空间指针

};


class ShareStacks : uncopyable{
public:
	typedef std::shared_ptr<StackMemory> StackMemoryPtr;

	ShareStacks(int stackSize,int stackNum);
	~ShareStacks();

	bool initStack();		//二段式初始化，保证空间分配成功

	//这里如果直接返回StackMemory，返回了一个副本，其中的指针将不受控，会出问题
	//如果返回*指针，那为什么不用智能指针来维护生命周期呢，stackmemory主要是被协程使用，需要在主协程析构中注意析构顺序问题
	StackMemoryPtr getStackMemory();	

private:
	
	int stackSize_;			//栈内存尺寸，和StackMemory一致
	int stackNum_;			//栈数量
	size_t allocIdx_;		//当前的分配下标
	std::vector<StackMemoryPtr> stackMemorys_;		//用智能指针管理的意义在哪里呢，其他地方都是使用裸指针来使用内存
	bool initFinished_;
};

}

}

#endif

//一开始设想用智能指针管理空间，内存栈区的大小在动态变化，每一个协程使用的栈大小都不一样，需要实时计算后拷贝
//以及之后的临时栈换入栈区内部的拷贝动作，如果使用智能指针会非常不方便
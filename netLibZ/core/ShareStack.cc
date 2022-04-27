#include <stdlib.h>

#include "netLibZ/core/ShareStack.h"

namespace NetLibZ{

namespace Core{
	
typedef std::shared_ptr<StackMemory> StackMemoryPtr;
typedef std::shared_ptr<Coroutine> CoroutinePtr;

//StackMemory
StackMemory::StackMemory(int stackSize)
	:occupyCoroutine_(),
	stackSize_(stackSize),
	stackBottomPtr_(NULL),
	stackPtr_(NULL){}

StackMemory::~StackMemory(){
	if(stackPtr_) free(stackPtr_);
}

bool StackMemory::initMemory(){
	if(!stackPtr_) {
		stackPtr_ = (char*)malloc(stackSize_);
		if(!stackPtr_) return false;
		stackBottomPtr_ = stackPtr_ + stackSize_;
		return true;
	}
	else return true;

}

Coroutine* StackMemory::getOccupyCoroutine(){
	return occupyCoroutine_;
}

void StackMemory::setOccupyCoroutine(Coroutine* co){ 
	occupyCoroutine_ = co; 
}

//ShareStacks
ShareStacks::ShareStacks(int stackSize,int stackNum)
	:stackSize_(stackSize),
	stackNum_(stackNum),
	allocIdx_(0),
	stackMemorys_(),
	initFinished_(false){}

ShareStacks::~ShareStacks(){}

bool ShareStacks::initStack(){
	if(!initFinished_){
		//分配空间
		for(int i = 0;i<stackNum_;++i){
			stackMemorys_.push_back(StackMemoryPtr(new StackMemory(stackSize_)));
			if(!stackMemorys_[i]->initMemory()) {
				//LOG error
				return false;
			}
		}
		initFinished_ = true;
	}
	return true;
}

StackMemoryPtr ShareStacks::getStackMemory(){
	if(!initFinished_) {
		//LOG error
		return NULL;
	}
	
	StackMemoryPtr memory = stackMemorys_[allocIdx_];
	allocIdx_ = (allocIdx_ + 1) % stackNum_;
	return memory;
}



}

}
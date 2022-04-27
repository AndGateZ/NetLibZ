
#include <iostream>
#include <memory>
#include <benchmark/benchmark.h>

#include "netLibZ/core/Coroutine.h"
#include "netLibZ/core/CurrentThread.h"

using namespace std;
using namespace NetLibZ::Core;

void Consumer(){
	cout<<1<<endl;
	NetLibZ::CurrentThread::yieldCoroutine();
	cout<<3<<endl;
}

void Producer(){
	cout<<2<<endl;
	NetLibZ::CurrentThread::yieldCoroutine();
	cout<<4<<endl;
}

int main()
{
	CoroutinePtr consumer_routine = NetLibZ::CurrentThread::createCoroutine(std::bind(Consumer));
	consumer_routine->resume();

	CoroutinePtr producer_routine = NetLibZ::CurrentThread::createCoroutine(std::bind(Producer));
	producer_routine->resume();

	consumer_routine->resume();
	producer_routine->resume();

	
	return 0;
}
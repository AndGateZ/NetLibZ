#include <iostream>
#include <memory>
#include <benchmark/benchmark.h>
#include <sys/time.h>

#include "netLibZ/core/Coroutine.h"
#include "netLibZ/core/CurrentThread.h"
#include "netLibZ/core/ShareStack.h"

using namespace std;
using namespace NetLibZ::Core;

// int64_t get_current_time(){
//     timeval now;
//     int ret = gettimeofday(&now, NULL);
//     assert(ret != -1);
//     return now.tv_sec * 1000000 + now.tv_usec;//us
// }

// void run(){}

// void test()
// {
// 	Coroutine *co = new Coroutine(std::move(run));
// 	coctx_init(co->ctx());
// 	co->allocStackMemory(false);
// 	//delete(co);
// }

// int main(){
// 	printf("Coroutine: %ld\n",sizeof(Coroutine));
// 	int64_t start = get_current_time();
// 	for(int i = 0;i<100;++i) test();
// 	int64_t end = get_current_time();
// 	printf("100: %ldus\n",end-start);

// 	start = get_current_time();
// 	for(int i = 0;i<1000;++i) test();
// 	end = get_current_time();
// 	printf("1000: %ldus\n",end-start);

// 	start = get_current_time();
// 	for(int i = 0;i<30000;++i) test();
// 	end = get_current_time();
// 	printf("30000: %ldus\n",end-start);

// 	int a = 0;
// 	cin>>a;
// 	return 0;
// }

void test1(){
	for(int i = 0;i<30000;++i){
		NetLibZ::CurrentThread::yieldCoroutine();
	}
}


static void BM_demo(benchmark::State& state) {
	CoroutinePtr consumer_routine = NetLibZ::CurrentThread::createCoroutine(std::bind(test1));
	CoroutinePtr co2 = NetLibZ::CurrentThread::createCoroutine(std::bind(test1));
 	for (auto _ : state) {
		consumer_routine->resume();
		co2->resume();
	}
	
}

BENCHMARK(BM_demo)->Iterations(30000);
BENCHMARK_MAIN(); //程序入口

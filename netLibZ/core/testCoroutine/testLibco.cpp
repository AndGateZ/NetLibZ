#include <stdio.h>
#include <iostream>
#include <libco/co_routine.h>
#include <libco/co_routine_inner.h>
#include <benchmark/benchmark.h>
#include <sys/time.h>

using namespace std;

// int64_t get_current_time(){
//     timeval now;
//     int ret = gettimeofday(&now, NULL);
//     assert(ret != -1);
//     return now.tv_sec * 1000000 + now.tv_usec;//us
// }

// void* run(void* args)
// {
// 	return NULL;
// }

// static stShareStack_t* share_stack;

// void test(){
// 	stCoRoutineAttr_t attr;
// 	attr.stack_size = 0;
// 	attr.share_stack = share_stack;
// 	stCoRoutine_t* consumer_routine;
// 	co_create(&consumer_routine, &attr, run, NULL);
// 	//free(consumer_routine);
// }


// int main(){
// 	printf("stShareStack_t: %ld\n",sizeof(stCoRoutine_t));
// 	share_stack= co_alloc_sharestack(1, 1024 * 128);	//初始化
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
// }

static stShareStack_t* share_stack;
void* test1(void*){
	for(int i = 0;i<30000;++i){
		co_yield_ct();
	}
}


static void BM_demo(benchmark::State& state) {
	share_stack= co_alloc_sharestack(1, 1024 * 128);	//初始化
	stCoRoutineAttr_t attr;
	attr.stack_size = 0;
	attr.share_stack = share_stack;
	stCoRoutine_t* consumer_routine;
	co_create(&consumer_routine, &attr, test1, NULL);
	stCoRoutine_t* co2;
	co_create(&co2, &attr, test1, NULL);
 	for (auto _ : state) {
		co_resume(consumer_routine);
		co_resume(co2);
	}
	
}

// BENCHMARK(BM_demo)->Iterations(1);
BENCHMARK(BM_demo)->Iterations(30000);
BENCHMARK_MAIN(); //程序入口

//g++ testLibco.cpp -o testLibco -lcolib -pthread -ldl -lbenchmark
#include <libco/co_routine.h>
#include <iostream>

using namespace std;

void* Consumer(void* args)
{
	cout<<1<<endl;
	co_yield_ct();
	cout<<3<<endl;
	return NULL;
}

void* Producer(void* args)
{
	cout<<2<<endl;
	co_yield_ct();
	cout<<4<<endl;
	return NULL;
}


int main()
{

	stCoRoutine_t* consumer_routine;
	co_create(&consumer_routine, NULL, Consumer, NULL);
	co_resume(consumer_routine);

	stCoRoutine_t* producer_routine;
	co_create(&producer_routine, NULL, Producer, NULL);
	co_resume(producer_routine);

	co_resume(consumer_routine);
	co_resume(producer_routine);
	
	return 0;
}
//编译
//g++ testlibco.cc -lcolib -lpthread -ldl
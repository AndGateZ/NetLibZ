#include <iostream>

using namespace std;

void test1(){
	cout<<1<<endl;
}

void test2(){
	cout<<2<<endl;
}

int main(){
	void (*funcptr)() = test1;
	funcptr();
	funcptr = test2;
	funcptr();
	return 0;
}
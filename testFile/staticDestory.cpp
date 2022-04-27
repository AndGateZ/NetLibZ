#include <iostream>
using namespace std;

class A {
public:
	int ma;
	A (int tmp) : ma(tmp) {
		cout << "A::A(" << tmp << ")" << endl;
	}
	~A() {
		cout << ma<<" A::~A()" << endl;
	}
};

void func() {
	cout << "func() start... " << endl;
	static A a(1);
	cout << "func() end." << endl;
}

A g_a(10);
static A sg_a(20);

int main() {
	cout << "main() start" << endl;
	func();
	cout << "main() end" << endl;
    
    return 0;
}


//A::A(10)
//A::A(20)
//main() start
//func() start...
//A::A(1)
//func() end.
//main() end
//1 A::~A()
//20 A::~A()
//10 A::~A()

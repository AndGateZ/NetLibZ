#include <iostream>
using namespace std;
 

class Empty{};

class BaseV1{
public:
	virtual void func() = 0;
};

class BaseV2{
public:
	virtual void func(){} //虚函数必须有定义
};

class BaseV3{
public:
	virtual void func(){} //虚函数必须有定义
};

class DerivedV2:public BaseV2{
public:
	virtual void func(){}
};

class DerivedV2Plus:public BaseV2,public BaseV3{
public:
	virtual void func(){}
};

class BaseN1{
public:
	int func(){return 1;}
};



int main(){
	Empty empty;
	BaseN1 baseN1;
	DerivedV2 derivedV2;
	DerivedV2Plus derivedV2Plus;
	cout << "sizeof Empty " << sizeof(empty) << endl;
    cout << "sizeof BaseV1 " << sizeof(BaseV1) << endl;
    cout << "sizeof BaseV2 " << sizeof(BaseV2) << endl;
    cout << "sizeof BaseN1 " << sizeof(baseN1) << endl;
	cout << "sizeof DerivedV2 " << sizeof(derivedV2) << endl;
	cout << "sizeof DerivedV2Plus " << sizeof(derivedV2Plus) << endl;
	return 0;
}
//1. C++标准规定类的大小不为0，空类的大小为1
//2. 类的大小只与它的数据成员有关, 当类不包含虚函数和非静态数据成员时，其对象大小也为1。
//3. 虚函数表在常量区，虚函数在代码段
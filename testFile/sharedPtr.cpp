
#include <stdlib.h>
#include <iostream>

using namespace std;

template<typename T>
class MySharedPtr{
public:
	//默认构造
	MySharedPtr():ptr_(NULL),count_(new size_t(0)){ }
	//赋值构造
	MySharedPtr(T *ptr) :ptr_(ptr),count_(new size_t(1)){}
	//拷贝构造
	MySharedPtr(const MySharedPtr<T> &MSP){
		ptr_ = MSP.ptr_;
		count_ = MSP.count_;
		(*count_)++;
	}

	~MySharedPtr(){
		if(ptr_){
			(*count_)--;
			if((*count_) == 0){
				delete ptr_;
				delete count_;
			}
		}
	}

	//等号重载
	MySharedPtr& operator=(const MySharedPtr<T> &MSP){
		if(MSP.ptr_ == ptr_) return *this;
		//本指针已经被占用
		if(ptr_){
			(*count_)--;
			if((*count_) == 0){
				delete ptr_;
				delete count_;
			}
		}
		//被指针未被占用
		ptr_ = MSP.ptr_;
		(*count_)++;
		return *this;
	}


	//*号重载
	T& operator*(){
		return *ptr_;
	}
	//->重载
	T* operator->(){
		return ptr_;
	}

	size_t useCount() const {return (*count_);}

private:
	T* ptr_;
	size_t* count_;

};




int main(){

	MySharedPtr<int> p1(new int(1));
	MySharedPtr<int> p2(p1);
	MySharedPtr<int> p3 = p2;

	cout<<p1.useCount()<<endl;
	cout<<*p1<<endl;
	return  0;
}

// int main(){
// 	shared_ptr<int> test(new int(1));
// 	shared_ptr<int> test1;
// 	cout<<test.use_count()<<endl;
// 	cout<<endl;
// 	test1 = test;
// 	cout<<test.use_count()<<endl;
// 	cout<<test1.use_count()<<endl;
// 	cout<<endl;
// 	test1 = NULL;
// 	cout<<test.use_count()<<endl;
// 	cout<<test1.use_count()<<endl;
// 	return 0;
// }
//取消shared_ptr的引用后，其count--
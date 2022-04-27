#include <mutex>
#include <iostream>

class Singleton{

public:
	//双层check
	static Singleton * getInstance() {
		if(thing_ == NULL){
			std::lock_guard<std::mutex> locker(mutex_);
			if(thing_ == NULL){
				thing_ = new Singleton();
				return thing_; 
			}
		}
		else return thing_; 
    }

	static void deleteInstance() {
		if(thing_ != NULL){
			std::lock_guard<std::mutex> locker(mutex_);
			if(thing_ != NULL){
				delete thing_;
			}
		}
    }

	Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&) = delete;

private:
	Singleton() = default;
	~Singleton() = default;
    
	static Singleton* thing_;
	static std::mutex mutex_;
};

Singleton* Singleton::thing_ = NULL;//懒汉模式，第一次用到的时候才实例化
std::mutex Singleton::mutex_;

int main(){
	Singleton* thing1 = Singleton::getInstance();
	Singleton* thing2 = Singleton::getInstance();
	std::cout<< thing1<<std::endl;
	std::cout<< thing2<<std::endl;
	return 0;
}


// 全局只有一个实例：static 特性，同时禁止用户自己声明并定义实例（把构造函数设为 private） 禁止赋值和拷贝
// 线程安全
// 用户通过接口获取实例：使用 static 类成员函数

//单例模式直接用uniqueptr不就好了。。
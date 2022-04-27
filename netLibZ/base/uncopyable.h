#ifndef _NETLIBZ_BASE_UNCOPYABLE_H_
#define _NETLIBZ_BASE_UNCOPYABLE_H_

namespace NetLibZ{

class uncopyable{
public:
    uncopyable(const uncopyable&) = delete;//拷贝构造函数
    void operator=(const uncopyable&) = delete;//=号的操作符重载，返回类型是void

protected:
    uncopyable() = default;
    ~uncopyable() = default;
};

}


#endif
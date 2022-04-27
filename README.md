# NetLibZ
基于协程的网络编程框架，项目参考libco 
<br />

## 项目介绍  
___
### 协程库（参考libco）  
协程库采用非对称协程的方式设计，使用共享栈的形式保存运行时栈数据，此模块主要可分为4个部分  
  1. coctx模块--现场保护与上下文切换  
  coctx_t作为上下文载体，coctx_swap.S汇编实现程序跳转
  2. CurrentThread -- 线程局部空间  
  提供该线程的协程环境、共享栈空间管理，当需要在当前线程创建协程的时候，提供协程环境和共享栈初始化、内存分配的功能
  3. ShareStack -- 位于堆的共享栈空间  
     * StackMemory：内存空间封装，以智能指针来维护声明周期 
     * ShareStacks：管理分配所有内存空间的对象，作为线程局部变量为当前线程的协程服务  
  4. Coroutine -- 协程  
     * CoroutineEnv：线程的协程环境类
     * Coroutine：协程类    
  
详见：[协程库](https://github.com/AndGateZ/NetLibZ/blob/main/netLibZ/record/%E5%8D%8F%E7%A8%8B%E5%BA%93.md)
<br/>
<br/>

### hook模块
hook模块使得不具备异步功能的api，尤其是socket的相关底层api，具有异步性能

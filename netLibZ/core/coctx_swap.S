//from libco

.globl coctx_swap
#if !defined( __APPLE__ )
.type  coctx_swap, @function
#endif
coctx_swap:

#if defined(__i386__)
    movl 4(%esp), %eax
    movl %esp,  28(%eax)
    movl %ebp, 24(%eax)
    movl %esi, 20(%eax)
    movl %edi, 16(%eax)
    movl %edx, 12(%eax)
    movl %ecx, 8(%eax)
    movl %ebx, 4(%eax)


    movl 8(%esp), %eax
    movl 4(%eax), %ebx
    movl 8(%eax), %ecx
    movl 12(%eax), %edx
    movl 16(%eax), %edi
    movl 20(%eax), %esi
    movl 24(%eax), %ebp
    movl 28(%eax), %esp

	ret

#elif defined(__x86_64__)		//rdi：当前上下文地址  rsi：目标上下文地址  此时sp指向返回地址，sp上面是参数，bp仍为上一个栈帧的栈底
	leaq (%rsp),%rax			//当前rsp指向的内容是原代码的下一条代码的地址，即返回地址，ax = sp的地址
    movq %rax, 104(%rdi)		//将通用寄存器存入当前ctx寄存器  此时ax = sp地址  104存储栈顶sp地址
    movq %rbx, 96(%rdi)			
    movq %rcx, 88(%rdi)
    movq %rdx, 80(%rdi)
	  movq 0(%rax), %rax		//ax寄存器的值 = 下一条代码的地址
	  movq %rax, 72(%rdi) 		//rdi 72存储下一条代码的地址
    movq %rsi, 64(%rdi)			//rdi 64存储目标协程指针，但不知道有什么用
	  movq %rdi, 56(%rdi)		//rdi 56存储自己的协程指针，但不知道有什么用
    movq %rbp, 48(%rdi)
    movq %r8, 40(%rdi)
    movq %r9, 32(%rdi)
    movq %r12, 24(%rdi)
    movq %r13, 16(%rdi)
    movq %r14, 8(%rdi)
    movq %r15, (%rdi)
	  xorq %rax, %rax

    movq 48(%rsi), %rbp			//将待切换的ctx寄存器内容赋值给通用寄存器
    movq 104(%rsi), %rsp		//恢复sp，sp指向si的返回地址，即目标上下文的下一条语句
    movq (%rsi), %r15
    movq 8(%rsi), %r14
    movq 16(%rsi), %r13
    movq 24(%rsi), %r12
    movq 32(%rsi), %r9
    movq 40(%rsi), %r8
    movq 56(%rsi), %rdi
    movq 80(%rsi), %rdx
    movq 88(%rsi), %rcx
    movq 96(%rsi), %rbx
		leaq 8(%rsp), %rsp		//栈顶指针上移8个字节，覆盖掉返回地址，相当于对返回地址进行出栈操作
		pushq 72(%rsi)			//寄存器72存储返回函数地址，在coctx_make初始化好了，下一条代码的地址入栈，伪造返回地址

    movq 64(%rsi), %rsi			
	ret
#endif
列出调用堆栈	kb	调试.列出调用堆栈
转换为小写	Lcase	编辑.转换为小写
转到	GotoLn	编辑.转到
转到大括号	GotoBrace	编辑.转到大括号
启动	g	调试.启动
逐过程	p	调试.逐过程
逐语句	t	调试.逐语句
停止	q	调试.停止调试
跳出	pr	调试.跳出
运行到光标处	rtc	调试.运行到光标处
“监视”窗口	Watch	调试.监视N
“调用堆栈”窗口	CallStack	调试.调用堆栈
快速监视	??	调试.快速监视
“寄存器”窗口	registers	调试.寄存器
“反汇编”窗口	disasm	调试.反汇编
列出反汇编	u	调试.列出反汇编
列出线程	~	调试.列出线程
计算语句	eval	调试.计算语句 --列出值
属性窗口	props	视图.属性窗口
外壳程序	shell	工具.外壳程序

来源：https://www.cnblogs.com/wucg/archive/2010/08/05/1792816.html
参考：http://www.goodgoodhack.com/a/chengxubiancheng/2430.html



内存泄漏检测机制
http ://www.cctry.com/thread-243581-1-1.html
eg:
#define _CRTDBG_MAP_ALLOC  
#include<stdlib.h>
#include<crtdbg.h>  
Watch窗口-->_crtBreakAlloc
代码起始--> _CrtSetBreakAlloc(内存错误地址);

vs2017:--https ://www.cnblogs.com/luruiyuan/p/6916634.html
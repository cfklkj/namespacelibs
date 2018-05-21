
初始化锁
CRITICAL_SECTION para;
InitializeCriticalSection(&para);

DeleteCriticalSection(&para);

//使用
EnterCriticalSection(&para);
。。。
LeaveCriticalSection(&para);


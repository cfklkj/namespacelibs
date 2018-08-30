#include <stdio.h>
#include <Windows.h> 
#include <iostream>
#include <functional> 

using namespace std;
//#pragma comment(lib, "User32")
 
#define DemoAsm
#ifdef DemoAsm
void main()
#else
void DemoAsm()
#endif // DEBUG
{ 
	unsigned int a;
	char inputKey;
	cout << "输入一个整数：" << endl;
	cin >> a;

	unsigned int *c = &a;
	//__asm
	//{
	//	mov eax, c; //c中存储的a的地址->eax 
	//	mov eax, [eax]; //a的值->eax 
	//	add eax, 1;
	//	mov a, eax;
	//}
	//cout << "+ 1 = " << a << endl;
	//cin >> inputKey;
	return ;

}

 
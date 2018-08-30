#include <stdio.h>
#include <Windows.h>
#include "..\include\fly_web.h" 
#include "..\include\fly_string.h" 
#pragma comment(lib, "..\\libs\\debug\\flyLibs")
//#pragma comment(lib, "User32")
#include <time.h>


int DiffLocalTime(const std::string strTime) //时间字符串格式 2017-08-23_11:25:30
{
	tm tm_;
	int year, month, day, hour = 0, minute = 0, second = 0;
	sscanf(strTime.c_str(), "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);
	tm_.tm_year = year - 1900;
	tm_.tm_mon = month - 1;
	tm_.tm_mday = day;
	tm_.tm_hour = hour;
	tm_.tm_min = minute;
	tm_.tm_sec = second;
	tm_.tm_isdst = 0;

	time_t t_ = mktime(&tm_); //已经减了8个时区  

	time_t  t_now;
	t_now = time(NULL); 
	int nSecond = (int)(t_now - t_); 
	if (nSecond > 0)
		return nSecond;
	else
		return 0;
}
void in(int& A, std::string *B)
{
	A = 5;
	*B = "Sdf:";
}
//http://www.goodgoodhack.com/a/chengxubiancheng/2456.html
void EnumModule()
{
	DWORD *PEB = NULL,
		*Ldr = NULL,
		*Flink = NULL,
		*p = NULL,
		*BaseAddress = NULL,
		*FullDllName = NULL;
	__asm
	{
		mov eax,fs:[0x30] //peb address
		mov PEB,eax
	}
	Ldr = *((DWORD**)((unsigned char*)PEB + 0x0c));
	Flink = *((DWORD**)((unsigned char*)Ldr + 0x14));
	p = Flink;
	p = *((DWORD**)p); //指针的指针 --  p =	p->next
	while (Flink != p)
	{
		BaseAddress = *((DWORD**)((unsigned char*)p + 0x10));
		FullDllName = *((DWORD**)((unsigned char*)p + 0x20));
		if (BaseAddress == 0) break;
		wprintf(L"ImageBase = %08x \r\n modlefullname=%s \r\n", BaseAddress, (unsigned char*)FullDllName);
		p = *((DWORD**)p);
	}
}
#define test1
#ifdef test
void main()
#else
void test()
#endif // DEBUG
{ 
	EnumModule();
	getchar();
	printf("%d", DiffLocalTime("2018-5-10 10:11:12"));
	int a = 10;
	std::string b = "dd";
	in(a, &b);
	getchar();
}

 
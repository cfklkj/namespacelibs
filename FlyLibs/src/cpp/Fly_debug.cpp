#include "Fly_debug.h"
#include "FlyDefine.h"
#include <Windows.h> 
#include <string>
#pragma warning(disable:4996)
#pragma comment(lib, "User32")
 
//----------------------------------------------------------------�ַ���-----------------
namespace Fly_debug{   
	//��ʾ��Ϣ
	void Print(const char* format, ...)
	{
#ifdef _DEBUG
		std::string prf = "";
		_FLY_STRING_FindBitSub_FORMAT(format, prf);
		printf("%s\n", prf.c_str());
#endif 
	}
}
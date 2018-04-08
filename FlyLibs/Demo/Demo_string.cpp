#include <stdio.h>
#include "..\include\Fly_string.h"
#include "..\include\FlyDefine.h" 
#pragma comment(lib, "..\\libs\\debug\\flyLibs")
//#pragma comment(lib, "User32")
#define DemoString1
#ifdef DemoString
void main()
#else
void DemoString()
#endif // DEBUG
{	 
	char temp[] = "--{\"ABC|DEFG|HIJK\"}--";
	std::string str = Fly_string::StrToX(temp, true);
	printf("s:%s---d:%s\n", temp, str.c_str());
	char *mcTemp = Fly_string::strMalloc(str.c_str());
	printf("mcTemp:%s\n", mcTemp);
	char* infoA = NULL;
	char* infoEnd = NULL;
	_FLY_STRRING_GetSub(infoA, temp, '\"'); //¿ªÍ·
	printf("infoA:%s\n", infoA);
	_FLY_STRRING_GetSub(infoEnd, infoA, '\"');  //½áÎ²
	printf("infoEnd:%s\n", infoEnd);
	printf("infoA:%s\n", infoA);
	*infoEnd = 0;
	infoEnd++; 
	printf("infoEnd:%s\n", infoEnd);
	str = Fly_string::GetSubStr(infoA, '|', 1);
	printf("str:%s\n", str.c_str());
	str = Fly_string::GetSubStr(infoA, '|', 2);
	printf("str:%s\n", str.c_str());
	Fly_string::freeChar(mcTemp);
	getchar();
}

 
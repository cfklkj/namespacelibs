#include <stdio.h>
#include <string.h>
#include <time.h>
#include <iostream>
#include <tuple>
  
#include "D:\document\me\namespacelibs\FlyLibs\include\Fly_CwriteLog.h"
#pragma comment(lib, "D:\\document\\me\\namespacelibs\\FlyLibs\\libs\\Debug\\FlyLibs.lib")

void main1()
{
	CWriteLog log;
	log.open(".\\test.log");
	log.writeLine("Hellolsdkfjsldf");
	log.close();
}
 


void main() { 

	wchar_t szNickName[] = L"测试代码123🐢123"; 
	std::string nickName = "";
	std::string szNickName2 = "";
	szNickName2.append((char*)szNickName, sizeof(szNickName));
	int step = 0;
	for (auto item : szNickName2)
	{
		if (item == 0)
		{
			nickName.push_back(item);
			break; 
		}
		else
		{
			step = 0;
		}

		nickName.push_back(item);
	}
	std::wstring nickNameW = L"";
	int value = 0, newValue = 0;
	step = 0;
	for (auto item : nickName)
	{
		if (step)
		{
			step = 0; 
			if (value > 0)
			{
				newValue = (item << 8) | value;
			}
			else
			{
				newValue = ((1+~item) << 8) | (1 + ~value);
			//	newValue = (item >> 8) | (value);
			}
			nickNameW.push_back(newValue);
		}
		else
		{
			value = item;
			step++;
		}
	} 
	printf(nickName.c_str());
	getchar();
}
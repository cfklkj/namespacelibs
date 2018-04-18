#include <stdio.h>
#include <Windows.h>
#include "..\include\fly_reg.h" 
#include "..\include\fly_string.h" 
#pragma comment(lib, "..\\libs\\debug\\flyLibs")


#include "Shlwapi.h"
#pragma comment(lib, "Shlwapi")
 
#define DemoReg
//关联程序
std::string RegWithExe(std::string& exePath, std::string protoName);

#ifdef DemoReg
void main()
#else
void DemoReg()
#endif // DEBUG
{  
	std::string strKey = "";
	std::string strKeys = Fly_reg::GetKeys(HKEY_CURRENT_USER, strKey);
	printf("%s\n", strKeys.c_str());
	for (int i = 1; ; i++)
	{
		std::string keys = Fly_string::GetSubStr(strKeys.c_str(), Fly_reg::GetKeysBreak().c_str(), i);
		if (keys.empty())
			break;
		printf("%s\n", keys.c_str());
	}

	strKey = "Environment";
	strKeys = Fly_reg::GetNames(HKEY_CURRENT_USER, strKey);
	printf("%s\n", strKeys.c_str());
	for (int i = 1; ; i++)
	{
		std::string keys = Fly_string::GetSubStr(strKeys.c_str(), Fly_reg::GetKeysBreak().c_str(), i);
		if (keys.empty())
			break;
		printf("%s\n", keys.c_str());
	}

	strKey = "Software\\Microsoft\\Windows\\CurrentVersion\\Themes";
	std::string strName = "InstallTheme";
	strKeys = Fly_reg::GetValue(HKEY_CURRENT_USER, strKey, strName);

	printf("values--%s\n", strKeys.c_str());

	strName = "D:\\soft_bag\\Me\\exe\\myrun.exe";
	RegWithExe(strName, "MyRun");
	printf("MyRun--%s\n", strName.c_str());
	getchar();
}

//关联程序
std::string RegWithExe(std::string& exePath, std::string protoName)
{
	std::string key = Fly_string::format("%s\\shell\\open\\command", protoName.c_str());
	std::string rstStr = Fly_reg::GetValue(HKEY_CLASSES_ROOT, key, "");
	if (rstStr.empty())
	{
		HKEY KEY;
		HKEY hkeyCR = HKEY_CURRENT_USER;
		std::string key = Fly_string::format("Software\\Classes\\%s\\shell\\open\\command", protoName.c_str());
		if (ERROR_SUCCESS == RegCreateKeyA(hkeyCR, key.c_str(), &KEY))
		{
			std::string value = Fly_string::format("\"%s\" \"%%1\"", exePath.c_str());
			SHSetValueA(hkeyCR, key.c_str(), NULL, REG_SZ, value.c_str(), value.size());

			key = Fly_string::format("Software\\Classes\\%s\\DefaultIcon", protoName.c_str());
			RegCreateKeyA(hkeyCR, key.c_str(), &KEY);
			SHSetValueA(hkeyCR, key.c_str(), NULL, REG_SZ, exePath.c_str(), exePath.size());
			key = Fly_string::format("Software\\Classes\\%s", protoName.c_str());
			value = Fly_string::format("URL: %s Protocol", protoName.c_str());
			SHSetValueA(hkeyCR, key.c_str(), NULL, REG_SZ, value.c_str(), value.size());
			SHSetValueA(hkeyCR, key.c_str(), "URL Protocol", REG_SZ, "", strlen(""));
			//重新获取
			key = Fly_string::format("%s\\shell\\open\\command", protoName.c_str());
			rstStr = Fly_reg::GetValue(HKEY_CLASSES_ROOT, key, "");
		}
	}
	return rstStr;
}
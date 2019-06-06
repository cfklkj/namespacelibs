#pragma once
#include <Windows.h>
#include <queue>  

namespace Fly_reg{ 
		/*
		以下使用系统api   Shlwapi.h  Shlwapi.lib
		SHSetValue   //添加 写入
		SHDeleteValue  //删除

		RegCreateKey
		RegDeleteKey
		*/
		//查询--- 
		//遍历项名称
		//返回 SubKeysStr
		std::string GetKeys(HKEY rootKey, std::string subKey); 
		std::string GetKeysBreak();

		//遍历所属项名称
		std::string GetNames(HKEY rootKey, std::string subKey);
		std::string GetValue(HKEY rootKey, std::string subKey, std::string subName);  
}
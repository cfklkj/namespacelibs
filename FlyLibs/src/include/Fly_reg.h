#pragma once
#include <Windows.h>
#include <queue>  

namespace Fly_reg{ 
		/*
		����ʹ��ϵͳapi   Shlwapi.h  Shlwapi.lib
		SHSetValue   //��� д��
		SHDeleteValue  //ɾ��

		RegCreateKey
		RegDeleteKey
		*/
		//��ѯ--- 
		//����������
		//���� SubKeysStr
		std::string GetKeys(HKEY rootKey, std::string subKey); 
		std::string GetKeysBreak();

		//��������������
		std::string GetNames(HKEY rootKey, std::string subKey);
		std::string GetValue(HKEY rootKey, std::string subKey, std::string subName);  
}
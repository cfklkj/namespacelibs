#include "Fly_debug.h"
#include "Fly_reg.h"
#include "Fly_string.h"
#include "FlyDefine.h"
#pragma comment(lib, "Advapi32")

#include  <locale.h> //œ‘ æwstring ÷–Œƒ

namespace Fly_reg {

	DWORD dwType = REG_BINARY | REG_DWORD | REG_EXPAND_SZ | REG_MULTI_SZ | REG_NONE | REG_SZ;

#define FLY_REG_BREAK "|*-_-*|"
	const int MAX_KEY_LENGTH = 255;
	const int MAX_VALUE_NAME = 16383;
	//////////////////////////////////////---------------------------------------------- 
	std::string GetKeysBreak()
	{
		return FLY_REG_BREAK;
	} 
	std::string GetKeys(HKEY rootKey, std::string subKey)
	{
		bool isQuery = false;
		HKEY hKey;
		if (RegOpenKeyEx(rootKey, subKey.c_str(), 0, KEY_READ, &hKey) != ERROR_SUCCESS) //KEY_READ
		{
			RegCloseKey(hKey);
			return false;
		}
		std::string retStr = "";
		TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name    
		DWORD    cbName;                   // size of name string     
		TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name     
		DWORD    cchClassName = MAX_PATH;  // size of class string     
		DWORD    cSubKeys = 0;               // number of subkeys     
		DWORD    cbMaxSubKey;              // longest subkey size     
		DWORD    cchMaxClass;              // longest class string     
		DWORD    cValues;              // number of values for key     
		DWORD    cchMaxValue;          // longest value name     
		DWORD    cbMaxValueData;       // longest value data     
		DWORD    cbSecurityDescriptor; // size of security descriptor     
		FILETIME ftLastWriteTime;      // last write time     

		DWORD i, retCode;

		DWORD cchValue = MAX_VALUE_NAME;

		// Get the class name and the value count.     
		retCode = RegQueryInfoKey(
			hKey,                    // key handle     
			achClass,                // buffer for class name     
			&cchClassName,           // size of class string     
			NULL,                    // reserved     
			&cSubKeys,               // number of subkeys     
			&cbMaxSubKey,            // longest subkey size     
			&cchMaxClass,            // longest class string     
			&cValues,                // number of values for this key     
			&cchMaxValue,            // longest value name     
			&cbMaxValueData,         // longest value data     
			&cbSecurityDescriptor,   // security descriptor     
			&ftLastWriteTime);       // last write time     

		// Enumerate the subkeys, until RegEnumKeyEx fails.    
		if (cSubKeys)
		{
			for (i = 0; i < cSubKeys; i++)
			{
				cbName = MAX_KEY_LENGTH;
				retCode = RegEnumKeyEx(hKey, i,
					achKey,
					&cbName,
					NULL,
					NULL,
					NULL,
					&ftLastWriteTime);
				if (retCode == ERROR_SUCCESS)
				{
					if (!retStr.empty())
						retStr += FLY_REG_BREAK;
					retStr += achKey;
				}
			}
		}
		RegCloseKey(hKey);
		return retStr;
	}
	std::string GetNames(HKEY rootKey, std::string subKey)
	{
		bool isQuery = false;
		HKEY hKey;
		if (RegOpenKeyEx(rootKey, subKey.c_str(), 0, KEY_READ, &hKey) != ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			return false;
		}

		std::string retStr = "";
		TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name    
		DWORD    cbName;                   // size of name string     
		TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name     
		DWORD    cchClassName = MAX_PATH;  // size of class string     
		DWORD    cSubKeys = 0;               // number of subkeys     
		DWORD    cbMaxSubKey;              // longest subkey size     
		DWORD    cchMaxClass;              // longest class string     
		DWORD    cValues;              // number of values for key     
		DWORD    cchMaxValue;          // longest value name     
		DWORD    cbMaxValueData;       // longest value data     
		DWORD    cbSecurityDescriptor; // size of security descriptor     
		FILETIME ftLastWriteTime;      // last write time     

		DWORD i, retCode;

		TCHAR  achValue[MAX_VALUE_NAME];
		DWORD cchValue = MAX_VALUE_NAME;

		// Get the class name and the value count.     
		retCode = RegQueryInfoKey(
			hKey,                    // key handle     
			achClass,                // buffer for class name     
			&cchClassName,           // size of class string     
			NULL,                    // reserved     
			&cSubKeys,               // number of subkeys     
			&cbMaxSubKey,            // longest subkey size     
			&cchMaxClass,            // longest class string     
			&cValues,                // number of values for this key     
			&cchMaxValue,            // longest value name     
			&cbMaxValueData,         // longest value data     
			&cbSecurityDescriptor,   // security descriptor     
			&ftLastWriteTime);       // last write time     
		// Enumerate the subkeys, until RegEnumKeyEx fails.  
		if (cValues)
		{
			// Enumerate the key values.   
			for (i = 0, retCode = ERROR_SUCCESS; i < cValues; i++)
			{
				cchValue = MAX_VALUE_NAME;
				achValue[0] = '\0';
				unsigned char vari[70];
				retCode = RegEnumValue(hKey, i,
					achValue,
					&cchValue,
					NULL,
					NULL,
					NULL,
					NULL);
				if (retCode == ERROR_SUCCESS)
				{
					if (!retStr.empty())
						retStr += FLY_REG_BREAK;
					retStr += achValue;
				}
			}
		}
		RegCloseKey(hKey);
		return retStr;

	}
	std::string GetValue(HKEY rootKey, std::string subKey, std::string subName)
	{
		bool isQuery = false;
		TCHAR szBuffer[255] = { 0 };
		DWORD dwNameLen = 255;
		HKEY hKey;
		if (RegOpenKeyEx(rootKey, subKey.c_str(), 0, KEY_READ, &hKey) != ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			return "";
		}
		std::string rstStr = "";
		DWORD rQ = RegQueryValueEx(hKey, subName.c_str(), 0, &dwType, (LPBYTE)szBuffer, &dwNameLen);
		if (rQ == ERROR_SUCCESS)
		{
			isQuery = true; 
			switch (dwType)
			{
			case REG_NONE:
			case REG_BINARY:
			{
				for (int i = 0; i < dwNameLen; i++)
					rstStr.push_back(szBuffer[i]);
			}break;
			case REG_DWORD:
			{
				DWORD dValue = 0;
				RegQueryValueEx(hKey, subName.c_str(), 0, &dwType, (LPBYTE)&dValue, &dwNameLen);
				rstStr = Fly_string::format("%d", dValue);
			}break;
			case REG_EXPAND_SZ:
			case REG_MULTI_SZ:
			case REG_SZ:
				rstStr = szBuffer;
				break;
			}
		}
		//release.  
		RegCloseKey(hKey);
		return rstStr;

	} 
} 
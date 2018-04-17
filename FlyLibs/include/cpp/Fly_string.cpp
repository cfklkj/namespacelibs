#include "..\Fly_string.h"
#include "..\FlyDefine.h"
#include <Windows.h> 
#include <string>
#pragma warning(disable:4996)
#pragma comment(lib, "User32")
 
//----------------------------------------------------------------字符串-----------------
namespace Fly_string{  
	
	char HexChar(char c)
	{
		if ((c >= '0') && (c <= '9'))
			return c - 0x30;
		else if ((c >= 'A') && (c <= 'F'))
			return c - 'A' + 10;
		else if ((c >= 'a') && (c <= 'f'))
			return c - 'a' + 10;
		else
			return 0x10;
	}
	//释放
	void freeWChar(wchar_t *pwstr)
	{
		if (pwstr) delete[] pwstr;
	}
	//释放
	void freeChar(char *pstr)
	{
		if (pstr) delete[] pstr; 
	}
	void freeUChar(unsigned char *pstr)
	{
		if (pstr) delete[] pstr; 
	}
	//wchar to char
	std::string w2c(const wchar_t *pwstr)
	{
		std::string rstStr = "";
		if (pwstr)
		{
			size_t nlength = wcslen(pwstr);

			size_t nL = (size_t)WideCharToMultiByte(0, 0, pwstr, nlength, NULL, 0, NULL, NULL); 

			if (nL < 1)
				return rstStr;

			char* str = new char[nL + 1];
			memset(str, 0, nL + 1);

			int n2 = WideCharToMultiByte(0, 0, pwstr, nlength, str, nL, NULL, NULL);

			str[nL] = 0;

			rstStr = str;

			freeChar(str); 

		}
		return rstStr;
	} 
	//char to wchar
	std::wstring c2w(const char *pcstr)
	{
		std::wstring rstStr = L"";
		if (pcstr)
		{
			size_t nu = strlen(pcstr);

			size_t nL = (size_t)MultiByteToWideChar(CP_ACP, 0, (const char *)pcstr, (int)nu, NULL, 0);

			if (nL < 1)
				return rstStr;

			wchar_t * pwstr = new wchar_t[nL + 1];
			memset(pwstr, 0, nL + 1);

			MultiByteToWideChar(CP_ACP, 0, (const char *)pcstr, (int)nu, pwstr, (int)nL);

			pwstr[nL] = 0;

			rstStr = pwstr;

			freeWChar(pwstr);

		}
		return rstStr;
	} 
	// ------------ UTF-8转Unicode再转单字节，字符串 ----------------   
	std::string Utf8ToGBK(const char *szUtf8)
	{
		std::string rstStr = "";
		if (szUtf8)
		{
			int len = MultiByteToWideChar(CP_UTF8, 0, szUtf8, -1, NULL, 0);
			WCHAR* wszUCD = new WCHAR[len];
			memset(wszUCD, 0, len);
			MultiByteToWideChar(CP_UTF8, 0, szUtf8, -1, wszUCD, len);

			//获取缓冲区的大小，并申请空间，缓冲区大小是按字节计算的

			int len2 = WideCharToMultiByte(CP_ACP, 0, wszUCD, -1, NULL, 0, NULL, NULL);

			char *buffer = new char[len2];

			WideCharToMultiByte(CP_ACP, 0, wszUCD, len, buffer, len2, NULL, NULL);

			freeWChar(wszUCD);
			rstStr = buffer;
			freeChar(buffer); 
		}
		return rstStr;
	}
	//GB2312到UTF-8的转换
	std::string G2U(const char* gb2312)
	{
		std::string rstStr = "";
		if (gb2312)
		{
			int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
			wchar_t* wstr = new wchar_t[len + 1];
			memset(wstr, 0, len + 1);
			MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
			len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
			char* str = new char[len + 1];
			memset(str, 0, len + 1);
			WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
			freeWChar(wstr);
			rstStr = str;
			freeChar(str);
		}
		return rstStr;
	}
	std::string G2Ustr(const char* gb2312)
	{
		std::string rstStr = "";
		if (gb2312)
		{
			int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
			wchar_t* wstr = new wchar_t[len + 1];
			wchar_t* Ustr = new wchar_t[len * 4 + 1];
			memset(wstr, 0, len + 1);
			memset(Ustr, 0, len * 4 + 1);
			MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
			len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);

			for (int i = 0; i < lstrlenW(wstr); i++)
			{
				WCHAR temp[8];
				if (wstr[i] >= 0 && wstr[i] <= 127)
				{
					temp[0] = wstr[i];
					temp[1] = 0;
					lstrcatW(Ustr, temp);
				}
				else
				{
					wsprintfW(temp, L"\\u%04hx", wstr[i]);
					lstrcatW(Ustr, temp);
				}
			}
			char* str = new char[len * 4 + 1];
			memset(str, 0, len * 4 + 1);
			WideCharToMultiByte(CP_UTF8, 0, Ustr, -1, str, len * 4, NULL, NULL);
			freeWChar(Ustr);
			freeWChar(wstr);
			rstStr = str;
			freeChar(str);
		}
		return rstStr;
	} 
	//"FA 13 24"  转字符串
	std::string Str2Hex(const char* str)
	{
		std::string rstStr = "";
		if (str)
		{
			int t, t1;
			int rlen = 0, len = strlen(str);

			for (int i = 0; i < len;)
			{
				char l, h = str[i];
				if (h == ' ')
				{
					i++;
					continue;
				}
				i++;
				if (i >= len)
					break;

				l = str[i];
				t = HexChar(h);
				t1 = HexChar(l);
				if ((t == 16) || (t1 == 16))
					break;
				else
					t = t * 16 + t1;
				i++;
				rstStr += (char)t;
			}
		}
		return rstStr;
	}


	//字符串转小写
	/*  --内部原理
	char A[] = "ABCdef";
	for (int i = 0; i < 3; i++) //转小写
	A[i] |= 0x20;
	for (int i = 3; i < 6; i++) //转大写
	A[i] &= 0xDF;     //~(0x20)
	*/
	std::string StrToX(char* pcstr, bool isToLower)
	{ 
		//_wcslwr _wcsupr
		return isToLower ? _strlwr(pcstr) : _strupr(pcstr);
	}

	//定位到下一个字符地址
	std::string pToStr(char* pwzStr, char wChr, bool isLast)
	{
		//wcschr wcsrchr
		return !isLast ? strchr(pwzStr, wChr) : strrchr(pwzStr, wChr);
	}	
	//提取字符串
	std::string GetSubStr(const char* pwzStr, char wChr, int index)
	{
		if (!pwzStr)
			return "";
		const char * wtemp = pwzStr;
		while (*pwzStr++ != wChr && *pwzStr != '\0');
		if (*pwzStr != '\0' && index > 1)
			return GetSubStr(pwzStr, wChr, --index);
		std::string rst = "";
		if (index > 1)
			return rst;
		while (*wtemp != wChr && *wtemp != '\0')
			rst += *wtemp++;
		return rst;
	}
	//字符串查找 
	bool FindSub(const char* Fullstr, const char* subStr)
	{
		const char* temp = subStr;
		while (*Fullstr != 0 && *temp != 0)
		{
			if (*Fullstr++ == *temp)
			{
				temp++;
			}
			else
				temp = subStr;
		}
		return *temp == 0;
	}
	//查找字符串返回指针
	std::string pFindSub(const char* Fullstr, const char* subStr)
	{
		const char* temp = subStr;
		while (*Fullstr != 0 && *temp != 0)
		{
			if (*Fullstr++ == *temp)
			{
				temp++;
			}
			else
				temp = subStr;
		}
		if (*temp == 0)
			return Fullstr;
		else
			return "";
	}
	//拷贝字符串
	char* strMalloc(const char* _source)
	{
		if (!_source)
			return false;
		int lenth = 0;
		const char * wtemp = _source;
		while (*(wtemp + lenth) != 0)
		{
			lenth++;
		}
		lenth += 2;
		char *mcBuff = new char[lenth];
		char *cmdStr = mcBuff;
		while (*wtemp != 0)
		{
			*cmdStr = *wtemp;
			cmdStr++;
			wtemp++;
		}
		*cmdStr = 0;
		return mcBuff;
	} 
}
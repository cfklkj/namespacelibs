#include "Fly_string.h"
#include "FlyDefine.h"
#include <Windows.h> 
#include <string>
#include <time.h>
#pragma warning(disable:4996)
#pragma comment(lib, "User32")
 
//----------------------------------------------------------------�ַ���-----------------
namespace Fly_string{
	namespace makeStr {

		//--����GUID
		std::string generateGuid()
		{
			static char buf[64] = { 0 };
			GUID guid;
			if (S_OK == ::CoCreateGuid(&guid))
			{
				_snprintf(buf, sizeof(buf)
					, "%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X"
					, guid.Data1
					, guid.Data2
					, guid.Data3
					, guid.Data4[0], guid.Data4[1]
					, guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5]
					, guid.Data4[6], guid.Data4[7]
				);
			}
			return (const char*)buf;
		}
	}
	
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
	//�ͷ�
	void freeWChar(wchar_t *pwstr)
	{
		if (pwstr) delete[] pwstr;
	}
	//�ͷ�
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
	// ------------ UTF-8תUnicode��ת���ֽڣ��ַ��� ----------------   
	std::string Utf8ToGBK(const char *szUtf8)
	{
		std::string rstStr = "";
		if (szUtf8)
		{
			int len = MultiByteToWideChar(CP_UTF8, 0, szUtf8, -1, NULL, 0);
			WCHAR* wszUCD = new WCHAR[len];
			memset(wszUCD, 0, len);
			MultiByteToWideChar(CP_UTF8, 0, szUtf8, -1, wszUCD, len);

			//��ȡ�������Ĵ�С��������ռ䣬��������С�ǰ��ֽڼ����

			int len2 = WideCharToMultiByte(CP_ACP, 0, wszUCD, -1, NULL, 0, NULL, NULL);

			char *buffer = new char[len2];

			WideCharToMultiByte(CP_ACP, 0, wszUCD, len, buffer, len2, NULL, NULL);

			freeWChar(wszUCD);
			rstStr = buffer;
			freeChar(buffer); 
		}
		return rstStr;
	}
	//GB2312��UTF-8��ת��
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
	//"FA 13 24"  ת�ַ���
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


	//�ַ���תСд
	/*  --�ڲ�ԭ��
	char A[] = "ABCdef";
	for (int i = 0; i < 3; i++) //תСд
	A[i] |= 0x20;
	for (int i = 3; i < 6; i++) //ת��д
	A[i] &= 0xDF;     //~(0x20)
	*/
	std::string StrToX(char* pcstr, bool isToLower)
	{ 
		//_wcslwr _wcsupr
		return isToLower ? _strlwr(pcstr) : _strupr(pcstr);
	}

	//��λ����һ���ַ���ַ
	std::string pToStr(char* pwzStr, char wChr, bool isLast)
	{
		//wcschr wcsrchr
		return !isLast ? strchr(pwzStr, wChr) : strrchr(pwzStr, wChr);
	}	
	//��ȡ�ַ���
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
	std::string GetSubStr(const char* pwzStr, const char* pstr, int index)
	{
		if (!pwzStr)
			return ""; 
		std::string rst = pFindSub(pwzStr, pstr);
		if (index > 1)
			return GetSubStr(rst.c_str(), pstr, --index); 
		return pFindSub_retHead(pwzStr, pstr);
	}
	//�ַ������� 
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
	//�����ַ�������ָ��
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
	std::string pFindSub_retHead(const char* Fullstr, const char* subStr)
	{
		const char* temp = subStr;
		std::string rstStr = "";
		while (*Fullstr != 0 && *temp != 0)
		{
			rstStr.push_back(*Fullstr);
			if (*Fullstr++ == *temp)
			{
				temp++;
			}
			else
			{
				temp = subStr;
			}
		}
		if (*temp == 0)
			return rstStr.substr(0, rstStr.size() -strlen(subStr));
		else
			return rstStr;
	}
	std::string delChar(const char * pzStr, const char * Chrs, int where)
	{
		if (!pzStr || !Chrs)
			return "";
		if (where == 3)
		{
			std::string tmp = delChar(pzStr, Chrs, 1);
			return delChar(tmp.c_str(), Chrs, 2);
		}
		std::string rst = "";
		if (where == 1)
		{ 
			std::string tmp = "";
			tmp.push_back(*pzStr);
			if (!FindSub(Chrs, tmp.c_str()))
				rst.push_back(*pzStr);
			while (pzStr && *++pzStr != '\0')
			{  
				rst.push_back(*pzStr);
			}
		}
		else if (where == 2)
		{
			char tmpChar = *pzStr;
			while (pzStr && *pzStr != '\0')
			{
				tmpChar = *pzStr;
				std::string tmp = "";
				tmp.push_back(tmpChar);
				if (*++pzStr == '\0' && FindSub(Chrs, tmp.c_str()))
					break;
				rst.push_back(tmpChar);
			} 
		}
		else
		{
			while (pzStr && *pzStr != '\0')
			{
				std::string tmp = "";
				tmp.push_back(*pzStr);
				if (!FindSub(Chrs, tmp.c_str()))
					rst.push_back(*pzStr);
				pzStr++;
			}
		}
		return rst;
	}
	//�����ַ���
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

	std::string format(const char* formatStr, ...)
	{
		std::string rstStr = "";
		_FLY_STRING_FindBitSub_FORMAT(formatStr, rstStr);
		return rstStr;
	}
	//�ַ����ո���
	std::string trimEnd(std::string &str)//ȥ��string�Ŀո�
	{
		if (str.empty())
			return str;
		const std::string &delim = " \t";
		std::string r = str.erase(str.find_last_not_of(delim) + 1);
		return r.erase(0, r.find_first_not_of(delim));
	}
	std::string trim(std::string& str)
	{
		if (str.empty())
			return str;
		std::string ret;
		int first = 0;
		size_t last = str.size() - 1;


		while (str[first] == ' ') {
			first++;
			if (first == str.size()) {
				break;
			}

		}

		if (first == str.size()) {
			return "";
		}

		while (str[last] == ' ') {
			last--;
			if (last < 0) {
				break;
			}
		}
		return str.substr(first, last - first + 1);
	}

	int ToInt(const std::string& str) {
		return atoi(str.c_str());
	}

	void FormatAppend(std::string& data, const char* formatStr, ...) {
		std::string retStr = "";
		_FLY_STRING_FindBitSub_FORMAT(formatStr, retStr);
		data += retStr;
	}

	//char ת wchar
	std::wstring C2W(const std::string &charBuff)
	{
		int nL = MultiByteToWideChar(CP_ACP, 0, charBuff.c_str(), -1, NULL, 0);
		if (nL < 1)
			return NULL;
		WCHAR * wstr = new WCHAR[nL + 2];
		if (!wstr)
			return NULL;
		MultiByteToWideChar(CP_ACP, 0, charBuff.c_str(), -1, wstr, nL);
		std::wstring wstrOut = wstr;
		delete[]wstr;
		wstr = NULL;
		return wstrOut;
	}
	//wchar ת char
	std::string W2C(const std::wstring &wcharBuff)
	{
		int nL = WideCharToMultiByte(0, 0, wcharBuff.c_str(), (int)wcharBuff.size(), NULL, 0, NULL, NULL);
		if (nL < 1)
			return NULL;
		char* str = new char[nL + 1];
		if (!str)
			return NULL;
		WideCharToMultiByte(0, 0, wcharBuff.c_str(), (int)wcharBuff.size(), str, nL, NULL, NULL);
		str[nL] = 0;
		std::string strOut = str;
		delete[]str;
		str = NULL;
		return strOut;
	}

	//UnicodeתUtf8		 	 
	std::string unicode2Utf8(const std::wstring& unicode)
	{
		if (unicode.empty())
			return "";
		int len;
		len = WideCharToMultiByte(CP_UTF8, 0, unicode.c_str(), -1, NULL, 0, NULL, NULL);
		char * utf8 = new char[len + 1];
		if (!utf8)
		{
			return "";
		}
		WideCharToMultiByte(CP_UTF8, 0, unicode.c_str(), -1, utf8, len, NULL, NULL);
		std::string lastString = utf8;
		delete[]utf8;
		return lastString;
	}
	//1����GBKת����UTF8
	std::string GBKToUTF8(const std::string& strGBK)
	{
		if (strGBK.empty())
			return strGBK;
		std::string strOutUTF8 = unicode2Utf8(C2W(strGBK));
		strOutUTF8 = trimEnd(strOutUTF8);
		return strOutUTF8;
	}


	char* GBKToUTF8_char(const char* chGBK)
	{

		DWORD dWideBufSize = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)chGBK, -1, NULL, 0);

		wchar_t * pWideBuf = new wchar_t[dWideBufSize];

		wmemset(pWideBuf, 0, dWideBufSize);

		MultiByteToWideChar(CP_ACP, 0, (LPCSTR)chGBK, -1, pWideBuf, dWideBufSize); 

		DWORD dUTF8BufSize = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)pWideBuf, -1, NULL, 0, NULL, NULL); 

		char * pUTF8Buf = new char[dUTF8BufSize];

		memset(pUTF8Buf, 0, dUTF8BufSize);

		int lenth = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)pWideBuf, -1, pUTF8Buf, dUTF8BufSize, NULL, NULL);  

		delete[]pWideBuf; 

		return pUTF8Buf;

	}
	// ------------ UTF-8תUnicode��ת���ֽڣ��ַ��� ----------------   
	std::string UTF8ToGBK(const char* szUtf8)
	{
		int len = MultiByteToWideChar(CP_UTF8, 0, szUtf8, -1, NULL, 0);
		WCHAR* wszUCD = new WCHAR[len];
		memset(wszUCD, 0, len);
		MultiByteToWideChar(CP_UTF8, 0, szUtf8, -1, wszUCD, len);

		//��ȡ�������Ĵ�С��������ռ䣬��������С�ǰ��ֽڼ����

		int len2 = WideCharToMultiByte(CP_ACP, 0, wszUCD, -1, NULL, 0, NULL, NULL);

		char *buffer = new char[len2];

		WideCharToMultiByte(CP_ACP, 0, wszUCD, len, buffer, len2, NULL, NULL);

		std::string strTemp(buffer);
		delete[]wszUCD;
		return strTemp;
	}
	//2����UTF8ת����GBK
	std::string UTF8ToGBK(const std::string& strUTF8)
	{
		std::string strTemp = UTF8ToGBK(strUTF8.c_str());
		strTemp = trimEnd(strTemp);
		return strTemp;
	}
	//�����ִ�Сд�Ƚ��ַ����Ƿ����
	bool noCaseCompare(const char* A, const char* B)
	{
		bool isSame = true;
		while (*A != '\0' && *B != '\0')
		{
			char TA = *A++;
			char TB = *B++;
			if ((TA |= 0x20) != (TB |= 0x20))
			{
				isSame = false;
				break;
			}
		} 
		return !isSame ? isSame : *A == *B;
	}

	//�ַ����ָ�
	std::vector<std::string> splitEx(const std::string& src, std::string spX)
	{
		std::vector<std::string> strs;
		if (src.empty())
			return strs;
		int spLgth = (int)spX.size();												//�ָ��ַ����ĳ���,�����Ϳ���֧���硰,,�����ַ����ķָ���
		int lstPos = 0, index = -1;
		while (-1 != (index = (int)src.find(spX, lstPos)))
		{
			strs.push_back(src.substr(lstPos, index - lstPos));
			lstPos = index + spLgth;
		}
		std::string lastString = src.substr(lstPos);														//��ȡ���һ���ָ����������
		if (!lastString.empty())
			strs.push_back(lastString);																		//������һ���ָ����������ݾ����
		return strs;
	}
	bool splitEx(const std::string& src, std::string spX, std::vector<std::string>& rstVct)
	{
		if (src.empty())
			return false;
		rstVct = splitEx(src, spX);
		return !rstVct.empty();
	}
}
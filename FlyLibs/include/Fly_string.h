#pragma once 
#include <string>

//----------------------------------------------------------------字符串-----------------
namespace Fly_string {
	//wchar to char
	std::string w2c(const wchar_t *pwstr);
	//char to wchar
	std::wstring c2w(const char *pcstr);
	// ------------ UTF-8转Unicode再转单字节，字符串 ----------------   
	std::string Utf8ToGBK(const char *szUtf8);
	//GB2312到UTF-8的转换
	std::string G2U(const char* gb2312);
	//GB2312到UTF-8编码\uabcd的转换
	std::string G2Ustr(const char* gb2312); 
	//"FA 13 24"  转字符串
	std::string Str2Hex(const char* str);

	//字符串转大小写
	std::string StrToX(char* pcstr, bool isToLower);

	//定位到下一个字符地址
	std::string pToStr(char* pwzStr, char wChr, bool isLast = false);
	//提取字符串
	std::string GetSubStr(const char* pwzStr, char wChr, int index);
	//字符串查找 
	bool FindSub(const char* Fullstr, const char* subStr);
	//查找字符串返回指针
	std::string pFindSub(const char* Fullstr, const char* subStr);
	//拷贝字符串
	char* strMalloc(const char* _source);
	//释放
	void freeChar(char *pstr); 
}  
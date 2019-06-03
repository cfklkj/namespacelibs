#pragma once 
#include <string>
#include <vector>

//----------------------------------------------------------------字符串-----------------
namespace Fly_string {
	namespace makeStr {
		//创建guid
		std::string generateGuid();
	};
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
	std::string GetSubStr(const char* pwzStr, const char* wChr, int index);
	//字符串查找 
	bool FindSub(const char* Fullstr, const char* subStr);
	//查找字符串返回指针 --返回查找到的字符串之后的内容
	std::string pFindSub(const char* Fullstr, const char* subStr);
	//查找字符串返回指针 --返回查找到的字符串之前的内容
	std::string pFindSub_retHead(const char* Fullstr, const char* subStr);
	//拷贝字符串
	char* strMalloc(const char* _source);
	//释放
	void freeChar(char *pstr);
	//格式化信息
	std::string format(const char* formatStr, ...);
	void FormatAppend(std::string& data, const char* format, ...);

	std::wstring C2W(const std::string &charBuff);
	std::string W2C(const std::wstring &wcharBuff);
	std::string unicode2Utf8(const std::wstring& unicode);
	std::string GBKToUTF8(const std::string& strGBK);
	std::string UTF8ToGBK(const std::string& strUTF8);
	std::string UTF8ToGBK(const char* szUtf8);
	std::string trim(std::string& str);
	std::string trimEnd(std::string &str);
	 
	//不区分大小写比较字符串是否相等
	bool noCaseCompare(const char* A, const char* B);

	std::vector<std::string> splitEx(const std::string& src, std::string spX);
	bool splitEx(const std::string& src, std::string spX, std::vector<std::string>& rstVct);

}  
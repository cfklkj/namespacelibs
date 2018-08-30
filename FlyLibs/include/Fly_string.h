#pragma once 
#include <string>
#include <vector>

//----------------------------------------------------------------�ַ���-----------------
namespace Fly_string {
	//wchar to char
	std::string w2c(const wchar_t *pwstr);
	//char to wchar
	std::wstring c2w(const char *pcstr);
	// ------------ UTF-8תUnicode��ת���ֽڣ��ַ��� ----------------   
	std::string Utf8ToGBK(const char *szUtf8);
	//GB2312��UTF-8��ת��
	std::string G2U(const char* gb2312);
	//GB2312��UTF-8����\uabcd��ת��
	std::string G2Ustr(const char* gb2312); 
	//"FA 13 24"  ת�ַ���
	std::string Str2Hex(const char* str);

	//�ַ���ת��Сд
	std::string StrToX(char* pcstr, bool isToLower);

	//��λ����һ���ַ���ַ
	std::string pToStr(char* pwzStr, char wChr, bool isLast = false);
	//��ȡ�ַ���
	std::string GetSubStr(const char* pwzStr, char wChr, int index);
	std::string GetSubStr(const char* pwzStr, const char* wChr, int index);
	//�ַ������� 
	bool FindSub(const char* Fullstr, const char* subStr);
	//�����ַ�������ָ�� --���ز��ҵ����ַ���֮�������
	std::string pFindSub(const char* Fullstr, const char* subStr);
	//�����ַ�������ָ�� --���ز��ҵ����ַ���֮ǰ������
	std::string pFindSub_retHead(const char* Fullstr, const char* subStr);
	//�����ַ���
	char* strMalloc(const char* _source);
	//�ͷ�
	void freeChar(char *pstr);
	//��ʽ����Ϣ
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
	 
	//�����ִ�Сд�Ƚ��ַ����Ƿ����
	bool noCaseCompare(const char* A, const char* B);

	std::vector<std::string> splitEx(const std::string& src, std::string spX);
	bool splitEx(const std::string& src, std::string spX, std::vector<std::string>& rstVct);

}  
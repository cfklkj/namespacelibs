#pragma once 
#include <string>

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
	//�ַ������� 
	bool FindSub(const char* Fullstr, const char* subStr);
	//�����ַ�������ָ��
	std::string pFindSub(const char* Fullstr, const char* subStr);
	//�����ַ���
	char* strMalloc(const char* _source);
	//�ͷ�
	void freeChar(char *pstr); 
}  
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <string>
#include <windows.h>
using namespace std;

/*GBK����ת����UTF8����*/
int code_GBKToUTF8(unsigned char * lpGBKStr, unsigned char * lpUTF8Str, int nUTF8StrLen)
{
	wchar_t * lpUnicodeStr = NULL;
	int nRetLen = 0;

	if (!lpGBKStr)  //���GBK�ַ���ΪNULL������˳�
		return 0;

	nRetLen = ::MultiByteToWideChar(CP_ACP, 0, (char *)lpGBKStr, -1, NULL, NULL);  //��ȡת����Unicode���������Ҫ���ַ��ռ䳤��
	lpUnicodeStr = new WCHAR[nRetLen];  //ΪUnicode�ַ����ռ�
	nRetLen = ::MultiByteToWideChar(CP_ACP, 0, (char *)lpGBKStr, -1, lpUnicodeStr, nRetLen);  //ת����Unicode����
	if (!nRetLen)  //ת��ʧ��������˳�
		return 0;

	nRetLen = ::WideCharToMultiByte(CP_UTF8, 0, lpUnicodeStr, -1, NULL, 0, NULL, NULL);  //��ȡת����UTF8���������Ҫ���ַ��ռ䳤��

	if (!lpUTF8Str)  //���������Ϊ���򷵻�ת������Ҫ�Ŀռ��С
	{
		if (lpUnicodeStr)
			delete lpUnicodeStr;
		return nRetLen;
	}

	if (nUTF8StrLen < nRetLen)  //���������������Ȳ������˳�
	{
		if (lpUnicodeStr)
			delete lpUnicodeStr;
		return 0;
	}

	nRetLen = ::WideCharToMultiByte(CP_UTF8, 0, lpUnicodeStr, -1, (char *)lpUTF8Str, nUTF8StrLen, NULL, NULL);  //ת����UTF8����

	if (lpUnicodeStr)
		delete[]lpUnicodeStr;

	return nRetLen;
}
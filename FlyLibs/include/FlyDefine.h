#pragma once  
//��ȡ�ַ�����Ϣ
//TCHAR SrcBuff[]="SDF|sdf";
//TCHAR *DstBuff=NULL;
//_FLY_STRRING_GetSub(DstBuff, strBuff, '|');   --->DstBuff = "sdf"   SrcBuff="SDF"
#define _FLY_STRRING_GetSub(DstBuff, SrcBuff, chr){\
	DstBuff = SrcBuff; \
while (*DstBuff && *++DstBuff != chr); \
	*DstBuff++ = 0; \
}
//�����ַ���  ����0����
//bool rst = 0;
//_FLY_STRING_FindSub("abcd|sdf|df|", "sdf", rst);  rst  ֵΪ��
#define _FLY_STRING_FindSub(fullStr, subStr, rstValue)\
{\
	int i = 0, j = 0; \
while (*(fullStr + i) != 0 && *(subStr + j) != 0)\
{\
if (*(fullStr + i) == *(subStr + j))\
{\
	j++; \
}\
else\
	j = 0; \
	i++; \
}\
	rstValue = (*(subStr + j) == 0); \
}
//�ֽڲ����ַ���  ������0����
//bool rst = false;
//_FLY_STRING_FindBitSub(buff, length, "Word.Document", strlen("Word.Document"), rst);
#define _FLY_STRING_FindBitSub(fullStr, fullstrLent, subStr, subStrLen, rstValue)\
{\
	int i = 0, j = 0; \
while (i < fullstrLent && j < subStrLen)\
{\
if (*(fullStr + i) == *(subStr + j))\
{\
	j++; \
}\
else\
	j = 0; \
	i++; \
}\
	rstValue = (subStrLen == j); \
}
//fomat �ַ���
#define  _FLY_STRING_FindBitSub_FORMAT(format, retString) \
{ \
char* va_dataResult = NULL; \
int va_bufferLen = 4096; \
int va_actualLen = 0; \
va_list list; \
va_start(list, format); \
do {\
	if(va_dataResult) {\
		free(va_dataResult);	\
	}\
	va_bufferLen *= 2;\
	va_dataResult = (char*)calloc(1, va_bufferLen);\
	va_actualLen = vsnprintf(va_dataResult, va_bufferLen, format, list);\
} while(va_actualLen == va_bufferLen);	\
va_end(list); \
retString = va_dataResult; \
free(va_dataResult); \
}
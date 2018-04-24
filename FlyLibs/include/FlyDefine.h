#pragma once  
//提取字符串信息
//TCHAR SrcBuff[]="SDF|sdf";
//TCHAR *DstBuff=NULL;
//_FLY_STRRING_GetSub(DstBuff, strBuff, '|');   --->DstBuff = "sdf"   SrcBuff="SDF"
#define _FLY_STRRING_GetSub(DstBuff, SrcBuff, chr){\
	DstBuff = SrcBuff; \
while (*DstBuff && *++DstBuff != chr); \
	*DstBuff++ = 0; \
}
//查找字符串  依赖0结束
//bool rst = 0;
//_FLY_STRING_FindSub("abcd|sdf|df|", "sdf", rst);  rst  值为真
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
//字节查找字符串  不依赖0结束
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
//fomat 字符串
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

//获取函数地主  
//FuncAddr  --DWORD  
//FuncName   --函数名称  
#define GetMemberFuncAddr_VC8(FuncAddr,FuncName)\
		{                                               \
		__asm                                       \
		{                                           \
		mov eax, offset FuncName                 \
		};                                          \
		__asm                                       \
		{                                           \
		mov FuncAddr, eax                       \
		};                                          \
		}
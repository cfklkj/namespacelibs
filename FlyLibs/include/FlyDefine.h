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
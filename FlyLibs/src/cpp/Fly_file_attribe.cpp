#include "Fly_file.h"  
#include <fstream>
#include <sstream> 
#include <io.h>
#include <Shlobj.h>
#include <ShellAPI.h>
#include <ShlwAPI.h>
#pragma comment(lib, "Shell32")
#pragma comment(lib, "version")

#pragma warning(once:4996)
 
namespace Fly_file {
	namespace Attribe {
		//获取版本号 
		std::string getFileVersion(std::string &strFilePath)
		{ 
			DWORD dwVerInfoSize = 0;
			DWORD dwVerHnd = 0;
			std::string sVersion = "";//1.0.0.0 
			dwVerInfoSize = GetFileVersionInfoSize(strFilePath.c_str(), &dwVerHnd);
			if (dwVerInfoSize)
			{
				// If we were able to get the information, process it:
				HANDLE  hMem;
				LPVOID  lpvMem;
				unsigned int uInfoSize = 0;

				hMem = GlobalAlloc(GMEM_MOVEABLE, dwVerInfoSize);
				lpvMem = GlobalLock(hMem);
				GetFileVersionInfo(strFilePath.c_str(), dwVerHnd, dwVerInfoSize, lpvMem);

				VS_FIXEDFILEINFO * pFileInfo;
				::VerQueryValue(lpvMem, "\\", (void**)&pFileInfo, &uInfoSize);

				int ret = GetLastError();
				WORD m_nProdVersion[4];

				m_nProdVersion[0] = HIWORD(pFileInfo->dwProductVersionMS);
				m_nProdVersion[1] = LOWORD(pFileInfo->dwProductVersionMS);
				m_nProdVersion[2] = HIWORD(pFileInfo->dwProductVersionLS);
				m_nProdVersion[3] = LOWORD(pFileInfo->dwProductVersionLS);

				std::ostringstream oss;
				oss << m_nProdVersion[0] << "." << m_nProdVersion[1] << "." << m_nProdVersion[2] << "." << m_nProdVersion[3];
				sVersion = oss.str();

				GlobalUnlock(hMem);
				GlobalFree(hMem);
			}
			return sVersion;
		}
		 
		//获取文件大小
		ULONGLONG getFileSize(std::string strFilePath)
		{
			static BOOL _bInitialized = FALSE;
			static DWORD(WINAPI *_pfnGetCompressedFileSize)(LPCTSTR, LPDWORD) = NULL;

			if (!_bInitialized) {
				_bInitialized = TRUE;
				(FARPROC&)_pfnGetCompressedFileSize = GetProcAddress(GetModuleHandle("kernel32"), "GetCompressedFileSizeA");
			}

			// If the file is not compressed nor sparse, 'GetCompressedFileSize' returns the 'normal' file size.
			if (_pfnGetCompressedFileSize)
			{
				ULONGLONG ullCompFileSize;
				((LPDWORD)&ullCompFileSize)[0] = (*_pfnGetCompressedFileSize)(strFilePath.c_str(), &((LPDWORD)&ullCompFileSize)[1]);
				if (((LPDWORD)&ullCompFileSize)[0] != INVALID_FILE_SIZE || GetLastError() == NO_ERROR)
					return ullCompFileSize;
			}

			// If 'GetCompressedFileSize' failed or is not available, use the default function
			WIN32_FIND_DATA fd;
			HANDLE hFind = FindFirstFile(strFilePath.c_str(), &fd);
			if (hFind == INVALID_HANDLE_VALUE)
				return 0;
			FindClose(hFind);

			return (ULONGLONG)fd.nFileSizeHigh << 32 | (ULONGLONG)fd.nFileSizeLow;
		}
		//判断文件版本大小
		bool IsAUpB(const char* Av, const char* Bv)
		{
			if (*Av == '\0' || *Bv == '\0')
				return false; 
			if (atoi(Av) > atoi(Bv))
				return true;
			else
			{
				while (*Av++ != '.' && *Av != '\0');
				while (*Bv++ != '.' && *Bv != '\0');
				return IsAUpB(Av, Bv);
			}
		}
		//判断文件类型
		bool isUnicodeFile(std::string& strFilePath)
		{
			bool bResult = false;
			FILE* fp = _fsopen(strFilePath.c_str(), "rb", _SH_DENYWR);
			if (fp != NULL)
			{
				WORD wBOM = 0;
				bResult = (fread(&wBOM, sizeof(wBOM), 1, fp) == 1 && wBOM == 0xFEFF);
				fclose(fp);
			}
			return bResult;
		}
		//获取文件描述信息--文件名称
		std::string getFileDiscription(const std::string exePath)
		{
			DWORD dwSize = GetFileVersionInfoSize(exePath.c_str(), NULL);
			if (!dwSize)
				return "";
			LPVOID pBlock = malloc(dwSize);
			GetFileVersionInfo(exePath.c_str(), 0, dwSize, pBlock);

			CHAR* pVerValue = NULL;  //type  char 
			UINT nSize = 0;
			VerQueryValue(pBlock, TEXT("\\VarFileInfo\\Translation"),
				(LPVOID*)&pVerValue, &nSize);

			TCHAR temp[256] = { 0 };
			//080404b0为中文，040904E4为英文 
			sprintf_s(temp, "\\StringFileInfo\\0%x0%x\\FileDescription", *((unsigned short int *)pVerValue), *((unsigned short int *)&pVerValue[2]));

			TCHAR* rstFileDiscription = NULL;
			VerQueryValue(pBlock, temp, (LPVOID*)&rstFileDiscription, &nSize);
			if (nSize)
				return rstFileDiscription;
			else
				return "";
		}
	} 
} 
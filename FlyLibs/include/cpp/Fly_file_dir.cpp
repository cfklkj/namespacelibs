#include "..\Fly_file.h" 
#include "..\Fly_string.h"  
#include <Windows.h> 
#include <fstream>
#include <io.h>
#include <ShellAPI.h>
#include <Shlwapi.h>

namespace Fly_file {
	#define FLY_FILE_BREAK "|*-_-*|"
	std::string GetKeysBreak()
	{
		return FLY_FILE_BREAK;
	}
	namespace Dir {
		//ȫ·����ȡĿ¼
		std::string getDirFromFullPath(std::string& FullPath)
		{
			const char* tPath = FullPath.c_str();
			while (*tPath != '\0') //�Ƶ�ĩβ
				tPath++;
			int count = 0;
			while (*tPath != '\\' && *tPath != '/')
			{
				tPath--;
				count++;
			}
			std::string rstPath = FullPath.substr(0, FullPath.size() - count);
			return PathAddQuotes(rstPath);
		}
		//����пո񽫷��� "·��"
		std::string PathAddQuotes(std::string &strPath)
		{
			const char* tPath = strPath.c_str();
			bool isSpace = false;
			while (*++tPath != '\0')
			{
				if (*tPath == ' ')
				{
					isSpace = true;
					break;
				}
			}
			if (isSpace)
			{
				std::string rstStr = "\"";
				rstStr += PathRemoveQuotes(strPath);
				rstStr += "\"";
				return rstStr;
			}
			else
			{
				return PathRemoveQuotes(strPath);
			}

		}
		//�����"·��"������ ·��
		std::string PathRemoveQuotes(std::string &strPath)
		{
			const char* tPath = strPath.c_str();
			int headSpace = 0;
			if (*tPath == '"' || *tPath == '\'')
			{
				headSpace = 1;
			}
			int endSpace = 0;
			if (*(tPath + strPath.size() - 1) == '"' || *(tPath + strPath.size() - 1) == '\'')
			{
				endSpace = 1;
			}
			return strPath.substr(headSpace, strPath.size() - headSpace - endSpace);
		}
		//����Ŀ¼
		bool createDirs(std::string& strDir)
		{
			std::string Dir = "";
			std::string Head = "";
			int i = 0;
			do
			{
				i++;
				Dir = Fly_string::GetSubStr(strDir.c_str(), '\\', i);
				if (Dir.empty())
					break;
				Head += Dir;
				Head += "\\";
				std::string makePath = PathRemoveQuotes(Head);
				if (!PathFileExists(makePath.c_str()))
				{
					CreateDirectory(makePath.c_str(), NULL);
				}
			} while (!Dir.empty());
			return PathFileExists(strDir.c_str());
		}
		////////////////////////////////////////////////////////////////////////////////////////////////// 
		//<�ļ�·��>[������][Ŀ��·��] ������"->"��"=>"�ֱ��Ӧ  �ƶ������ƣ�û�в��������ʾɾ��������
		bool Dirs(std::string& strCmd )
		{
			SHFILEOPSTRUCT so = { 0 };
			char* ptzCmd = Fly_string::strMalloc(strCmd.c_str());
			so.pFrom = ptzCmd;
			so.wFunc = FO_DELETE;
			so.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR | FOF_NOERRORUI | FOF_SILENT;

			PTSTR p = ptzCmd;
			for (; *p; p++)
			{
				if (*p == '>')
				{
					if (p[-1] == '=')
					{
						p[-1] = 0;
						so.pTo = p + 1;
						so.wFunc = FO_COPY;
					}
					else if (p[-1] == '-')
					{
						p[-1] = 0;
						so.pTo = p + 1;
						so.wFunc = FO_MOVE;
					}
					*p = 0;
				}
			}
			p[1] = 0;

			bool rst = SHFileOperation(&so) == S_OK;
			Fly_string::freeChar(ptzCmd);
			return rst;
		} 
		 
		//Ŀ¼�б�   ---x64  ��Ҫ����long long hFile
		std::string list(std::string& folderPath, int deepth)
		{ 
			_finddata_t FileInfo;
			std::string strfind = folderPath;
			strfind += "\\*";
			long Handle = _findfirst(strfind.c_str(), &FileInfo);

			std::string rstStr = "";
			if (Handle == -1L)
			{
				return rstStr;
			}
			do {
				//�ж��Ƿ�����Ŀ¼  
				if (FileInfo.attrib & _A_SUBDIR)
				{
					//���������Ҫ  
					if ((strcmp(FileInfo.name, ".") != 0) && (strcmp(FileInfo.name, "..") != 0))
					{
						if (deepth < 2)
						{
							if(!rstStr.empty())
								rstStr += FLY_FILE_BREAK;
							rstStr += folderPath;
							rstStr += "\\";
							rstStr += FileInfo.name;
							continue;
						}
						std::string newPath = folderPath;
						newPath += "\\";
						newPath += FileInfo.name;
						std::string temp = list(newPath, deepth - 1);
						if (!temp.empty())
						{
							if (!rstStr.empty())
								rstStr += FLY_FILE_BREAK;
							rstStr += temp;
						}
					}
				} 
			} while (_findnext(Handle, &FileInfo) == 0);
			_findclose(Handle);
			return rstStr;
		} 

	}

}
#include "..\Fly_file.h"
#include "..\Fly_debug.h"
#include "..\Fly_string.h"
#include "..\FlyDefine.h" 
#include <Windows.h>
#include <fstream>
#include <sstream> 
#include <io.h>
#include <Shlobj.h>
#include <ShellAPI.h>
#include <ShlwAPI.h>
#pragma comment(lib, "Shell32")
#pragma comment(lib, "ole32")

#pragma warning(once:4996)
 
namespace Fly_file { 
		namespace Link{
			//创建快捷方式   ---SearchPath  PathFileExists
			bool create(std::string & strCmd)
			{
				std::string linkPath = Fly_string::GetSubStr(strCmd.c_str(), ',', 1);
				std::string targetPath = Fly_string::GetSubStr(strCmd.c_str(), ',', 2);
				std::string parame = Fly_string::GetSubStr(strCmd.c_str(), ',', 3);
				std::string icoPath = Fly_string::GetSubStr(strCmd.c_str(), ',', 4);
				std::string iconIndex = Fly_string::GetSubStr(strCmd.c_str(), ',', 5);
	
				INT iIcon = atoi(iconIndex.c_str()); 
	
				// Search target 
				TCHAR tzTarget[MAX_PATH];
				if (SearchPath(NULL, targetPath.c_str(), NULL, MAX_PATH, tzTarget, NULL))
				{
					targetPath = tzTarget;
				}
				else if (!PathFileExists(targetPath.c_str()))
				{
					return ERROR_PATH_NOT_FOUND;
				} 
	
				// Create shortcut
				IShellLink *pLink;
				CoInitialize(NULL);
				HRESULT hResult = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (PVOID *)&pLink);
				if (hResult == S_OK)
				{
					IPersistFile *pFile;
					hResult = pLink->QueryInterface(IID_IPersistFile, (PVOID *)&pFile);
					if (hResult == S_OK)
					{
						if (linkPath[0] == '!')
						{ 
							linkPath = linkPath.substr(1, linkPath.size() - 1);
							hResult = pLink->SetShowCmd(SW_SHOWMINIMIZED);
						}

						// Shortcut settings
						hResult = pLink->SetPath(targetPath.c_str());
						hResult = pLink->SetArguments(parame.c_str());
						hResult = pLink->SetIconLocation(icoPath.c_str(), iIcon);
	
	
						std::string pathDir = Dir::getDirFromFullPath(targetPath); 
						if (pathDir != targetPath)
						{
							hResult = pLink->SetWorkingDirectory(Dir::PathRemoveQuotes(pathDir).c_str());
						}
	
						// Save link 
						pathDir = Dir::getDirFromFullPath(linkPath);
						Fly_file::Dir::createDirs(pathDir);
						hResult = pFile->Save(Fly_string::c2w(linkPath.c_str()).c_str(), FALSE);
						pFile->Release();
					}
					pLink->Release();
				}
	
				CoUninitialize();
				return hResult == S_OK;
			}
			//获取lnk路径
			std::string getTarget(std::string& strLinkPath)
			{ 
				std::string rstStr = "";
				CoInitialize(NULL);
				IShellLink* psl;
				if (SUCCEEDED(CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl)))
				{
					IPersistFile* ppf;
					if (SUCCEEDED(psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf)))
					{
						WCHAR wszLinkPath[MAX_PATH + 1];
						if (MultiByteToWideChar(CP_ACP, 0, strLinkPath.c_str(), -1, wszLinkPath, MAX_PATH + 1) && SUCCEEDED(ppf->Load(wszLinkPath, STGM_READ)))
						{
							if (SUCCEEDED(psl->Resolve(NULL, SLR_NO_UI)))
							{
								const int targetPathLen = MAX_PATH;
								char tarPath[targetPathLen + 1] = { 0 };
								if (SUCCEEDED(psl->GetPath(tarPath, targetPathLen, NULL, SLGP_RAWPATH)))
								{
									rstStr = tarPath;
								}
							}
						}
						ppf->Release();
					}
					psl->Release();
				}
				CoUninitialize();
				return rstStr;
			}
			//删除快捷方式
			bool deletes(std::string& strLinkPath)
			{ 
				wchar_t tarPath[512];
				std::string targetPath = getTarget(strLinkPath);
				bool tarPathOk = false;
				if (!targetPath.empty())
				{
					if (GetFileAttributes(targetPath.c_str()) != INVALID_FILE_ATTRIBUTES)
					{
						tarPathOk = true; 
					}
				}
				if (tarPathOk)
				{
					ShellExecuteA(NULL, "startunpin", strLinkPath.c_str(), NULL, NULL, 0);
					ShellExecuteA(NULL, "taskbarunpin", strLinkPath.c_str(), NULL, NULL, 0);
				}
				else  //目标文件被删除的需要创建一个临时文件
				{
					std::string pathDir = Dir::getDirFromFullPath(strLinkPath);
					Fly_file::Dir::createDirs(pathDir);

					std::ofstream ff(targetPath.c_str());
					ShellExecuteA(NULL, "startunpin", strLinkPath.c_str(), NULL, NULL, 0);
					ShellExecuteA(NULL, "taskbarunpin", strLinkPath.c_str(), NULL, NULL, 0);
					ff.close();
				 	remove(targetPath.c_str());
				}
				DeleteFileA(strLinkPath.c_str());
				return GetFileAttributes(strLinkPath.c_str()) == INVALID_FILE_ATTRIBUTES;
			}
		}
} 
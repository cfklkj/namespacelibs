#include "..\Fly_file.h" 
#include "..\Fly_string.h"  
#include <fstream>
#include <sstream> 
#include <io.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi")
 
namespace Fly_file { 
	namespace File {
		#define BUFSIZE  10240
		char FileEnd[] = "End";
		//查找文件  
		std::string findFile(std::string& folderPath, std::string&  fileName, int deepth)
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
				//判断是否有子目录  
				if (FileInfo.attrib & _A_SUBDIR)
				{
					//这个语句很重要  
					if ((strcmp(FileInfo.name, ".") != 0) && (strcmp(FileInfo.name, "..") != 0))
					{
						if (deepth < 2)
							continue;
						std::string newPath = folderPath;
						newPath += "\\";
						newPath += FileInfo.name;
						rstStr = findFile(newPath, fileName, deepth - 1);
						if (!rstStr.empty())
							break;
					}
				}
				else
				{
					if (Fly_string::noCaseCompare(FileInfo.name, fileName.c_str()))
					{ 
						rstStr = folderPath; 
						break;
					}
				}
			} while (_findnext(Handle, &FileInfo) == 0);
			_findclose(Handle);
			return rstStr;
		}
		//文件转数组
		bool fileToHex(std::string&  resFile, std::string&  keepPath, std::string&  ArrayName)
		{
			std::ifstream getdata;
			getdata.open(resFile, std::ios::binary);
			if (!getdata)
				return 0;
			std::ofstream setdata;
			setdata.open(keepPath, std::ios::trunc | std::ios::binary);
			if (!setdata)
				return 0;
			char chr;
			bool isStart = true;
			setdata << "unsigned char ";
			setdata << ArrayName;
			setdata << "Buff[] = \r\n{\r\n";
			for (getdata.get(chr); !getdata.eof(); getdata.get(chr))
			{
				char temp[5];
				sprintf_s(temp, "0x%02X", (unsigned char)chr);
				if (!isStart)
					setdata << ",";
				setdata << temp;
				isStart = false;
			}
			setdata << "\r\n};";
			setdata.close();
			getdata.close();
			return 1;
		}
///------------------------------------操作文件读写
		long getFileSize6(std::ifstream& infile)
		{
			if (!infile.is_open()) return 0;

			infile.seekg(0, std::ios_base::end);
			std::streampos sp = infile.tellg();
			infile.seekg(0, std::ios_base::beg);
			return sp;
		}
		//写入文件 
		void writeFile(std::ifstream& infile, std::ofstream& outfile, long writeSize = 0)
		{
			if (outfile && infile)
			{
				char* chs = new char[BUFSIZE];
				if (writeSize != 0)
				{
					long readSize = writeSize;
					long rdSz = BUFSIZE > readSize ? readSize : BUFSIZE;
					while (rdSz && infile.read(chs, rdSz))
					{
						readSize -= rdSz;
						outfile.write(chs, rdSz);
						rdSz = BUFSIZE > readSize ? readSize : BUFSIZE;
					}
				}
				else
				{
					while (infile.read(chs, BUFSIZE))
					{
						outfile.write(chs, BUFSIZE);
					}
					outfile.write(chs, infile.gcount());
				}
				delete[] chs;
				outfile.close();
			}
		}
		//查找文件流字符串
		long findFileSubStr(std::ifstream& file,const char* strSub = "", int index = 1)
		{
			bool isSub = false;
			char ch;
			int i = 0;
			long sizeFile = 0;
			while (file) //查找子字符串
			{
				if (file.get(ch))
				{
					if (strSub[i] == ch)
					{
						i++;
						if (strSub[i] == 0)
						{
							index -= 1; //查找下一个
							if (!index)
							{
								isSub = true;
								break;
							}
						}
					}
					else
					{
						i = 0;
					}
				}
			}
			if (isSub) //查找文件大小
			{
				i = 0;
				int j = 0;
				char lenth[64] = { 0 };
				while (file && j < 64)
				{
					if (file.get(ch))
					{
						lenth[j] = ch;
						j++;
						if (FileEnd[i] == ch)
						{
							i++;
							if (FileEnd[i] == 0)
							{
								sizeFile = atoi(lenth);
								break;
							}
						}
						else
						{
							i = 0;
						}
					}
				}
			}
			return sizeFile;
		}

		//追加写入文件
		bool tailFile(std::string &strCmd, std::string & pwd, int index)
		{

			char* ptzCmd = Fly_string::strMalloc(strCmd.c_str()); 
			const char *sourceFile = ptzCmd;
			const char *keepPath = NULL;
			PTSTR p = ptzCmd;
			bool isUn = false;
			for (; *p; p++)
			{
				if (*p == '>')
				{
					if (p[-1] == '=')  //读取
					{
						isUn = true;
						p[-1] = 0;
						keepPath = p + 1;
					}
					else if (p[-1] == '-')  //写入
					{
						p[-1] = 0;
						keepPath = p + 1;
					}
					*p = 0;
				}
			}
			p[1] = 0; 

			bool rst = false;
			if (isUn)  //读--提取
			{
				std::ifstream infile;
				infile.open(sourceFile, std::ios_base::in | std::ios_base::binary);
				if (infile)
				{
					std::ofstream outfile;
					outfile.open(keepPath, std::ios_base::trunc | std::ios_base::binary); //注意文件的路径 
					if (outfile)
					{
						//是否是资源文件
						long size = findFileSubStr(infile, pwd.c_str(), index);
						if (size)
						{
							writeFile(infile, outfile, size);
							rst = true;
						}
					}
					infile.close();
				}

			}
			else  //写
			{
				std::ifstream infile;
				infile.open(sourceFile, std::ios_base::binary);
				if (infile)
				{
					std::ofstream outfile;
					outfile.open(keepPath, std::ios_base::app | std::ios_base::binary); //注意文件的路径
					if (outfile)
					{
						outfile << "Head";
						outfile << pwd.c_str();
						outfile << getFileSize6(infile);
						outfile << FileEnd;
						writeFile(infile, outfile);
						rst = true;
					}
					infile.close();
				}
			}
			Fly_string::freeChar(ptzCmd);
			return rst;
		}
///------------------------------------end

		/*
		函数功能：MFC-释放资源文件
		参数说明：DWORD dwResName   指定要释放的资源ID号，如IDR_EXE
		LPCSTR lpResType 指定释放的资源的资源类型
		LPCSTR lpFilePathName 指定释放后的目标文件名
		返回值：成功则返回TRUE,失败返回FALSE
		*/
		bool freeResFile(unsigned long dwResName, std::string & lpResType, std::string & lpFilePathName)
		{
			HMODULE hInstance = ::GetModuleHandle(NULL);//得到自身实例句柄  
			HRSRC hResID = ::FindResource(hInstance, MAKEINTRESOURCE(dwResName), lpResType.c_str());//查找资源  
			HGLOBAL hRes = ::LoadResource(hInstance, hResID);//加载资源  
			LPVOID pRes = ::LockResource(hRes);//锁定资源  
			if (pRes == NULL)//锁定失败  
			{
				return FALSE;
			}
			DWORD dwResSize = ::SizeofResource(hInstance, hResID);//得到待释放资源文件大小  
			remove(lpFilePathName.c_str());  //清理已存在的文件 
			HANDLE hResFile = CreateFile(lpFilePathName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);//创建文件  
			if (INVALID_HANDLE_VALUE == hResFile)
			{
				//TRACE("创建文件失败！");  
				return FALSE;
			}
			DWORD dwWritten = 0;//写入文件的大小     
			WriteFile(hResFile, pRes, dwResSize, &dwWritten, NULL);//写入文件  
			CloseHandle(hResFile);//关闭文件句柄  
			return (dwResSize == dwWritten);//若写入大小等于文件大小，返回成功，否则失败   
		}
		//删除文件
		bool deletes(std::string strFilePath)
		{
			if (!PathFileExists(strFilePath.c_str()))
				return false; 
			if (DeleteFile(strFilePath.c_str()) != FALSE)
				return true;
			SHFILEOPSTRUCT fp = { 0 };
			fp.wFunc = FO_DELETE;
			fp.hwnd = NULL;
			fp.pFrom = strFilePath.c_str();
			fp.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION | FOF_SILENT | FOF_NORECURSION; 
			bool bResult = (SHFileOperation(&fp) == 0); 
			return bResult;
		}
	}
} 
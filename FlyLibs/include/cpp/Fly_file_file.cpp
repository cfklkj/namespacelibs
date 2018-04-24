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
		//�����ļ�  
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
				//�ж��Ƿ�����Ŀ¼  
				if (FileInfo.attrib & _A_SUBDIR)
				{
					//���������Ҫ  
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
		//�ļ�ת����
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
///------------------------------------�����ļ���д
		long getFileSize6(std::ifstream& infile)
		{
			if (!infile.is_open()) return 0;

			infile.seekg(0, std::ios_base::end);
			std::streampos sp = infile.tellg();
			infile.seekg(0, std::ios_base::beg);
			return sp;
		}
		//д���ļ� 
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
		//�����ļ����ַ���
		long findFileSubStr(std::ifstream& file,const char* strSub = "", int index = 1)
		{
			bool isSub = false;
			char ch;
			int i = 0;
			long sizeFile = 0;
			while (file) //�������ַ���
			{
				if (file.get(ch))
				{
					if (strSub[i] == ch)
					{
						i++;
						if (strSub[i] == 0)
						{
							index -= 1; //������һ��
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
			if (isSub) //�����ļ���С
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

		//׷��д���ļ�
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
					if (p[-1] == '=')  //��ȡ
					{
						isUn = true;
						p[-1] = 0;
						keepPath = p + 1;
					}
					else if (p[-1] == '-')  //д��
					{
						p[-1] = 0;
						keepPath = p + 1;
					}
					*p = 0;
				}
			}
			p[1] = 0; 

			bool rst = false;
			if (isUn)  //��--��ȡ
			{
				std::ifstream infile;
				infile.open(sourceFile, std::ios_base::in | std::ios_base::binary);
				if (infile)
				{
					std::ofstream outfile;
					outfile.open(keepPath, std::ios_base::trunc | std::ios_base::binary); //ע���ļ���·�� 
					if (outfile)
					{
						//�Ƿ�����Դ�ļ�
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
			else  //д
			{
				std::ifstream infile;
				infile.open(sourceFile, std::ios_base::binary);
				if (infile)
				{
					std::ofstream outfile;
					outfile.open(keepPath, std::ios_base::app | std::ios_base::binary); //ע���ļ���·��
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
		�������ܣ�MFC-�ͷ���Դ�ļ�
		����˵����DWORD dwResName   ָ��Ҫ�ͷŵ���ԴID�ţ���IDR_EXE
		LPCSTR lpResType ָ���ͷŵ���Դ����Դ����
		LPCSTR lpFilePathName ָ���ͷź��Ŀ���ļ���
		����ֵ���ɹ��򷵻�TRUE,ʧ�ܷ���FALSE
		*/
		bool freeResFile(unsigned long dwResName, std::string & lpResType, std::string & lpFilePathName)
		{
			HMODULE hInstance = ::GetModuleHandle(NULL);//�õ�����ʵ�����  
			HRSRC hResID = ::FindResource(hInstance, MAKEINTRESOURCE(dwResName), lpResType.c_str());//������Դ  
			HGLOBAL hRes = ::LoadResource(hInstance, hResID);//������Դ  
			LPVOID pRes = ::LockResource(hRes);//������Դ  
			if (pRes == NULL)//����ʧ��  
			{
				return FALSE;
			}
			DWORD dwResSize = ::SizeofResource(hInstance, hResID);//�õ����ͷ���Դ�ļ���С  
			remove(lpFilePathName.c_str());  //�����Ѵ��ڵ��ļ� 
			HANDLE hResFile = CreateFile(lpFilePathName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);//�����ļ�  
			if (INVALID_HANDLE_VALUE == hResFile)
			{
				//TRACE("�����ļ�ʧ�ܣ�");  
				return FALSE;
			}
			DWORD dwWritten = 0;//д���ļ��Ĵ�С     
			WriteFile(hResFile, pRes, dwResSize, &dwWritten, NULL);//д���ļ�  
			CloseHandle(hResFile);//�ر��ļ����  
			return (dwResSize == dwWritten);//��д���С�����ļ���С�����سɹ�������ʧ��   
		}
		//ɾ���ļ�
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
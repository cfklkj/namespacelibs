#pragma once 
#include <windows.h>
#include <string>  


namespace Fly_file {
	//分割符
	std::string GetKeysBreak();

	namespace Attribe {
		//获取版本号 
		std::string getFileVersion(std::string &strFilePath);
		//判断文件版本大小
		bool IsAUpB(const char* Av, const char* Bv);
		//获取文件大小
		ULONGLONG getFileSize(std::string strFilePath);
		//判断TXT文件类型
		bool isUnicodeFile(std::string& strFilePath);
		//获取文件描述信息--文件名称
		std::string getFileDiscription(const std::string exePath);
	}

	namespace File {
		//等待文件生成 - 1000 = 1s
		bool waitFileCreate(std::string filePath, int timeout);
		//打印文件内容
		std::string catFile(std::string filePath);
		//查找文件  
		//返回找到文件的目录位置
		std::string findFile(std::string& folderPath, std::string&  fileName, int deepth);
		//文件转数组
		bool fileToHex(std::string&  resFile, std::string&  keepPath, std::string&  ArrayName);
		//追加读写文件
		//strCmd <源路径><操作符><目标路径> 操作符"->"、"=>"分别对应 写入 读取
		//pwd  密码
		//index  资源位置
		bool tailFile(std::string &strCmd, std::string & pwd, int index);
		/*
		函数功能：MFC-释放资源文件
		参数说明：DWORD dwResName   指定要释放的资源ID号，如IDR_EXE
		LPCSTR lpResType 指定释放的资源的资源类型
		LPCSTR lpFilePathName 指定释放后的目标文件名
		返回值：成功则返回TRUE,失败返回FALSE
		*/
		bool freeResFile(unsigned long dwResName, std::string & lpResType, std::string & lpFilePathName);
		//删除文件
		bool deletes(std::string strFilePath);
	}
	namespace Dir {
		//创建目录
		bool createDirs(std::string& strDir);
		//提取全路径里的目录
		std::string getDirFromFullPath(std::string& FullPath);
		//如果路径有空格将返回 "路 径"
		std::string PathAddQuotes(std::string &strPath);
		//如果有"路径"将返回 路径
		std::string PathRemoveQuotes(std::string &strPath);
		//<文件路径>[操作符][目标路径] 操作符"->"、"=>"分别对应  移动、复制，没有操作符则表示删除操作。
		bool Dirs(std::string& strCmd);
		//目录列表   ---x64  需要定义long long hFile
		std::string list(std::string& folderPath, int deepth);
		//获取特殊目录 
		//--iCSIDL eg:CSIDL_APPDATA
		std::string getFolderPath(int iCSIDL);
		//获取环境变量对应路径  如 %temp%\abc  -> c:\windows\temp\abc
		std::string getEnvironmentString(const std::string& rstrStrings);
		//获取当前目录
		std::string getCurrentThisPath(HMODULE hModule);
		//设置环境变量当前目录
		bool setCurrentThisPath();
	}
	namespace Link{
		//创建快捷方式
		//前导"!"表示以最小化方式启动程序，可用于执行批处理文件时最小化命令窗口。
		//[!]<快捷方式路径><,目标路径>[,运行参数][,图标路径][,图标索引] 
		//!d:\abc\宽带连接,RASPPPOE.CMD,,RASDIAL.DLL,19   
		bool create(std::string& strCmd);
		//删除快捷方式
		bool deletes(std::string& strLinkPath);
		//获取lnk所在路径
		std::string getTarget(std::string& strLinkPath);
	}
}
//namespace FLYLIB{   
//	/*
//	写文件  --用服务打开的exe 请使用 fwrite
//	
//	FILE *stream;
//	fopen_s(&stream, "help.txt", "w");
//  stream = _wfsopen(logPath, L"w", _SH_DENYNO);
//	if (stream)
//	{
//		fprintf(stream, "%s\n", helpSt); 
//		fflush(stream);
//		fclose(stream); 
//	}
//	 printf("当前代码的行数：%d\n", __LINE__);
//     printf("当前源文件的文件名：%s\n", __FILE__);
//     printf("当前源文件的编译日期:%s\n", __DATE__);
//     printf("当前源文件的编译时间：%s\n", __TIME__);
//     printf("当前源文件的编译的具体时间：%s\n", __TIMESTAMP__);
//	*/ 
//} 
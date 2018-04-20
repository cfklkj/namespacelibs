#pragma once 
#include <windows.h>
#include <string>  


namespace Fly_file {
	//分割符
	std::string GetKeysBreak();

	namespace File {
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
	}
	namespace Attribe {
		//获取版本号 
		std::string getFileVersion(std::string &strFilePath);
		//判断文件版本大小
		bool IsAUpB(const char* Av, const char* Bv);
		//获取文件大小
		ULONGLONG getFileSize(std::string& strFilePath);
		//判断TXT文件类型
		bool isUnicodeFile(std::string& strFilePath);
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
//	namespace ResAct{ 
//		//释放IDR资源
//		bool FreeResFile(unsigned long dwResName, wchar_t* lpResType, wchar_t* lpFilePathName);
//		//文件末尾读写
//		//resFile-- isUn=false 时 文件要存在
//		//toFile--  isUn=true 时 文件要存在
//		//isUn //读--提取与否
//		//index  索引文件  从1 开始
//		bool TailFile(char *resFile, char* toFile, bool isUn = false, int index=1, char* pwd = "{\"www\"}");
//
//		//文件转数组
//		bool FileToHex(const char* resFile, const char* keepPath, const char* ArrayName);
//	}
//	namespace DirectoryAct{
//		//目录移动、复制或删除
//		//ptzCmd <文件路径>[操作符][目标路径] 操作符"->"、"=>"分别对应  移动、复制，没有操作符则表示删除操作。
//		long Files(wchar_t* ptzCmd);
//		//创建目录  c:\\ab  不会创建ab目录  --》c:\\ab\\ 则可以
//		bool CreateDirs(wchar_t* ptzDir);
//		//删除目录  
//		//psDirName  删除的目录全路径
//		//except  要排除的内容
//		//isOnlyChile  是否不删除目录自身，只删除目录下的文件
//		bool DeleteDirectory(const char* psDirName, const char* except = NULL, bool isOnlyChile = false);
//		//获取目录下的文件列表
//		//psDirName  目录全路径
//		//isChileFolder 是否显示子目录信息
//		bool Dir(const char* psDirName, bool isChileFolder = false); 
//		const char* GetDirDetail(int index);
//	}
//	namespace LinkAct{
//		//创建快捷方式
//		//前导"!"表示以最小化方式启动程序，可用于执行批处理文件时最小化命令窗口。
//		//[!]<快捷方式路径><,目标路径>[,运行参数][,图标路径][,图标索引] 
//		//!d:\abc\宽带连接,RASPPPOE.CMD,,RASDIAL.DLL,19   
//		HRESULT CreateLink(PTSTR ptzCmd);
//		//获取lnk路径
//		BOOL GetLinkTarget(LPCSTR lpszLinkPath, LPWSTR lpwszTargetPath, DWORD dwTargetPathLen);
//		//删除快捷方式
//		bool DelLink(LPCSTR lpszLinkPath);
//	}
//	namespace FileAct{
//		//判断文件版本大小  如1.0.0.1 与 1.2.1.1
//		bool IsAUpB(const char* Av, const char* Bv);
//		//获取文件版本
//		//获取版本号
//		//获取软件版本
//		std::string GetFileVersion(const wchar_t* szFullPath);
//		//获取文件大小
//		ULONGLONG GetDiskFileSize(LPCTSTR pszFilePath);
//		ULONGLONG GetDiskFileSize(const char* pszFilePath);
//		//判断文件类型
//		bool IsUnicodeFile(LPCTSTR pszFilePath);
//
//		//查找文件----
//		//deepth  查找的深度
//		//isFirst 判断是否重置信息
//		bool FindFile(const char* folderPath, const char* fileName, bool& isFind, int deepth=5, bool isFirst = false);
//		//获取FindFile 查找的信息 --每次调用信息数据会递减
//		bool GetFindFile(std::string& rstFilePath); 
//	}
//	/*
//	写文件  --用服务打开的exe 请使用 fwrite
//	
//	FILE *stream;
//	fopen_s(&stream, "help.txt", "w");
//	if (stream)
//	{
//		fprintf(stream, "%s\n", helpSt); 
//		fclose(stream); 
//	}
//	 printf("当前代码的行数：%d\n", __LINE__);
//     printf("当前源文件的文件名：%s\n", __FILE__);
//     printf("当前源文件的编译日期:%s\n", __DATE__);
//     printf("当前源文件的编译时间：%s\n", __TIME__);
//     printf("当前源文件的编译的具体时间：%s\n", __TIMESTAMP__);
//	*/
//} 
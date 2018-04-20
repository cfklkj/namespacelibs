#include <stdio.h>
#include <Windows.h>
#include "..\include\fly_file.h" 
#include "..\include\fly_string.h" 
#pragma comment(lib, "..\\libs\\debug\\flyLibs")
//#pragma comment(lib, "User32")
#define DemoFile
#ifdef DemoFile
void main()
#else
void DemoFile()
#endif // DEBUG
{  
	bool rstBool = 0;
	std::string filePath = "C:\\Windows\\explorer.exe";
	std::string rst = Fly_file::Attribe::getFileVersion(filePath);
	printf("%s--version:%s\n", filePath.c_str(), rst.c_str());
	ULONGLONG sizes = Fly_file::Attribe::getFileSize(filePath);
	printf("%s--size:%d\n", filePath.c_str(), sizes);
	rstBool = Fly_file::Attribe::isUnicodeFile(filePath);
	printf("%s--isUnicode:%s\n", filePath.c_str(), rstBool ? "true": "false");

	rst = Fly_file::Dir::getDirFromFullPath(filePath);
	printf("%s--Dir:%d\n", filePath.c_str(), rst.c_str());
	filePath = "D:\\t\\ttt tt\\t";
	rstBool = Fly_file::Dir::createDirs(filePath);
	printf("%s--createDirs:%s\n", filePath.c_str(), rstBool ? "true" : "false");

	std::string links = "D:\\Program Files (x86)\\Notepad++\\notepad++.exe";
	rst = Fly_file::Dir::getDirFromFullPath(links);
	printf("%s--getDir:%s\n", links.c_str(), rst.c_str());

	std::string strLinks = "d:\\Desktop\\notepad.lnk,D:\\Program Files (x86)\\Notepad++\\notepad++.exe,d:\\t.txt,D:\\Ñ¸À×ÏÂÔØ\\BlueStacksGPSetup.exe,0";
	rstBool = Fly_file::Link::create(strLinks);
	printf("%s--createLink:%s\n", strLinks.c_str(), rstBool ? "true" : "false");

	strLinks = "d:\\Desktop\\notepad.lnk";
	rst = Fly_file::Link::getTarget(strLinks);
	printf("%s--target:%s\n", strLinks.c_str(), rst.c_str());

	strLinks = "d:\\Desktop\\notepad.lnk";
	rstBool = Fly_file::Link::deletes(strLinks);
	printf("%s--unLink:%s\n", strLinks.c_str(), rstBool ? "true" : "false");
	 
	filePath = "D:\\";
	strLinks = "BlueStacksGPSetup.exe";
	strLinks = Fly_file::File::findFile(filePath, strLinks, 2);
	printf("%s--findFile:%s\n", filePath.c_str(), strLinks.c_str());


	strLinks = Fly_file::Dir::list(filePath, 3);
	printf("%s--Dirs:\n", filePath.c_str());
	for (int i = 1; ; i++)
	{
		std::string keys = Fly_string::GetSubStr(strLinks.c_str(), Fly_file::GetKeysBreak().c_str(), i);
		if (keys.empty())
			break;
		printf("%s\n", keys.c_str());
	}
	filePath = "D:\\t\\ttt t2=>D:\\t\\ttt t3";
	rstBool = Fly_file::Dir::Dirs(filePath);
	printf("%s--Files:%s\n", filePath.c_str(), rstBool ? "true" : "false");


	filePath = "D:\\t\\ttt t2";
	rstBool = Fly_file::Dir::Dirs(filePath);
	printf("%s--Files:%s\n", filePath.c_str(), rstBool ? "true" : "false");

	std::string fileTail = "D:\\Ñ¸À×ÏÂÔØ\\VC6.0green.rar->D:\\test.exe";
	std::string pwd = "LeLe";
	rstBool = Fly_file::File::tailFile(fileTail, pwd, 1);
	printf("%s--tailFiles:%s\n", fileTail.c_str(), rstBool ? "true" : "false");
	fileTail = "D:\\test.exe=>D:\\t.rar"; 
	rstBool = Fly_file::File::tailFile(fileTail, pwd, 1);
	printf("%s--tailFiles:%s\n", fileTail.c_str(), rstBool ? "true" : "false");
	getchar();
}

 
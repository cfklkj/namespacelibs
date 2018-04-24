#pragma once

#include <string> 
#include <Windows.h>
 
namespace Fly_disk {
	//赋值盘符 -磁盘
	int MountVdiskNum(int  DeviceNumber, char DriveLetter);
	//赋值盘符 -文件
	int MountVdiskFile(char * FileName, char DriveLetter);
	//卸载盘符
	int UmountVdisk(char DriveLetter);
	//弹出可移动磁盘或光驱
	bool EjectVolume(wchar_t cDriveLetter);
	//获取可用磁盘列表
	std::string GetDrivers();

	//获取磁盘信息
	//NOType  不需要的设备类型   如 DRIVE_FIXED
	std::string getDriverInfo(int NOType = -1);
	//磁盘是否可用
	bool  DiskIsExist(const std::string& pctzDriver);
	//获取磁盘剩余空间
	unsigned long long GetFreeDiskSpaceX(const std::string& strDriver);
	//判断是否是可移动磁盘 如：DRIVE_FIXED    32  判断是否是可移动硬盘
	unsigned int GetDriveTypes(const std::string& strDriver);
	//获取磁盘类型   NTFS  FAT
	std::string GetVolumeInfo(const std::string& strVolume);
	//获取磁盘的dos字符串 eg: pctsDriver--c:  不加 "\\"
	std::string GetDriverDosName(const std::string& strDriver);
}
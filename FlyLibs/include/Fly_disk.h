#pragma once

#include <string> 
#include <Windows.h>
 
namespace Fly_disk {
	//��ֵ�̷� -����
	int MountVdiskNum(int  DeviceNumber, char DriveLetter);
	//��ֵ�̷� -�ļ�
	int MountVdiskFile(char * FileName, char DriveLetter);
	//ж���̷�
	int UmountVdisk(char DriveLetter);
	//�������ƶ����̻����
	bool EjectVolume(wchar_t cDriveLetter);
	//��ȡ���ô����б�
	std::string GetDrivers();

	//��ȡ������Ϣ
	//NOType  ����Ҫ���豸����   �� DRIVE_FIXED
	std::string getDriverInfo(int NOType = -1);
	//�����Ƿ����
	bool  DiskIsExist(const std::string& pctzDriver);
	//��ȡ����ʣ��ռ�
	unsigned long long GetFreeDiskSpaceX(const std::string& strDriver);
	//�ж��Ƿ��ǿ��ƶ����� �磺DRIVE_FIXED    32  �ж��Ƿ��ǿ��ƶ�Ӳ��
	unsigned int GetDriveTypes(const std::string& strDriver);
	//��ȡ��������   NTFS  FAT
	std::string GetVolumeInfo(const std::string& strVolume);
	//��ȡ���̵�dos�ַ��� eg: pctsDriver--c:  ���� "\\"
	std::string GetDriverDosName(const std::string& strDriver);
}
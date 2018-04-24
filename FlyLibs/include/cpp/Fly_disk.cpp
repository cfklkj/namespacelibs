/*******************************************************************************
  http://blog.csdn.net/cybermanlee/article/details/1600080

********************************************************************************/
#include "..\Fly_file.h" 
#include "..\Fly_string.h"  
 
#include <windows.h>
#include <winioctl.h> 
#include <tchar.h>
#define FILE_DEVICE_FILE_DISK 0x8000
#define IOCTL_FILE_DISK_OPEN_FILE   CTL_CODE(FILE_DEVICE_FILE_DISK, 0x800, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_FILE_DISK_CLOSE_FILE CTL_CODE(FILE_DEVICE_FILE_DISK, 0x801, METHOD_BUFFERED,FILE_READ_ACCESS | FILE_WRITE_ACCESS)


namespace Fly_disk {

	typedef struct _OPEN_FILE_INFORMATION {
		LARGE_INTEGER   FileSize;
		BOOLEAN       ReadOnly;
		USHORT       FileNameLength;
		CHAR       FileName[MAX_PATH];
	} OPEN_FILE_INFORMATION, *POPEN_FILE_INFORMATION;
#define DEVICE_NAME_PREFIXNUM "\\Device\\FileDisk\\FileDisk"
#define DEVICE_NAME_PREFIXFILE "\\GLOBAL??\\"

	int MountVdisk(
		int                     DeviceNumber,
		char * FileName,
		char                    DriveLetter
	)
	{
		char    VolumeName[] = "\\\\.\\ :";
		char    DeviceName[255];
		HANDLE Device;
		DWORD   BytesReturned;

		VolumeName[4] = DriveLetter; //处鋦理え?结á果?类え?似?"\\\\.\\D:"

		////////////////////////////////////////////////////////////////
		//测a试?该?虚é拟a卷í是?否?已?经-存?在ú，?
		//如?果?已?经-存?在ú
		//则ò输?出?错洙?误ó
		////////////////////////////////////////////////////////////////
		Device = CreateFileA(
			VolumeName,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			FILE_FLAG_NO_BUFFERING,
			NULL
		);

		if (Device != INVALID_HANDLE_VALUE)
		{
			SetLastError(ERROR_BUSY);
			return -1;
		}

		////////////////////////////////////////////////////////////////
		//如?果?不?存?在ú
		//则ò开a始?建¨立ⅰ?虚é拟a盘ì符?
		//DEVICE_NAME_PREFIX=“°\Device\FileDisk\FileDisk”±
		////////////////////////////////////////////////////////////////

		if (DeviceNumber > -1)
			sprintf_s(DeviceName, DEVICE_NAME_PREFIXNUM "%u", DeviceNumber);
		else
			sprintf_s(DeviceName, DEVICE_NAME_PREFIXFILE "%s", FileName);

		////////////////////////////////////////////////////////////////
		//所ù有瓺到?VolumeName的?IO请?求ó都?将?被?重?定¨向ò到?DeviceName
		//执′行D到?这a里?，?在ú系μ统?中D应畖该?可é以?看′见?一?个?虚é拟a的?盘ì符?出?现?
		//但?是?还1不?能ü对?该?盘ì符?做?任?何?操ù作痢?
		////////////////////////////////////////////////////////////////

		if (!DefineDosDeviceA(
			DDD_RAW_TARGET_PATH,
			&VolumeName[4],
			DeviceName
		))
		{
			return -1;
		}
		return 0;
	}
	//赋值盘符 -文件
	int MountVdiskFile(char * FileName, char DriveLetter)
	{
		return MountVdisk(-1, FileName, DriveLetter);
	}
	//赋值盘符 -磁盘
	int MountVdiskNum(int  DeviceNumber, char DriveLetter)
	{
		if (DeviceNumber < 0)
			return false;
		return MountVdisk(DeviceNumber, NULL, DriveLetter);
	}

	int UmountVdisk(char DriveLetter)
	{
		char    VolumeName[] = "\\\\.\\ :";
		HANDLE Device;
		DWORD   BytesReturned;

		VolumeName[4] = DriveLetter;

		/*		Device = CreateFileA(
					VolumeName,
					GENERIC_READ | GENERIC_WRITE,
					FILE_SHARE_READ | FILE_SHARE_WRITE,
					NULL,
					OPEN_EXISTING,
					FILE_FLAG_NO_BUFFERING,
					NULL
					);*/
		Device = CreateFileA(
			VolumeName,
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			NULL,
			NULL
		);
		//////////////////////////////////////////////////////////////////
		//如?果?返う?回?的?是?INVALID_HANDLE_VALUE
		//则ò意癮味?着?该?虚é拟a盘ì不?存?在ú，?不?能ü执′行D/umount的?动ˉ作痢?
		//输?出?错洙?误ó提?示?以?后ó返う?回?
		//////////////////////////////////////////////////////////////////
		if (Device == INVALID_HANDLE_VALUE)
		{
			//char device[1024];
			//QueryDosDeviceA(&VolumeName[4], device, 1024); // (驱动器Z是用"subst"虚拟的)来查询映射的路径
			//DefineDosDeviceA(DDD_RAW_TARGET_PATH, "J:", "");
			//int a = GetLastError();
			return -1;
		}
		//////////////////////////////////////////////////////////////////
		//如?果?返う?回?其?他?值μ，?则ò虚é拟a的?盘ì符?存?在ú
		//开a始?执′行D/umount的?动ˉ作痢?
		//包悒?括ぁ?以?下?几?个?步?骤è：阰
		// 1、￠锁?定¨当獭?前°卷í，?通?过y发ぁ?送íFSCTL_LOCK_VOLUME到?设Θ?备?驱y动ˉ实害?现?
		// 2、￠关?闭?所ù有瓺该?卷í上?打洙?开a的?所ù有瓺文?件t，?通?过y发ぁ?送íIOCTL_FILE_DISK_CLOSE_FILE到?设Θ?备?驱y动ˉ实害?现?
		// 3、￠卸?载?该?卷í，?通?过y发ぁ?送íFSCTL_DISMOUNT_VOLUME到?设Θ?备?驱y动ˉ实害?现?
		// 4、￠解a除y该?卷í的?锁?定¨，?通?过y发ぁ?送íFSCTL_UNLOCK_VOLUME到?设Θ?备?驱y动ˉ实害?现?
		// 5、￠关?闭?设Θ?备?
		// 6、￠删?除y虚é拟a盘ì符?
		//////////////////////////////////////////////////////////////////


		//锁?定¨需è要癮卸?载?的?卷í
		if (!DeviceIoControl(
			Device,
			FSCTL_LOCK_VOLUME,
			NULL,
			0,
			NULL,
			0,
			&BytesReturned,
			NULL
		))
		{
			return -1;
		}

		//关?闭?该?卷í上?打洙?开a的?所ù有瓺文?件t
		if (!DeviceIoControl(
			Device,
			IOCTL_FILE_DISK_CLOSE_FILE,
			NULL,
			0,
			NULL,
			0,
			&BytesReturned,
			NULL
		))
		{
			return -1;
		}
		//卸?载?该?卷í
		if (!DeviceIoControl(
			Device,
			FSCTL_DISMOUNT_VOLUME,
			NULL,
			0,
			NULL,
			0,
			&BytesReturned,
			NULL
		))
		{
			return -1;
		}
		//解a除y对?该?卷í的?锁?定¨
		if (!DeviceIoControl(
			Device,
			FSCTL_UNLOCK_VOLUME,
			NULL,
			0,
			NULL,
			0,
			&BytesReturned,
			NULL
		))
		{
			return -1;
		}
		////////////////////////////////////////////////////////////
		//关?闭?设Θ?备?HANDLE
		//执′行D到?这a里?，?系μ统?中D应畖该?还1是?可é以?看′到?一?个?虚é拟a的?盘ì符?
		//但?是?已?经-不?能ü对?该?盘ì符?做?任?何?操ù作痢?了?
		////////////////////////////////////////////////////////////
		CloseHandle(Device);

		///////////////////////////////////////////////////////////
		//删?除y虚é拟a盘ì符?
		//执′行D完?该?操ù作痢?，?系μ统?中D看′到?的?盘ì符?应畖该?消?失骸?
		///////////////////////////////////////////////////////////
		if (!DefineDosDeviceA(
			DDD_REMOVE_DEFINITION,
			&VolumeName[4],
			NULL
		))
		{
			return -1;
		}

		return 0;
	}


	// -------------获取磁盘号、判断是gpt还是mbr

	// GPT各分区类型的GUID
	//#define PARTITION_SYSTEM_GUID	c12a7328-f81f-11d2-ba4b-00a0c93ec93b
	DEFINE_GUID(PARTITION_SYSTEM_GUID, 0xc12a7328L, 0xf81f, 0x11d2, 0xba, 0x4b, 0x00, 0xa0, 0xc9, 0x3e, 0xc9, 0x3b);


	//#define PARTITION_MSFT_RESERVED_GUID	e3c9e316-0b5c-4db8-817d-f92df00215ae
	DEFINE_GUID(PARTITION_MSFT_RESERVED_GUID, 0xe3c9e316L, 0x0b5c, 0x4db8, 0x81, 0x7d, 0xf9, 0x2d, 0xf0, 0x02, 0x15, 0xae);


	//#define PARTITION_MSFT_RECOVERY_GUID  de94bba4-06d1-4d40-a16a-bfd50179d6ac
	DEFINE_GUID(PARTITION_MSFT_RECOVERY_GUID, 0xde94bba4L, 0x06d1, 0x4d40, 0xa1, 0x6a, 0xbf, 0xd5, 0x01, 0x79, 0xd6, 0xac);


	// 磁盘信息
	struct PTInfo {
		int Pid;  // 分区id
		int Did; // 设备id，只是编号而已
		bool Boot;  //是否活动
		// 是否GPT分区
		bool isGPT;
	};

	// -------------获取磁盘号、判断是gpt还是mbr 
	bool  getPartitionInfo(char sn, PTInfo* info) {
		char buf[] = "\\\\.\\?:";
		buf[4] = sn; // 传入盘符  
		DWORD dwSize = 0;
		HANDLE hFile;
		PARTITION_INFORMATION_EX  dg; // 硬盘分区信息
		STORAGE_DEVICE_NUMBER sdn; // 存储设备信息

		//////////////////获取设备句柄//////////////////
		// 在win8系统下（有多个1G左右的分区），调用失败，导致找不到X盘
		// 访问权限若不为空，则打开GPT分区后的设备操作会失败，导致找不到系统分区
		hFile = CreateFileA(buf, NULL, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
			OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE) {
			printf("err-getPt -1");;
			return false;
		}

		//第二个参数是对设备所指定的操作
		if (!DeviceIoControl(hFile, IOCTL_STORAGE_GET_DEVICE_NUMBER, NULL, 0, &sdn,
			sizeof(sdn), &dwSize, NULL)) {
			printf("err-getPt -2");
			CloseHandle(hFile);
			return false;
		}

		info->Did = sdn.DeviceNumber; // 设备号
		info->Did += 1;
		info->Pid = sdn.PartitionNumber; // 分区号
		info->isGPT = false;


		if (!DeviceIoControl(hFile, IOCTL_DISK_GET_PARTITION_INFO_EX, NULL, 0, &dg,
			sizeof(dg), &dwSize, NULL)) {
			CloseHandle(hFile);
			printf("err-getPt -3\n");
			return true;
		}
		if (dg.PartitionStyle == PARTITION_STYLE_MBR) {
			PARTITION_INFORMATION_MBR  piMbr = dg.Mbr;
			info->Boot = piMbr.BootIndicator;
			info->isGPT = false;
		}
		else if (dg.PartitionStyle == PARTITION_STYLE_GPT) {
			PARTITION_INFORMATION_GPT   piGpt = dg.Gpt;

			/*if (piGpt.PartitionType == PARTITION_SYSTEM_GUID){
				info->Boot = true;
				printf("-p2\n");
				return false;
			}
			else if (piGpt.PartitionType == PARTITION_MSFT_RESERVED_GUID){
				info->Boot = false;
			}
			else if (piGpt.PartitionType == PARTITION_MSFT_RECOVERY_GUID){
				info->Boot = false;
			}
			else {
				info->Boot = false;
			}*/
			info->isGPT = true;
		}

		// 判断目标磁盘是否准备就绪     如果不可读写则返回失败
		DWORD BytesReturned;
		DISK_GEOMETRY_EX dge;
		BOOL res = DeviceIoControl(hFile, // device to be queried
			IOCTL_DISK_GET_DRIVE_GEOMETRY_EX, // operation to perform
			NULL, 0, // no input buffer
			&dge, sizeof(dge),     // output buffer
			&BytesReturned,                 // # bytes returned
			(LPOVERLAPPED)NULL); // synchronous I/O
		if (!res) {
			CloseHandle(hFile);
			printf("err-getPt -4\n");
			return true;
		}
		if (0 == (long long)dge.DiskSize.QuadPart) { //DiskSize.QuadPart  is too long  to use (long long)
			CloseHandle(hFile);
			printf("err-getPt -5\n");
			return false;
		}

		CloseHandle(hFile);
		return true;
	}
	//获取磁盘详细信息，可判断磁盘是否可用
	HRESULT GetDiskSize(LPCTSTR pszDrive, BOOL FreeSize, BOOL DiskExis)  //DiskExis判断磁盘是否可用
	{
		__int64 i64FreeBytesToCaller, i64TotalBytes, i64FreeBytes;
		BOOL hResult = false;

		hResult = GetDiskFreeSpaceEx(pszDrive,
			(PULARGE_INTEGER)&i64FreeBytesToCaller,
			(PULARGE_INTEGER)&i64TotalBytes,
			(PULARGE_INTEGER)&i64FreeBytes);

		if (DiskExis)
		{
			return hResult;
		}
		else
		{
			return (FreeSize ? (DWORD)(i64FreeBytes >> 20) : (DWORD)(i64TotalBytes >> 20));
		}
	}
	//磁盘是否可用
	bool  DiskIsExist(const std::string& pDirectory)
	{
		return GetDiskSize(pDirectory.c_str(), false, true);
	}

	//获取可用磁盘列表
	std::string GetDrivers()
	{
		std::string rstBuff = "";
		int DSLength = GetLogicalDriveStringsA(0, NULL);    //通过GetLogicalDriveStrings()函数获取所有驱动器字符串信息长度。
		char* DStr = new char[DSLength + 1];//用获取的长度在堆区创建一个c风格的字符串数组

		GetLogicalDriveStringsA(DSLength, DStr); //(LPTSTR)   new delete  类型注意事项
		int DScount = DSLength / 4;
		for (int i = 0; i < DScount; ++i)
		{ 
			if (DiskIsExist(&DStr[i * 4]))
			{
				if (i > 0)
					rstBuff += Fly_file::GetKeysBreak();
				rstBuff += &DStr[i * 4];
			} 
		}
		return rstBuff;
	}
	//获取磁盘信息
	std::string getDriverInfo(int NOType)
	{
		std::string rstBuff = "";
		int DSLength = GetLogicalDriveStrings(0, NULL);    //通过GetLogicalDriveStrings()函数获取所有驱动器字符串信息长度。
		char* DStr = new char[DSLength + 1];//用获取的长度在堆区创建一个c风格的字符串数组

		GetLogicalDriveStringsA(DSLength, DStr); //(LPTSTR)   new delete  类型注意事项
		int DScount = DSLength / 4;
		char writeBuff[2048];
		char DTypes[][24] = { "硬盘", "光驱", "可移动式磁盘", "网络磁盘", "虚拟RAM磁盘", "未知设备" };
		int step = 1;
		for (int i = 0; i < DScount; ++i)

			//为了显示每个驱动器的状态，则通过循环输出实现，由于DStr内部保存的数据是A:\NULLB:\NULLC:\NULL，这样的信息，所以DSLength/4可以获得具体大循环范围

		{
			PTInfo ptInfo;
			if (!getPartitionInfo(DStr[i * 4], &ptInfo))
			{
				printf("%c\n", DStr[i * 4]);
				continue;
			}

			char dir[4] = { DStr[i * 4], ':', '\\', 0 };

			//GetDriveType函数，可以获取驱动器类型，参数为驱动器的根目录 
			int  DType = GetDriveTypeA(&DStr[i * 4]);// + i * 4

			if (NOType > -1 && DType != NOType) //DRIVE_FIXED
				continue;

			unsigned _int64 i64FreeBytesToCaller;

			unsigned _int64 i64TotalBytes;

			unsigned _int64 i64FreeBytes;
			bool  fResult = GetDiskFreeSpaceExA(

				dir,

				(PULARGE_INTEGER)&i64FreeBytesToCaller,

				(PULARGE_INTEGER)&i64TotalBytes,

				(PULARGE_INTEGER)&i64FreeBytes);

			//GetDiskFreeSpaceEx函数，可以获取驱动器磁盘的空间状态,函数返回的是个BOOL类型数据

			if (fResult)//通过返回的BOOL数据判断驱动器是否在工作状态   A / 1024/ 1024 = A >> 20 | A / 1024/ 1024 /1024 = A >> 30

			{
				char fTotalBytes[64], fFreeBytesToCaller[64];
				if (i64TotalBytes >> 20 > 1024)
					//cout<<" totalspace:"<<(float)i64TotalBytes/1024/1024/1024<<" GB";//磁盘总容量
					sprintf_s(fTotalBytes, "%.2fG", (float)(i64TotalBytes >> 30));
				else
				{
					//cout<<" totalspace:"<<(float)i64TotalBytes/1024/1024<<" MB";//磁盘总容量
					sprintf_s(fTotalBytes, "%.2fM", (float)(i64TotalBytes >> 20));
					if (i64TotalBytes >> 20 < 500)
						continue;
				}

				if (i64FreeBytesToCaller >> 20 > 1024)
					//	cout<<" Freetotalspace:"<<(float)i64FreeBytesToCaller/1024/1024/1024<<" GB";//磁盘剩余空间
					sprintf_s(fFreeBytesToCaller, "%.2fG", (float)(i64FreeBytesToCaller >> 30));
				else
					//	cout<<" Freetotalspace:"<<(float)i64FreeBytesToCaller/1024/1024<<" MB";//磁盘剩余空间  
					sprintf_s(fFreeBytesToCaller, "%.2fM", (float)(i64FreeBytesToCaller >> 20));


				DWORD   VolumeSerialNumber = 0;
				char   VolumeName[64];
				GetVolumeInformationA(dir, VolumeName, 64, &VolumeSerialNumber, NULL, NULL, NULL, 10);

				sprintf_s(writeBuff, "%c:|%d:%d|%s|%s|%s|%s|%s|%s\n", DStr[i * 4], ptInfo.Did, ptInfo.Pid, DTypes[DType], (VolumeName[0] != 0 ? VolumeName : "本地磁盘"), fTotalBytes, fFreeBytesToCaller, (ptInfo.isGPT ? "是" : "否"), (ptInfo.Boot ? "A" : "B"));
#ifdef _DEBUG
				printf("%s\r\n", writeBuff);
#endif
				rstBuff += writeBuff;
			}
		}
		delete[] DStr;
		return rstBuff;
	}
	//获取磁盘剩余空间
	ULONGLONG GetFreeDiskSpaceX(const std::string&  strDirectory)
	{
		static BOOL _bInitialized = FALSE;
		static BOOL(WINAPI *_pfnGetDiskFreeSpaceEx)(LPCTSTR, PULARGE_INTEGER, PULARGE_INTEGER, PULARGE_INTEGER) = NULL;
		const char* pDirectory = strDirectory.c_str();
		if (!_bInitialized)
		{
			_bInitialized = TRUE;
			(FARPROC&)_pfnGetDiskFreeSpaceEx = GetProcAddress(GetModuleHandle("kernel32"), "GetDiskFreeSpaceExW");
		}

		if (_pfnGetDiskFreeSpaceEx)
		{
			ULARGE_INTEGER nFreeDiskSpace;
			ULARGE_INTEGER dummy;
			if ((*_pfnGetDiskFreeSpaceEx)(pDirectory, &nFreeDiskSpace, &dummy, &dummy) == TRUE)
				return nFreeDiskSpace.QuadPart;
			else
				return 0;
		}
		else
		{
			TCHAR cDrive[MAX_PATH];
			const TCHAR *p = _tcschr(pDirectory, _T('\\'));
			if (p)
			{
				size_t uChars = p - pDirectory;
				if (uChars >= _countof(cDrive))
					return 0;
				memcpy(cDrive, pDirectory, uChars * sizeof(TCHAR));
				cDrive[uChars] = _T('\0');
			}
			else
			{
				size_t uChars = _tcslen(pDirectory);
				if (uChars >= _countof(cDrive))
					return 0;
				memcpy(cDrive, pDirectory, uChars);
				cDrive[uChars] = _T('\0');
			}
			DWORD dwSectPerClust, dwBytesPerSect, dwFreeClusters, dwDummy;
			if (GetDiskFreeSpace(cDrive, &dwSectPerClust, &dwBytesPerSect, &dwFreeClusters, &dwDummy))
				return (ULONGLONG)dwFreeClusters * (ULONGLONG)dwSectPerClust * (ULONGLONG)dwBytesPerSect;
			else
				return 0;
		}
	}
	//获取磁盘类型   NTFS  FAT
	std::string GetVolumeInfo(const std::string& pszVolume)
	{ 
		if (pszVolume.size() > 0 && pszVolume[pszVolume.size() - 1] == _T('\\'))
			;
		else
			return "";

		// 'GetVolumeInformation' may cause a noticable delay - depending on the type of volume 
		// which is queried. As we are using that function for almost every file (for compensating
		// the NTFS file time issues), we need to cash this information.
		//
		// The cash gets cleared when the user manually hits the 'Reload' button in the 'Shared
		// Files' window and when Windows broadcasts a message about that a volume was mounted/unmounted.
		//
		DWORD dwMaximumComponentLength = 0;
		DWORD dwFileSystemFlags = 0;
		TCHAR szFileSystemNameBuffer[MAX_PATH + 1] = { 0 };
		if (!GetVolumeInformation(pszVolume.c_str(), NULL, 0, NULL, &dwMaximumComponentLength, &dwFileSystemFlags, szFileSystemNameBuffer, _countof(szFileSystemNameBuffer)))
			szFileSystemNameBuffer[0] = _T('\0');
		return szFileSystemNameBuffer;
	}
	//判断是否是可移动磁盘
	UINT GetDriveTypes(const std::string& strDriver)
	{
		const char*  ptzDriver = strDriver.c_str();
		if (!ptzDriver)
		{
			return NULL;
		}
		TCHAR szVolumeName[8] = TEXT("\\\\.\\C:");
		UINT uDriveType = GetDriveType(ptzDriver);
		if ((uDriveType == DRIVE_FIXED) || (uDriveType == DRIVE_REMOVABLE))
		{
			DWORD dwAccessFlags = GENERIC_READ | GENERIC_WRITE;
			szVolumeName[4] = *ptzDriver;
			HANDLE hDevice = CreateFile(szVolumeName,
				dwAccessFlags,
				FILE_SHARE_READ | FILE_SHARE_WRITE,
				NULL,
				OPEN_EXISTING,
				0,
				NULL);
			if (hDevice != INVALID_HANDLE_VALUE)
			{
				STORAGE_HOTPLUG_INFO stu_HOTPLUG;
				DWORD lngRet;
				if (DeviceIoControl(hDevice,
					IOCTL_STORAGE_GET_HOTPLUG_INFO,
					NULL,
					0,
					&stu_HOTPLUG,
					sizeof(STORAGE_HOTPLUG_INFO),
					&lngRet,
					NULL))
				{
					if (stu_HOTPLUG.DeviceHotplug)
					{
						if (DRIVE_FIXED == uDriveType)
							uDriveType = uDriveType * 10 + DRIVE_REMOVABLE;
					}
				}
				CloseHandle(hDevice);
			}
		}
		return uDriveType;
	}
	//获取磁盘的dos字符串
	std::string GetDriverDosName(const std::string& pctsDriver)
	{
		TCHAR tzDos[MAX_PATH] = { 0 };
		if (QueryDosDevice(pctsDriver.c_str(), tzDos, MAX_PATH))
		{ 
			return tzDos;
		}
		return "";
	}

	/////////////////////////////////////////////////////////////////////////////
	//------start 弹出+加载可移动磁盘
	TCHAR szVolumeName[8] = TEXT("\\\\.\\C:");
	HANDLE OpenVolume(TCHAR cDriveLetter)
	{
		HANDLE hVolume;
		UINT uDriveType = 0;
		TCHAR szRootName[5] = TEXT("C:\\");
		DWORD dwAccessFlags;

		szRootName[0] = cDriveLetter;

		uDriveType = GetDriveTypes(szRootName);
		switch (uDriveType)
		{
		case 32:
		case DRIVE_REMOVABLE:
			dwAccessFlags = GENERIC_READ | GENERIC_WRITE;
			break;
		case DRIVE_CDROM:
			dwAccessFlags = GENERIC_READ;
			break;
		default:
			return INVALID_HANDLE_VALUE; //不能弹出的设备
		}

		szVolumeName[4] = cDriveLetter;

		hVolume = CreateFile(szVolumeName,
			dwAccessFlags,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);

		return hVolume;
	}
	/////////////////////////////////////////////////////////////////////////////
#define LOCK_TIMEOUT  10000 // 10 Seconds
#define RETRY				20
	BOOL LockVolume(HANDLE hVolume)
	{
		DWORD dwBytesReturned;
		DWORD dwSleepAmount;
		int nTryCount;

		dwSleepAmount = LOCK_TIMEOUT / RETRY;

		// Do this in a loop until a timeout period has expired
		for (nTryCount = 0; nTryCount < RETRY; nTryCount++)
		{
			if (DeviceIoControl(hVolume,
				FSCTL_LOCK_VOLUME,
				NULL, 0,
				NULL, 0,
				&dwBytesReturned,
				NULL))
			{
				return TRUE;
			}
			Sleep(dwSleepAmount);
		}
		return FALSE;
	}
	/////////////////////////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////////////////////////
	BOOL DismountVolume(HANDLE hVolume)
	{
		DWORD dwBytesReturned;
		return DeviceIoControl(hVolume,
			FSCTL_DISMOUNT_VOLUME,
			NULL, 0,
			NULL, 0,
			&dwBytesReturned,
			NULL);
	}
	/////////////////////////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////////////////////////
	BOOL PreventRemovalOfVolume(HANDLE hVolume, BOOL fPreventRemoval)
	{
		DWORD dwBytesReturned;
		PREVENT_MEDIA_REMOVAL PMRBuffer;
		PMRBuffer.PreventMediaRemoval = fPreventRemoval;
		return DeviceIoControl(hVolume,
			IOCTL_STORAGE_MEDIA_REMOVAL,
			&PMRBuffer, sizeof(PREVENT_MEDIA_REMOVAL),
			NULL, 0,
			&dwBytesReturned,
			NULL);
	}
	/////////////////////////////////////////////////////////////////////////////


	BOOL MediaExist(HANDLE hVolume)
	{
		DWORD dwBytesReturned;
		return DeviceIoControl(hVolume,
			IOCTL_DISK_CHECK_VERIFY,
			NULL, 0, NULL, 0,
			&dwBytesReturned, NULL);
	}

	/////////////////////////////////////////////////////////////////////////////
	BOOL AutoEjectVolume(HANDLE hVolume)
	{
		DWORD dwBytesReturned;

		return DeviceIoControl(hVolume,
			IOCTL_STORAGE_EJECT_MEDIA,
			NULL, 0,
			NULL, 0,
			&dwBytesReturned,
			NULL);
	}
	/////////////////////////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////////////////////////
	//弹出可移动磁盘或光驱
	bool EjectVolume(wchar_t cDriveLetter)
	{
		HANDLE hVolume;
		BOOL hResult;

		// Open the volume.
		hVolume = OpenVolume(cDriveLetter);
		//hVolume = OpenVolume((TCHAR)cDriveLetter);
		if (hVolume == INVALID_HANDLE_VALUE)
		{
			return FALSE;
		}

		if (MediaExist(hVolume))
		{
			// Lock and dismount the volume.
			if (LockVolume(hVolume) && DismountVolume(hVolume))
			{
				//PreventRemovalOfVolume(hVolume,FALSE);
				hResult = AutoEjectVolume(hVolume);
				UINT uDriveType;
				TCHAR szRootName[5] = TEXT("C:\\");

				szRootName[0] = cDriveLetter;
				uDriveType = GetDriveTypes(szRootName);
				if ((uDriveType == 2 || uDriveType == 32) && hResult) //去除可移动磁盘信息
				{
					TCHAR    VolumeName[] = TEXT("\\\\.\\_:");
					VolumeName[4] = cDriveLetter;
					//VolumeName[4] = (TCHAR)cDriveLetter;
					hResult = DefineDosDevice(DDD_REMOVE_DEFINITION, &VolumeName[4], NULL);
				}
			}
			else
			{
				hResult = FALSE;
			}
			CloseHandle(hVolume);
			return hResult;
		}
		else
		{
			return TRUE;
		}
	}
	//------end 弹出+加载可移动磁盘
}
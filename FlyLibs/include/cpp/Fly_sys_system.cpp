#include "..\Fly_sys.h"  
#include "..\Fly_web.h"
#include "..\Fly_string.h"
#include "..\Fly_Time.h"
#include <Shlwapi.h>
#include <time.h>
 

namespace Fly_sys {
	namespace System {//提升进程访问权限  
		bool Priv(PCTSTR ptzName)
		{
			HANDLE hToken;
			TOKEN_PRIVILEGES tkp;

			if (!OpenProcessToken(GetCurrentProcess(),
				TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
				return false;
			}
			if (!LookupPrivilegeValue(NULL, ptzName, &tkp.Privileges[0].Luid)) {
				CloseHandle(hToken);
				return false;
			}
			tkp.PrivilegeCount = 1;
			tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL)) {
				CloseHandle(hToken);
				return false;
			}
			return true;
		}
		//关闭、重启系统
		HRESULT bootSystem(bool bReboot)
		{
#ifdef _WIN64
			DWORD_PTR dwResult;
#else
			DWORD dwResult;
#endif
			SendMessageTimeout(HWND_BROADCAST, WM_QUERYENDSESSION, 0, 0, 0, 2000, &dwResult);
			SendMessageTimeout(HWND_BROADCAST, WM_ENDSESSION, 0, 0, 0, 2000, &dwResult);
		
			//SendMessageTimeout(HWND_BROADCAST, WM_CLOSE, 0, 0, 0, 2000, &dwResult);
		
			SendMessageTimeout(HWND_BROADCAST, WM_DESTROY, 0, 0, 0, 2000, &dwResult);
		
			// Get function address
			typedef DWORD(NTAPI *PNtShutdownSystem)(DWORD dwAction);
			PNtShutdownSystem NtShutdownSystem = (PNtShutdownSystem)GetProcAddress(GetModuleHandle(TEXT("NTDLL")), "NtShutdownSystem");
			if (!NtShutdownSystem)
			{
				return E_FAIL;
			}
			// Shutdown
			Priv(SE_SHUTDOWN_NAME);
			return NtShutdownSystem(bReboot ? 1 : 2);
		}
		//是否是64位系统
		bool is64bitSystem()
		{
			SYSTEM_INFO si;
			GetNativeSystemInfo(&si);
			if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
				si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
				return true;
			else
				return false;
		}
		//获取系统任务栏状态位置有四种情况：左(0)、上(1)、右(2)、下(3)，此外要考虑任务栏自动隐藏(5)的情况
		//return pos
		int getTaskbarPos(int &width, int &height)
		{
			int wx = GetSystemMetrics(SM_CXSCREEN);
			int wy = GetSystemMetrics(SM_CYSCREEN);
			RECT rtWorkArea;
			int pos = -1;
			SystemParametersInfo(SPI_GETWORKAREA, 0, &rtWorkArea, 0);
			//1.任务栏停靠在左边情况
			if (0 != rtWorkArea.left)
			{
				width = wx - (rtWorkArea.left - rtWorkArea.right);// rtWorkArea.Width();
				height = wy;
				pos = 0;
				return pos;
			}
			//2.任务栏停靠在上边情况
			if (0 != rtWorkArea.top)
			{
				width = wx;
				height = wy - (rtWorkArea.top - rtWorkArea.bottom);//rtWorkArea.Height();
				pos = 1;
				return pos;
			}
			//3.任务栏停靠在右边情况
			if (0 == rtWorkArea.left && wx != (rtWorkArea.left - rtWorkArea.right))
			{
				width = wx - (rtWorkArea.left - rtWorkArea.right);
				height = wy;
				pos = 2;
				return pos;
			}
			//4.任务栏停靠在下边情况
			if (0 == rtWorkArea.top && wy != (rtWorkArea.top - rtWorkArea.bottom))
			{
				width = wx;
				height = wy - (rtWorkArea.top - rtWorkArea.bottom);
				pos = 3;
				return pos;
			}
			//5.任务栏自动隐藏的情况，这样其宽高都是0
			if (0 == rtWorkArea.left && 0 == rtWorkArea.top
				&& wx == (rtWorkArea.left - rtWorkArea.right) && wy == (rtWorkArea.top - rtWorkArea.bottom))
			{
				width = 0;
				height = 0;
				pos = 4;
				return pos;
			}
			return pos;
		}
		std::string getSystemName()
		{
			std::string vname;
			//先判断是否为win8.1或win10  
			typedef void(__stdcall*NTPROC)(DWORD*, DWORD*, DWORD*);
			HINSTANCE hinst = LoadLibraryA("ntdll.dll");
			DWORD dwMajor, dwMinor, dwBuildNumber;
			NTPROC proc = (NTPROC)GetProcAddress(hinst, "RtlGetNtVersionNumbers");
			proc(&dwMajor, &dwMinor, &dwBuildNumber);
			if (dwMajor == 6 && dwMinor == 3)   //win 8.1  
			{
				vname = "win8";
				return vname;
			}
			if (dwMajor == 10 && dwMinor == 0)  //win 10  
			{
				vname = "win10";
				return vname;
			}
			//下面判断不能Win Server，因为本人还未有这种系统的机子，暂时不给出   

			//判断win8.1以下的版本  
			SYSTEM_INFO info;                //用SYSTEM_INFO结构判断64位AMD处理器    
			GetSystemInfo(&info);            //调用GetSystemInfo函数填充结构    
			OSVERSIONINFOEX os;
			os.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
#pragma warning(disable:4996)
			if (GetVersionEx((OSVERSIONINFO *)&os))
			{

				//下面根据版本信息判断操作系统名称    
				switch (os.dwMajorVersion)
				{                        //判断主版本号    
				case 4:
					switch (os.dwMinorVersion)
					{                //判断次版本号    
					case 0:
						if (os.dwPlatformId == VER_PLATFORM_WIN32_NT)
							vname = "winNT4.0";  //1996年7月发布    
						else if (os.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
							vname = "win95";
						break;
					case 10:
						vname = "win98";
						break;
					case 90:
						vname = "winMe";
						break;
					}
					break;
				case 5:
					switch (os.dwMinorVersion)
					{               //再比较dwMinorVersion的值    
					case 0:
						vname = "win2000";    //1999年12月发布    
						break;
					case 1:
						vname = "winXP";      //2001年8月发布    
						break;
					case 2:
						if (os.wProductType == VER_NT_WORKSTATION &&
							info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
							vname = "Microsoft Windows XP Professional x64 Edition";
						else if (GetSystemMetrics(SM_SERVERR2) == 0)
							vname = "win2003";   //2003年3月发布    Microsoft Windows Server
						else if (GetSystemMetrics(SM_SERVERR2) != 0)
							vname = "win2003R2";
						break;
					}
					break;
				case 6:
					switch (os.dwMinorVersion)
					{
					case 0:
						if (os.wProductType == VER_NT_WORKSTATION)
							vname = "winVista";
						else
							vname = "win2008";   //服务器版本    
						break;
					case 1:
						if (os.wProductType == VER_NT_WORKSTATION)
							vname = "win7";
						else
							vname = "win2008R2";
						break;
					case 2:
						if (os.wProductType == VER_NT_WORKSTATION)
							vname = "win8";
						else
							vname = "win2012";
						break;
					}
					break;
				default:
					vname = "未知操作系统";
				}
			}
			else
				vname = "版本获取失败";
			return vname;
		}
		//设置桌面背景
		bool  setBkGround(std::string& strPath)
		{
			const char* jpgPath = strPath.c_str();
			if (GetFileAttributes(jpgPath) == INVALID_FILE_ATTRIBUTES)
				return false; 
			SHSetValue(HKEY_CURRENT_USER, TEXT("Control Panel\\Desktop"), TEXT("Wallpaper"), REG_SZ, (PTSTR)strPath.c_str(), strPath.size());
			return SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (PVOID)jpgPath, SPIF_SENDCHANGE);
		}
		//分辨率
		//获取分辨率信息
		static int  s_oldX = 0, s_oldY = 0;
		std::string getDisplay()
		{
			DEVMODE NewDevMode;
			NewDevMode.dmSize = sizeof(DEVMODE);
			int iMode = 0;
			std::string rstDisplay = "";
			char tempBuff[128];
			int  oldX = 0, oldY = 0;
			while (EnumDisplaySettings(NULL, iMode++, &NewDevMode))
			{
				if (oldX != NewDevMode.dmPelsWidth || oldY != NewDevMode.dmPelsHeight)
				{
					oldX = NewDevMode.dmPelsWidth;
					oldY = NewDevMode.dmPelsHeight;
					if (s_oldX < NewDevMode.dmPelsWidth)
					{
						s_oldX = NewDevMode.dmPelsWidth;
						s_oldY = NewDevMode.dmPelsHeight;
					}
					sprintf_s(tempBuff, "%d*%d\n", NewDevMode.dmPelsWidth, NewDevMode.dmPelsHeight);
					rstDisplay += tempBuff;
				}
			}
			//最佳分辨率
			sprintf_s(tempBuff, "bestPix:%d*%d", s_oldX, s_oldY);
			rstDisplay += tempBuff;
			//当前分辨率
			if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &NewDevMode))
			{
				sprintf_s(tempBuff, "\ncurrentPix:%d*%d", NewDevMode.dmPelsWidth, NewDevMode.dmPelsHeight);
				rstDisplay += tempBuff;
			}
			return rstDisplay;
		} 
		//设置分辨率
		bool setDisplay(int pixX, int pixY)
		{
			DEVMODE dmScreenSettings;                               // 设备模式  
			memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));   // 确保内存清空为零  

			dmScreenSettings.dmSize = sizeof(dmScreenSettings);     // DEVMODE结构的大小  
			if (pixX == 0 || pixY == 0)  //使用最佳分辨率
			{
				getDisplay();
				pixX = s_oldX;
				pixY = s_oldY;
			}
			dmScreenSettings.dmPelsWidth = pixX;                  // 所选屏幕宽度  
			dmScreenSettings.dmPelsHeight = pixY;                  // 所选屏幕高度
			dmScreenSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
			return DISP_CHANGE_SUCCESSFUL == ChangeDisplaySettings(&dmScreenSettings, CDS_RESET); //CDS_FULLSCREEN
		}
		//设置系统时间
		//[时间],[NTPIP|URL]   时间格式 2018-4-24 0:00:08
		bool setTime(std::string& strCmd)
		{ 
			time_t		ntp_time;
			SYSTEMTIME	newtime;
			float		Splitseconds;
			struct		tm	*lpLocalTime;

			const char* tempStr = strCmd.c_str();
			std::string localTime = Fly_string::GetSubStr(tempStr, ',', 1);
			if (!Fly_Time::TIME::stringToTime(localTime, ntp_time))
			{
				lpLocalTime = localtime(&ntp_time);
				if (lpLocalTime == NULL)
				{
					return FALSE;
				}
				// 获取新的时间
				newtime.wYear = lpLocalTime->tm_year + 1900;
				newtime.wMonth = lpLocalTime->tm_mon + 1;
				newtime.wDayOfWeek = lpLocalTime->tm_wday;
				newtime.wDay = lpLocalTime->tm_mday;
				newtime.wHour = lpLocalTime->tm_hour;
				newtime.wMinute = lpLocalTime->tm_min;
				newtime.wSecond = lpLocalTime->tm_sec;

				// 设置时间精度
				Splitseconds = (float)ntohl(0);
				Splitseconds = (float)0.000000000200 * Splitseconds;
				Splitseconds = (float)1000.0 * Splitseconds;
				newtime.wMilliseconds = (unsigned   short)Splitseconds;

				// 修改本机系统时间
				SetLocalTime(&newtime);
				return true;
			}

			std::string NTP = Fly_string::GetSubStr(tempStr, ',', 2);
			std::string IP = Fly_web::RAW::GetIpPortfromUrl(NTP.c_str());
		 	if (IP.empty())
		 		return false;
			WORD	wVersionRequested;
			WSADATA wsaData;

			// 初始化版本
			wVersionRequested = MAKEWORD(1, 1);
			if (0 != WSAStartup(wVersionRequested, &wsaData))
			{
				WSACleanup();
				return FALSE;
			}
			if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
			{
				WSACleanup();
				return FALSE;
			}

			struct   NTP_Packet
			{
				int      Control_Word;
				int      root_delay;
				int      root_dispersion;
				int      reference_identifier;
				__int64	reference_timestamp;
				__int64 originate_timestamp;
				__int64 receive_timestamp;
				int      transmit_timestamp_seconds;
				int      transmit_timestamp_fractions;
			};

			// 这个IP是中国大陆时间同步服务器地址，可自行修改
			SOCKET soc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			struct sockaddr_in addrSrv;
			addrSrv.sin_addr.S_un.S_addr = inet_addr(IP.c_str());
			addrSrv.sin_family = AF_INET;
			addrSrv.sin_port = htons(123);

			NTP_Packet NTP_Send, NTP_Recv;
			NTP_Send.Control_Word = htonl(0x0B000000);
			NTP_Send.root_delay = 0;
			NTP_Send.root_dispersion = 0;
			NTP_Send.reference_identifier = 0;
			NTP_Send.reference_timestamp = 0;
			NTP_Send.originate_timestamp = 0;
			NTP_Send.receive_timestamp = 0;
			NTP_Send.transmit_timestamp_seconds = 0;
			NTP_Send.transmit_timestamp_fractions = 0;

			if (SOCKET_ERROR == sendto(soc, (const char*)&NTP_Send, sizeof(NTP_Send),
				0, (struct sockaddr*)&addrSrv, sizeof(addrSrv)))
			{
				return FALSE;
			}
			int sockaddr_Size = sizeof(addrSrv);
			if (SOCKET_ERROR == recvfrom(soc, (char*)&NTP_Recv, sizeof(NTP_Recv),
				0, (struct sockaddr*)&addrSrv, &sockaddr_Size))
			{
				closesocket(soc);
				return FALSE;
			}
			closesocket(soc);
			WSACleanup();


			// 获取时间服务器的时间
			ntp_time = ntohl(NTP_Recv.transmit_timestamp_seconds) - 2208988800; 
			lpLocalTime = localtime(&ntp_time);
			if (lpLocalTime == NULL)
			{
				return FALSE;
			}
			// 获取新的时间
			newtime.wYear = lpLocalTime->tm_year + 1900;
			newtime.wMonth = lpLocalTime->tm_mon + 1;
			newtime.wDayOfWeek = lpLocalTime->tm_wday;
			newtime.wDay = lpLocalTime->tm_mday;
			newtime.wHour = lpLocalTime->tm_hour;
			newtime.wMinute = lpLocalTime->tm_min;
			newtime.wSecond = lpLocalTime->tm_sec;

			// 设置时间精度
			Splitseconds = (float)ntohl(NTP_Recv.transmit_timestamp_fractions);
			Splitseconds = (float)0.000000000200 * Splitseconds;
			Splitseconds = (float)1000.0 * Splitseconds;
			newtime.wMilliseconds = (unsigned   short)Splitseconds;

			// 修改本机系统时间
			SetLocalTime(&newtime);
			return TRUE;
		}
	}
}
 
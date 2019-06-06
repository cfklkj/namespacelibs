#include "Fly_sys.h"   
#include "Fly_string.h"
#include "Fly_Time.h"
#include <Shlwapi.h>
#include <time.h>
 

namespace Fly_sys {
	namespace System {//�������̷���Ȩ��  
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
		//�رա�����ϵͳ
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
		//�Ƿ���64λϵͳ
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
		//��ȡϵͳ������״̬λ���������������(0)����(1)����(2)����(3)������Ҫ�����������Զ�����(5)�����
		//return pos
		int getTaskbarPos(int &width, int &height)
		{
			int wx = GetSystemMetrics(SM_CXSCREEN);
			int wy = GetSystemMetrics(SM_CYSCREEN);
			RECT rtWorkArea;
			int pos = -1;
			SystemParametersInfo(SPI_GETWORKAREA, 0, &rtWorkArea, 0);
			//1.������ͣ����������
			if (0 != rtWorkArea.left)
			{
				width = wx - (rtWorkArea.left - rtWorkArea.right);// rtWorkArea.Width();
				height = wy;
				pos = 0;
				return pos;
			}
			//2.������ͣ�����ϱ����
			if (0 != rtWorkArea.top)
			{
				width = wx;
				height = wy - (rtWorkArea.top - rtWorkArea.bottom);//rtWorkArea.Height();
				pos = 1;
				return pos;
			}
			//3.������ͣ�����ұ����
			if (0 == rtWorkArea.left && wx != (rtWorkArea.left - rtWorkArea.right))
			{
				width = wx - (rtWorkArea.left - rtWorkArea.right);
				height = wy;
				pos = 2;
				return pos;
			}
			//4.������ͣ�����±����
			if (0 == rtWorkArea.top && wy != (rtWorkArea.top - rtWorkArea.bottom))
			{
				width = wx;
				height = wy - (rtWorkArea.top - rtWorkArea.bottom);
				pos = 3;
				return pos;
			}
			//5.�������Զ����ص��������������߶���0
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
			//���ж��Ƿ�Ϊwin8.1��win10  
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
			//�����жϲ���Win Server����Ϊ���˻�δ������ϵͳ�Ļ��ӣ���ʱ������   

			//�ж�win8.1���µİ汾  
			SYSTEM_INFO info;                //��SYSTEM_INFO�ṹ�ж�64λAMD������    
			GetSystemInfo(&info);            //����GetSystemInfo�������ṹ    
			OSVERSIONINFOEX os;
			os.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
#pragma warning(disable:4996)
			if (GetVersionEx((OSVERSIONINFO *)&os))
			{

				//������ݰ汾��Ϣ�жϲ���ϵͳ����    
				switch (os.dwMajorVersion)
				{                        //�ж����汾��    
				case 4:
					switch (os.dwMinorVersion)
					{                //�жϴΰ汾��    
					case 0:
						if (os.dwPlatformId == VER_PLATFORM_WIN32_NT)
							vname = "winNT4.0";  //1996��7�·���    
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
					{               //�ٱȽ�dwMinorVersion��ֵ    
					case 0:
						vname = "win2000";    //1999��12�·���    
						break;
					case 1:
						vname = "winXP";      //2001��8�·���    
						break;
					case 2:
						if (os.wProductType == VER_NT_WORKSTATION &&
							info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
							vname = "Microsoft Windows XP Professional x64 Edition";
						else if (GetSystemMetrics(SM_SERVERR2) == 0)
							vname = "win2003";   //2003��3�·���    Microsoft Windows Server
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
							vname = "win2008";   //�������汾    
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
					vname = "δ֪����ϵͳ";
				}
			}
			else
				vname = "�汾��ȡʧ��";
			return vname;
		}
		//�������汳��
		bool  setBkGround(std::string& strPath)
		{
			const char* jpgPath = strPath.c_str();
			if (GetFileAttributes(jpgPath) == INVALID_FILE_ATTRIBUTES)
				return false; 
			SHSetValue(HKEY_CURRENT_USER, TEXT("Control Panel\\Desktop"), TEXT("Wallpaper"), REG_SZ, (PTSTR)strPath.c_str(), strPath.size());
			return SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (PVOID)jpgPath, SPIF_SENDCHANGE);
		}
		//�ֱ���
		//��ȡ�ֱ�����Ϣ
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
			//��ѷֱ���
			sprintf_s(tempBuff, "bestPix:%d*%d", s_oldX, s_oldY);
			rstDisplay += tempBuff;
			//��ǰ�ֱ���
			if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &NewDevMode))
			{
				sprintf_s(tempBuff, "\ncurrentPix:%d*%d", NewDevMode.dmPelsWidth, NewDevMode.dmPelsHeight);
				rstDisplay += tempBuff;
			}
			return rstDisplay;
		} 
		//���÷ֱ���
		bool setDisplay(int pixX, int pixY)
		{
			DEVMODE dmScreenSettings;                               // �豸ģʽ  
			memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));   // ȷ���ڴ����Ϊ��  

			dmScreenSettings.dmSize = sizeof(dmScreenSettings);     // DEVMODE�ṹ�Ĵ�С  
			if (pixX == 0 || pixY == 0)  //ʹ����ѷֱ���
			{
				getDisplay();
				pixX = s_oldX;
				pixY = s_oldY;
			}
			dmScreenSettings.dmPelsWidth = pixX;                  // ��ѡ��Ļ����  
			dmScreenSettings.dmPelsHeight = pixY;                  // ��ѡ��Ļ�߶�
			dmScreenSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
			return DISP_CHANGE_SUCCESSFUL == ChangeDisplaySettings(&dmScreenSettings, CDS_RESET); //CDS_FULLSCREEN
		}
	}
}
 
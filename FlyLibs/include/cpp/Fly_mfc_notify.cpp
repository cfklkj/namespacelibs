#include <afxcontrolbars.h> 
#include "..\Fly_mfc.h" 

//#define WM_SYSTEMTRAY (WM_USER + 101)

namespace FLYLIB
{
	namespace MFC
	{
		namespace NotifyIco
		{
			struct STRNOTIFY{
				NOTIFYICONDATA NotifyIcon;  //系统托盘类 
				HICON	ico_A;
				HICON	ico_B;
				bool    isNotify;  
			};
			static STRNOTIFY Notifys;
			DWORD WINAPI NotiFysThread(LPVOID lparam)   //托盘闪动
			{
				while (Notifys.isNotify)
				{
					Sleep(700);
					Notifys.NotifyIcon.hIcon = Notifys.ico_B;
					Shell_NotifyIcon(NIM_MODIFY, &Notifys.NotifyIcon);
					Sleep(500);
					Notifys.NotifyIcon.hIcon = NULL;
					Shell_NotifyIcon(NIM_MODIFY, &Notifys.NotifyIcon);
				}
				Notifys.NotifyIcon.hIcon = Notifys.ico_A;
				Shell_NotifyIcon(NIM_MODIFY, &Notifys.NotifyIcon);
				return 1;
			}
			void SetNofify(HWND hWnd, UINT uCallbackMessage, std::string &tips)
			{
				//设置系统托盘 
				Notifys.NotifyIcon.hWnd = hWnd;
				Notifys.NotifyIcon.uCallbackMessage = uCallbackMessage;
				lstrcpy(Notifys.NotifyIcon.szTip, tips.substr(0, 128).c_str());
				Notifys.NotifyIcon.hIcon = Notifys.ico_A;
				Notifys.NotifyIcon.cbSize = sizeof(NOTIFYICONDATA);
				Notifys.NotifyIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP; 
			}
			bool InitNotifyIco(HWND hWnd, UINT uCallbackMessage, std::string& tips, std::string& NormallIco, std::string& NotifyIcon)
			{
				Notifys.ico_A = (HICON)LoadImage(NULL, NormallIco.c_str(), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
				Notifys.ico_B = (HICON)LoadImage(NULL, NotifyIcon.c_str(), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
				SetNofify(hWnd, uCallbackMessage, tips);
				return true;
			}
			bool InitNotifyIco(HWND hWnd, UINT uCallbackMessage, std::string& tips, UINT NormallIco, UINT NotifyIcon)
			{
				Notifys.ico_A = LoadIcon(NULL, MAKEINTRESOURCE(NormallIco));
				Notifys.ico_B = LoadIcon(NULL, MAKEINTRESOURCE(NotifyIcon));
				SetNofify(hWnd, uCallbackMessage, tips); 
				return true;
			}
			bool ShowNotify()
			{
				Notifys.NotifyIcon.hIcon = Notifys.ico_A;
				return Shell_NotifyIcon(NIM_ADD, &Notifys.NotifyIcon);   //添加系统托盘 
			}
			bool StartNotify(bool isNotify)
			{
				if (isNotify)
				{
					if (!Notifys.ico_A)
						return false;
					Notifys.isNotify = isNotify; 
					CreateThread(NULL, NULL, NotiFysThread, NULL, NULL, NULL);
				}
				else
					Notifys.isNotify = isNotify;
				return true;
			}
			bool UnNotify()
			{
				Notifys.isNotify = false;
				return Shell_NotifyIcon(NIM_DELETE, &Notifys.NotifyIcon);
			}
		}
	}
}
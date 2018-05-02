#include "stdafx.h"
#include <afxcontrolbars.h> 
#include "..\Fly_mfc.h" 
 
#include <atlbase.h>
#include <atlwin.h>
#include <windows.h> 
#pragma comment(lib, "User32.lib")

namespace Fly_mfc
{
	namespace DLG { 

		typedef struct
		{
			LPCTSTR	pszInitialDir;
			LPCTSTR	pszDlgTitle;
		} BROWSEINIT, *LPBROWSEINIT;

		extern "C" int CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM /*lParam*/, LPARAM lpData)
		{
			if (uMsg == BFFM_INITIALIZED)
			{
				// Set initial directory
				if (((LPBROWSEINIT)lpData)->pszInitialDir != NULL)
					SendMessage(hWnd, BFFM_SETSELECTION, TRUE, (LPARAM)((LPBROWSEINIT)lpData)->pszInitialDir);

				// Set dialog's window title
				if (((LPBROWSEINIT)lpData)->pszDlgTitle != NULL)
					SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)((LPBROWSEINIT)lpData)->pszDlgTitle);
			}

			return 0;
		}
		//浏览目录对话框 rstPath  返回选择的目录 
		bool SelectDir(HWND hWnd, LPTSTR pszPath, LPCTSTR pszTitle, LPCTSTR pszDlgTitle)
		{
			bool bResult = false;
			CoInitialize(0);
			LPMALLOC pShlMalloc;
			if (SHGetMalloc(&pShlMalloc) == NOERROR)
			{
				BROWSEINFO BrsInfo = { 0 };
				BrsInfo.hwndOwner = hWnd;
				BrsInfo.lpszTitle = (pszTitle != NULL) ? pszTitle : pszDlgTitle;
				BrsInfo.ulFlags = BIF_VALIDATE | BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS | BIF_SHAREABLE | BIF_DONTGOBELOWDOMAIN;

				BROWSEINIT BrsInit = { 0 };
				if (pszPath != NULL || pszTitle != NULL || pszDlgTitle != NULL) {
					// Need the 'BrowseCallbackProc' to set those strings
					BrsInfo.lpfn = BrowseCallbackProc;
					BrsInfo.lParam = (LPARAM)&BrsInit;
					BrsInit.pszDlgTitle = (pszDlgTitle != NULL) ? pszDlgTitle : NULL/*pszTitle*/;
					BrsInit.pszInitialDir = pszPath;
				}

				LPITEMIDLIST pidlBrowse;
				if ((pidlBrowse = SHBrowseForFolder(&BrsInfo)) != NULL) {
					if (SHGetPathFromIDList(pidlBrowse, pszPath))
						bResult = true;
					pShlMalloc->Free(pidlBrowse);
				}
				pShlMalloc->Release();
			}
			CoUninitialize();
			return bResult;
		}
		//置顶窗体
		bool SetTop(HWND hDlg, bool isDeep)
		{
			CRect rectDlg;
			GetWindowRect(hDlg, &rectDlg);
			SetForegroundWindow(hDlg);
			return SetWindowPos(hDlg, !isDeep ? HWND_TOPMOST : HWND_BOTTOM,
				rectDlg.left, rectDlg.top, rectDlg.right, rectDlg.bottom, SWP_NOSIZE);
		}
		//是否有窗体全屏
		bool HaveWindowsFull()
		{
			HWND hwnd = GetForegroundWindow();
			wchar_t buff[MAX_PATH];
			GetWindowText(hwnd, buff, MAX_PATH);
			CRect rect;
			GetWindowRect(hwnd, &rect);
			int width = GetSystemMetrics(SM_CXSCREEN);
			int height = GetSystemMetrics(SM_CYSCREEN);
			if ((rect.Width() == width && rect.Height() == height) && buff[0] != '\0')
			{
				return true;
			}
			return false;
		}
	}
	namespace IMG {

		int GetDesktopColorDepth()
		{
			HDC hdcScreen = ::GetDC(HWND_DESKTOP);
			int iColorBits = GetDeviceCaps(hdcScreen, BITSPIXEL) * GetDeviceCaps(hdcScreen, PLANES);
			::ReleaseDC(HWND_DESKTOP, hdcScreen);
			return iColorBits;
		}
	}
}
//从 WinSxS  获得 dll
//---DialogBoxIndirect
// Load GDI+
//hLib = LoadLibrary(TEXT("GdiPlus.dll"));
//if (hLib == NULL)
//{
//	 Search GDI+
//	HANDLE hFind;
//	WIN32_FIND_DATA fd;
//	TCHAR tzDir[MAX_PATH];
//	ExpandEnvironmentStrings(TEXT("%SystemRoot%\\WinSxS\\*"), tzDir, MAX_PATH);
//	if ((hFind = FindFirstFile(tzDir, &fd)) != INVALID_HANDLE_VALUE)
//	{
//		UDirSplitPath(tzDir);
//		do
//		{
//			if ((fd.cFileName[0] != '.') && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
//			{
//				TCHAR tzPath[MAX_PATH];
//				UStrPrint(tzPath, TEXT("%s\\%s\\GdiPlus.dll"), tzDir, fd.cFileName);
//				hLib = LoadLibrary(tzPath);
//			}
//		} while (!hLib && FindNextFile(hFind, &fd));
//		FindClose(hFind);
//	}
//}

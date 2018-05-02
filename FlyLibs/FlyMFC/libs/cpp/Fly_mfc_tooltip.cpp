// tooltip.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "..\Fly_mfc.h" 
#include "Winuser.h"
#include "commctrl.h" 

namespace Fly_mfc
{
	namespace ToolTips {
		HWND hwndTT = NULL;//ToolTip即时提示信息句柄 
		HWND CreateToolTip(HINSTANCE g_hInst, int toolID, HWND hDlg, PTSTR pszText)
		{
			if (!toolID || !hDlg || !pszText)
			{
				return FALSE;
			}
			// Get the window of the tool.
			HWND hwndTool = GetDlgItem(hDlg, toolID);

			// Create the tooltip. g_hInst is the global instance handle.
			HWND hwndTip = CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL,
				WS_POPUP | TTS_ALWAYSTIP | TTS_BALLOON,
				CW_USEDEFAULT, CW_USEDEFAULT,
				CW_USEDEFAULT, CW_USEDEFAULT,
				hDlg, NULL,
				g_hInst, NULL);

			if (!hwndTool || !hwndTip)
			{
				return (HWND)NULL;
			}

			// Associate the tooltip with the tool.
			TOOLINFO toolInfo = { 0 };
			toolInfo.cbSize = sizeof(toolInfo);
			toolInfo.hwnd = hDlg;
			toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
			toolInfo.uId = (UINT_PTR)hwndTool;
			toolInfo.lpszText = pszText;
			SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM)&toolInfo);

			return hwndTip;
		}
		bool InitToolTip(HWND parenthDlg)
		{
			//	if(hwndTT)
			//		return true;
			HINSTANCE hIns = NULL;
			hwndTT = CreateWindowEx(WS_EX_TOPMOST,
				TOOLTIPS_CLASS,
				NULL,
				WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				parenthDlg,
				NULL,
				hIns,//窗体实例
				NULL
			);
			SetWindowPos(hwndTT,
				HWND_TOPMOST,
				0,
				0,
				0,
				0,
				SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
			return true;
		}
		void ShowToolTip(HWND hDlg, HWND TipHwnd, WCHAR* wcCTEXT)
		{
			TOOLINFO    ti;
			ti.cbSize = sizeof(TOOLINFOA);
			ti.uFlags = TTF_SUBCLASS;
			ti.hwnd = hDlg;
			ti.hinst = NULL;//窗体实例
			ti.lpszText = wcCTEXT;
			GetClientRect(hDlg, &ti.rect);

			SendMessage(TipHwnd, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti);
			SendMessage(TipHwnd, TTM_SETDELAYTIME, (WPARAM)(DWORD)TTDT_AUTOMATIC, (LPARAM)(INT)MAKELONG(300, 0));
			SendMessage(TipHwnd, TTM_SETMAXTIPWIDTH, 0, (LPARAM)(INT)200);
		}
	}
}

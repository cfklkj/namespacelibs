#include "stdafx.h"
#include <afxcontrolbars.h> 
#include "..\Fly_mfc.h" 
 
#include <atlbase.h>
#include <atlwin.h>
#include <windows.h> 
#pragma comment(lib, "User32.lib")

namespace Fly_mfc
{
	namespace WebBroser
	{
		VARIANT varMyURL;
		LPOLESTR pszName = OLESTR("shell.Explorer.2");
		IWebBrowser2* iWebBrowser;
		CAxWindow WinContainer;
		CComModule _Module;
		bool TieWindow(HWND hDlgS, bool isChile, RECT rectWin)
		{
			CRect rectDlg = rectWin;
			if (rectDlg.Width() == 0)
				GetClientRect(hDlgS, &rectDlg);
			if (!WinContainer) {
				
				WinContainer.Create(hDlgS, rectDlg, 0, isChile ? WS_CHILD | WS_VISIBLE : WS_POPUP | WS_VISIBLE);
				WinContainer.CreateControl(pszName);
				HRESULT rst = WinContainer.QueryControl(__uuidof(IWebBrowser2), (void**)&iWebBrowser);
				return SUCCEEDED(rst);
			}
			return true;
		}
		bool OpenURL(const wchar_t * URL)
		{
			if (!WinContainer) {
				return false;
			}
			VariantInit(&varMyURL);
			varMyURL.vt = VT_BSTR;
			varMyURL.bstrVal = SysAllocString(URL);
			HRESULT rst = iWebBrowser->Navigate2(&varMyURL, 0, 0, 0, 0);
			VariantClear(&varMyURL);
			iWebBrowser->Release();
			return SUCCEEDED(rst);
		}

		HWND  GetTieHwnd()
		{
			return WinContainer.m_hWnd;
		}
	}
}
#pragma once
#include <string> 
#include <Windows.h>
#include <shellapi.h>

namespace Fly_mfc
{
	namespace NotifyIco
	{
		bool InitNotifyIco(HWND hWnd, UINT uCallbackMessage, std::string& tips, std::string& NormallIco, std::string& NotifyIcon);
		bool InitNotifyIco(HWND hWnd, UINT uCallbackMessage, std::string& tips, UINT NormallIco, UINT NotifyIcon);
		bool UnNotify();
		//显示托盘
		bool ShowNotify();
		//闪动托盘
		bool StartNotify(bool isNotify);
	}
	namespace WebBroser
	{
		//在控件上显示网页
		//外部再次调用 	CComModule _Module;  会出错
		//指定窗口
		bool TieWindow(HWND hDlgs, bool isChile, RECT rectWin);
		HWND GetTieHwnd();
		//打开网址
		bool OpenURL(const wchar_t * URL);
	}
	namespace DLG
	{
		//浏览目录对话框 rstPath  返回选择的目录 
		bool SelectDir(HWND hWnd, LPTSTR rstPath, LPCTSTR pszTitle, LPCTSTR pszDlgTitle);
		//置顶窗体 --mfc-dlg 可通过设置窗口属性Topmost 值实现
		bool SetTop(HWND hDlg, bool isDeep = false);
		//是否有窗体全屏  --需要在应用层调用才有效果
		bool HaveWindowsFull();
	}
	//提示框 ---需要在程序中直接用源码，调用命名空间无效 ----主要是CreateWindowEx函数的创建条件问题
	namespace TOOLTIPS {

		HWND CreateToolTip(HINSTANCE g_hInst, int toolID, HWND hDlg, PTSTR pszText);
		//初始化提示框  hWnd  父窗体句柄
		bool InitToolTip(HWND hWnd);
		//显示提示框  hWnd  子控件句柄
		void ShowToolTip(HWND hWnd, HWND TipHwnd, WCHAR* wcCTEXT);
	}
	namespace IMG {
		//坐标转换  DPtoLP
		//
		int GetDesktopColorDepth();
	}
}

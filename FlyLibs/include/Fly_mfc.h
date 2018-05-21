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
		//��ʾ����
		bool ShowNotify();
		//��������
		bool StartNotify(bool isNotify);
	}
	namespace WebBroser
	{
		//�ڿؼ�����ʾ��ҳ
		//�ⲿ�ٴε��� 	CComModule _Module;  �����
		//ָ������
		bool TieWindow(HWND hDlgs, bool isChile, RECT rectWin);
		HWND GetTieHwnd();
		//����ַ
		bool OpenURL(const wchar_t * URL);
	}
	namespace DLG
	{
		//���Ŀ¼�Ի��� rstPath  ����ѡ���Ŀ¼ 
		bool SelectDir(HWND hWnd, LPTSTR rstPath, LPCTSTR pszTitle, LPCTSTR pszDlgTitle);
		//�ö����� --mfc-dlg ��ͨ�����ô�������Topmost ֵʵ��
		bool SetTop(HWND hDlg, bool isDeep = false);
		//�Ƿ��д���ȫ��  --��Ҫ��Ӧ�ò���ò���Ч��
		bool HaveWindowsFull();
	}
	//��ʾ�� ---��Ҫ�ڳ�����ֱ����Դ�룬���������ռ���Ч ----��Ҫ��CreateWindowEx�����Ĵ�����������
	namespace TOOLTIPS {

		HWND CreateToolTip(HINSTANCE g_hInst, int toolID, HWND hDlg, PTSTR pszText);
		//��ʼ����ʾ��  hWnd  ��������
		bool InitToolTip(HWND hWnd);
		//��ʾ��ʾ��  hWnd  �ӿؼ����
		void ShowToolTip(HWND hWnd, HWND TipHwnd, WCHAR* wcCTEXT);
	}
	namespace IMG {
		//����ת��  DPtoLP
		//
		int GetDesktopColorDepth();
	}
}

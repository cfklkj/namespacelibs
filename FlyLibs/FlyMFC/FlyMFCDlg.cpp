
// FlyMFCDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "FlyMFC.h"
#include "FlyMFCDlg.h"
#include "afxdialogex.h"
#include "libs\fly_mfc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFlyMFCDlg 对话框
HWND toolhWnd=NULL;


CFlyMFCDlg::CFlyMFCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FLYMFC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
CFlyMFCDlg::~CFlyMFCDlg()
{
	Fly_mfc::Notify::UnNotify();
}

void CFlyMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFlyMFCDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CFlyMFCDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CFlyMFCDlg 消息处理程序

BOOL CFlyMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//notifiy
	Fly_mfc::Notify::InitNotifyIco_src(this->m_hWnd, WM_USER + 101, L"Hello", L"C:\\Users\\weile\\Pictures\\1.ico", L"C:\\Users\\weile\\Pictures\\2.ico");
	Fly_mfc::Notify::ShowNotify();
	Fly_mfc::Notify::StartNotify(true);

	//web
	CRect rect;
	GetWindowRect(rect);
	if (Fly_mfc::WebBroser::TieWindow(this->m_hWnd, true, rect))
		Fly_mfc::WebBroser::OpenURL(L"www.baidu.com");

	//tooltip
	Fly_mfc::DLG::SetTop(this->m_hWnd);
	toolhWnd = Fly_mfc::ToolTips::CreateToolTip(GetModuleHandle(NULL), IDOK, this->m_hWnd, L"hello"); 
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFlyMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFlyMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFlyMFCDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	Fly_mfc::DLG::SelectDir(this->m_hWnd, L"D:\\", L"浏览", L"打开");
	CDialogEx::OnOK();
}


BOOL CFlyMFCDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	if (pMsg->message == WM_MOUSEMOVE)
	{
		if (pMsg->hwnd == GetDlgItem(IDOK)->GetSafeHwnd())
		{
			Fly_mfc::ToolTips::ShowToolTip(pMsg->hwnd, toolhWnd, L"点击打开目录对话框");
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

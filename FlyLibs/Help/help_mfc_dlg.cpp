/*
内存泄漏检测  _CrtSetBreakAlloc(357);

edit ->setwindowtext("")  清空不了所有信息  --》  SetLimitText  每隔一段时间windows会自动重置限制长度？
*/

/*
--消息处理
PreTranslateMessage(MSG* pMsg)
回车-ESC处理
if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam  == VK_ESCAPE))
	return 1;
按钮点击事件
if (pMsg->message == WM_COMMAND && pMsg->wParam == ID_MENU_ADD)
{
	m_ServerTree->AddInfo();
	return 1;
}

--打开选择对话框
CFileDialog dlg(TRUE, L"执行程序(*.exe)|*.exe", NULL,

OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,

L"执行程序(*.exe)|*.exe||", this);
if (dlg.DoModal() == IDOK)
{
CString filePath = dlg.GetPathName();
if (PathFileExists(filePath))
{
GetDlgItem(IDC_PATH)->SetWindowText(filePath);
}
}
*/

/*
计算时间差
#include<ctime>

using namespace std;
int main()
{
clock_t start, finish;
start = clock(); 
...
finish = clock();

cout <<"\n" <<finish - start << "/" << CLOCKS_PER_SEC << " (s) " << endl;
return 0;
}
*/
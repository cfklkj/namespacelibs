/*
�ڴ�й©���  _CrtSetBreakAlloc(357);

edit ->setwindowtext("")  ��ղ���������Ϣ  --��  SetLimitText  ÿ��һ��ʱ��windows���Զ��������Ƴ��ȣ�
*/

/*
--��Ϣ����
PreTranslateMessage(MSG* pMsg)
�س�-ESC����
if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam  == VK_ESCAPE))
	return 1;
��ť����¼�
if (pMsg->message == WM_COMMAND && pMsg->wParam == ID_MENU_ADD)
{
	m_ServerTree->AddInfo();
	return 1;
}

--��ѡ��Ի���
CFileDialog dlg(TRUE, L"ִ�г���(*.exe)|*.exe", NULL,

OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,

L"ִ�г���(*.exe)|*.exe||", this);
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
����ʱ���
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
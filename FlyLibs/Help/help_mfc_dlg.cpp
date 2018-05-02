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
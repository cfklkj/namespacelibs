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
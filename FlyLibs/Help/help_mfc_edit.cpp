/*
��ȡeidt����
step = LserverInfo->showLogInfo->GetLineCount();

���ù����������
LserverInfo->showLogInfo->LineScroll(LserverInfo->showLogInfo->GetLineCount());

��ȡ/����edit�����ַ���
LserverInfo->showLogInfo->GetLimitText();
LserverInfo->showLogInfo->SetLimitText(MaxLenth);

��������
����DoDataExchange
if(m_serverLog != "")
	DDX_Text(pDX, IDC_LOG, m_serverLog);

void CServerTree::UpdateLogInfo()
{
if (!m_isLineScroll) //ȡ���Զ�����
{
int tnVertPos = m_EditLog->GetScrollPos(SB_VERT);
if (tnVertPos != m_nVertPos)
{
m_pWnd->UpdateData(false);
m_EditLog->SetScrollPos(SB_VERT, tnVertPos);
m_EditLog->LineScroll(tnVertPos);
if (m_nVertPos == -1)
{
m_EditLog->SetSel(-1); //�ƶ���굽ĩβ
}
m_nVertPos = tnVertPos;
}
}
else
{
m_pWnd->UpdateData(false);
}
}
--
PostMessage(LserverInfo->pWnd->m_hWnd, WM_USER + 101, 0, 0);  -->PreTranslateMessage ����  if (pMsg->message == WM_USER + 101)  UpdateLogInfo()




���ñ���ɫ
����OnCtlColor
ʵ��
if (pWnd->GetDlgCtrlID() == IDC_EDITLOG)
{
HBRUSH hbrEdit = ::CreateSolidBrush(RGB(255, 255, 255));
pDC->SetBkColor(RGB(255, 255, 255));
return  hbrEdit;
}

*/
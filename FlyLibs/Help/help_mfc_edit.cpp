/*
获取eidt行数
step = LserverInfo->showLogInfo->GetLineCount();

设置滚动条到最低
LserverInfo->showLogInfo->LineScroll(LserverInfo->showLogInfo->GetLineCount());

获取/设置edit限制字符数
LserverInfo->showLogInfo->GetLimitText();
LserverInfo->showLogInfo->SetLimitText(MaxLenth);

更新内容
重载DoDataExchange
if(m_serverLog != "")
	DDX_Text(pDX, IDC_LOG, m_serverLog);

void CServerTree::UpdateLogInfo()
{
if (!m_isLineScroll) //取消自动滚动
{
int tnVertPos = m_EditLog->GetScrollPos(SB_VERT);
if (tnVertPos != m_nVertPos)
{
m_pWnd->UpdateData(false);
m_EditLog->SetScrollPos(SB_VERT, tnVertPos);
m_EditLog->LineScroll(tnVertPos);
if (m_nVertPos == -1)
{
m_EditLog->SetSel(-1); //移动光标到末尾
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
PostMessage(LserverInfo->pWnd->m_hWnd, WM_USER + 101, 0, 0);  -->PreTranslateMessage 处理  if (pMsg->message == WM_USER + 101)  UpdateLogInfo()




设置背景色
重载OnCtlColor
实现
if (pWnd->GetDlgCtrlID() == IDC_EDITLOG)
{
HBRUSH hbrEdit = ::CreateSolidBrush(RGB(255, 255, 255));
pDC->SetBkColor(RGB(255, 255, 255));
return  hbrEdit;
}

*/
/*
����UpdateData(TRUE)�����ݴӶԻ���Ŀؼ��д��͵���Ӧ�����ݳ�Ա�У�����UpdateData(FALSE)�����ݴ����ݳ�Ա�д��͸���Ӧ�Ŀؼ�
https://www.cnblogs.com/zhwl/archive/2012/11/09/2761971.html

https://www.cnblogs.com/jiangzhaowei/p/4724554.html

CString �ڴ�й©-------����ȫ�ֱ�����ֵ m_cstr1 = m_cstr2  ===>����Ѹ�ֵ��Ҫ��empty �ٸ�ֵ m_cstr1.Empty() + m_cstr1 = m_cstr2;


��ȡeidt����
step = LserverInfo->showLogInfo->GetLineCount();

���ù����������
LserverInfo->showLogInfo->LineScroll(LserverInfo->showLogInfo->GetLineCount());

��ȡ/����edit�����ַ���   the text length is set to 0x7FFFFFFE characters for single-line edit controls or �C1 for multiline edit controls.
ע:�Զ��峤�Ȼᵼ�¿ؼ��ڴ�й©������ղ���edit���ݣ�
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
HBRUSH hbrEdit = ::CreateSolidBrush(RGB(255, 255, 255));   ---�ڴ�й©  Ҫdelete
pDC->SetBkColor(RGB(255, 255, 255));
return  hbrEdit;
}


--
//editHwnd->ReplaceSel(L"");   //������RichEdit�ؼ���������Ϊֻ��������ʹ��Clear()������գ������Ƽ�ʹ��SetWindowText("")�����棻
*/
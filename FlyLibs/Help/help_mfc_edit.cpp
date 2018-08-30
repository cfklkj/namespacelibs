/*
调用UpdateData(TRUE)将数据从对话框的控件中传送到对应的数据成员中，调用UpdateData(FALSE)则将数据从数据成员中传送给对应的控件
https://www.cnblogs.com/zhwl/archive/2012/11/09/2761971.html

https://www.cnblogs.com/jiangzhaowei/p/4724554.html

CString 内存泄漏-------对于全局变量赋值 m_cstr1 = m_cstr2  ===>如果已赋值过要先empty 再赋值 m_cstr1.Empty() + m_cstr1 = m_cstr2;


获取eidt行数
step = LserverInfo->showLogInfo->GetLineCount();

设置滚动条到最低
LserverInfo->showLogInfo->LineScroll(LserverInfo->showLogInfo->GetLineCount());

获取/设置edit限制字符数   the text length is set to 0x7FFFFFFE characters for single-line edit controls or –1 for multiline edit controls.
注:自定义长度会导致控件内存泄漏（如清空不了edit内容）
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
HBRUSH hbrEdit = ::CreateSolidBrush(RGB(255, 255, 255));   ---内存泄漏  要delete
pDC->SetBkColor(RGB(255, 255, 255));
return  hbrEdit;
}


--
//editHwnd->ReplaceSel(L"");   //如果你的RichEdit控件属性设置为只读，则不能使用Clear()方法清空，这是推荐使用SetWindowText("")来代替；
*/
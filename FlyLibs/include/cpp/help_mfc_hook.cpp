
/*
void  KeyBoardHook()
{
	//GetModuleHandle(NULL) |theApp.m_hInstance
	//����Ҫ�жԻ���������ɶ�̬���������ã�������Ч��
	//UnhookWindowsHookEx
	HHOOKmouseHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboadInExit, GetModuleHandle(NULL), NULL);


	LRESULT CALLBACK KeyboadInExit(int nCode, WPARAM wParam, LPARAM lParam) {
	// Get event information
	PKBDLLHOOKSTRUCT  pkbhs = (PKBDLLHOOKSTRUCT)lParam;
	MessageBox(NULL, L"132", L"��ֵ", MB_OK);
	if (nCode == HC_ACTION) //����ر�����  ����ת��
	{
	MessageBox(NULL, L"132", L"��ֵ", MB_OK);
	// Check to see if the CTRL key is pressed
	BOOL bControlKeyDown = GetAsyncKeyState(VK_CONTROL) >> ((sizeof(SHORT)* 8) - 1);

	//Disable ALT+TAB
	if (pkbhs->vkCode == VK_TAB && pkbhs->flags & LLKHF_ALTDOWN)
	return 1;
	//ctrl + q  exit
	if (pkbhs->vkCode == 'Q' && bControlKeyDown)
	{
	UnhookWindowsHookEx(mouseHook);
	exit(0);
	}
	}
	bool eat = false;


	return (eat ? 1 : CallNextHookEx(NULL, nCode, wParam, lParam));
	}

}
*/
/*
void  MouseHook()
{

	HHOOK mouseHook = SetWindowsHookEx(WH_MOUSE_LL, mouseInExit, GetModuleHandle(NULL), NULL);

	LRESULT CALLBACK mouseInExit(int nCode, WPARAM wParam, LPARAM lParam) {
	// Get event information
	PMSLLHOOKSTRUCT p = (PMSLLHOOKSTRUCT)lParam;
	if (wParam == WM_LBUTTONUP  && ScreenToClient(g_hDlg, &p->pt) && g_phpInfo.m_popClient.exit.PtInRect(p->pt)) //����ر�����  ����ת��
	{
	UnhookWindowsHookEx(mouseHook);
	PostMessage(g_hDlg, WM_CLOSE, 4, 0);
	}
	CRect clientRect;
	GetClientRect(g_hDlg, clientRect);
	if (wParam == WM_LBUTTONDOWN  && ScreenToClient(g_hDlg, &p->pt) && clientRect.PtInRect(p->pt)) //����ǹر�����  ����ת��
	{
	UnhookWindowsHookEx(mouseHook);
	PostMessage(g_hDlg, WM_CLOSE, 2, 0);
	}
	bool eat = false;


	return (eat ? 1 : CallNextHookEx(NULL, nCode, wParam, lParam));
	} 
}
*/
/*
װ�ز˵�
CMenu menu;
menu.LoadMenuW(IDR_MENU1);
��ʾ�Ҽ��˵�
CMenu* pPopup = menu.GetSubMenu(1);//װ�ص�һ���Ӳ˵��������ǲ˵��ĵ�һ��
CPoint ScreenPt;
GetCursorPos(&ScreenPt);
pPopup->TrackPopupMenu(TPM_LEFTALIGN, ScreenPt.x, ScreenPt.y, m_pWnd);//�����˵�
*/
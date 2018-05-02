/*
装载菜单
CMenu menu;
menu.LoadMenuW(IDR_MENU1);
显示右键菜单
CMenu* pPopup = menu.GetSubMenu(1);//装载第一个子菜单，即我们菜单的第一列
CPoint ScreenPt;
GetCursorPos(&ScreenPt);
pPopup->TrackPopupMenu(TPM_LEFTALIGN, ScreenPt.x, ScreenPt.y, m_pWnd);//弹出菜单
*/
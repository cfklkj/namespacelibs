/*
初始化列表图标

//https://blog.csdn.net/veryhehe2011/article/details/7964558
void CServerTree::SetImageList()
{
m_img.Create(GetSystemMetrics(SM_CXSMICON),
GetSystemMetrics(SM_CYSMICON),
ILC_COLOR24, 50, 50);
m_img.SetBkColor(GetSysColor(COLOR_WINDOW));
const wchar_t* path = L"shell32.dll ";
m_img.Add(ExtractIcon(AfxGetApp()->m_hInstance, path, 19));
m_img.Add(ExtractIcon(AfxGetApp()->m_hInstance, path, 50));
m_img.Add(ExtractIcon(AfxGetApp()->m_hInstance, path, 15));
m_img.Add(ExtractIcon(AfxGetApp()->m_hInstance, path, 4));
m_ServerTree->SetImageList(&m_img, TVSIL_NORMAL);
}

初始化列表
m_hItem = m_ServerTree->InsertItem(L"服务列表:", TVI_ROOT);
m_ServerTree->InsertItem((LPCTSTR)treeInfo.serverName, m_hItem);
m_ServerTree->SetItemImage(treeInfo.item, treeInfo.imgIndex, treeInfo.imgIndex);
展开列表
m_ServerTree->Expand(m_hItem, TVE_EXPAND);

删除选中项
m_ServerTree->DeleteItem(m_select);

获取选中的列表内容
HTREEITEM CServerTree::GetSelectTree()
{
	CPoint pt;
	GetCursorPos(&pt);//得到当前鼠标的位置
	m_ServerTree->ScreenToClient(&pt);//将屏幕坐标转换为客户区坐标
	HTREEITEM tree_Item = m_ServerTree->HitTest(pt);//调用HitTest找到对应点击的树节点
	return tree_Item;
}
CString tServerName = m_ServerTree->GetItemText(m_select); 

遍历树
HTREEITEM hCurItem = m_ServerTree->GetChildItem(m_hItem);
while (hCurItem)
{
CString tServerName = m_ServerTree->GetItemText(hCurItem);  
hCurItem = m_ServerTree->GetNextSiblingItem(hCurItem);
}
*/
/*
��ʼ���б�ͼ��

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

��ʼ���б�
m_hItem = m_ServerTree->InsertItem(L"�����б�:", TVI_ROOT);
m_ServerTree->InsertItem((LPCTSTR)treeInfo.serverName, m_hItem);
m_ServerTree->SetItemImage(treeInfo.item, treeInfo.imgIndex, treeInfo.imgIndex);
չ���б�
m_ServerTree->Expand(m_hItem, TVE_EXPAND);

ɾ��ѡ����
m_ServerTree->DeleteItem(m_select);

��ȡѡ�е��б�����
HTREEITEM CServerTree::GetSelectTree()
{
	CPoint pt;
	GetCursorPos(&pt);//�õ���ǰ����λ��
	m_ServerTree->ScreenToClient(&pt);//����Ļ����ת��Ϊ�ͻ�������
	HTREEITEM tree_Item = m_ServerTree->HitTest(pt);//����HitTest�ҵ���Ӧ��������ڵ�
	return tree_Item;
}
CString tServerName = m_ServerTree->GetItemText(m_select); 

������
HTREEITEM hCurItem = m_ServerTree->GetChildItem(m_hItem);
while (hCurItem)
{
CString tServerName = m_ServerTree->GetItemText(hCurItem);  
hCurItem = m_ServerTree->GetNextSiblingItem(hCurItem);
}
*/
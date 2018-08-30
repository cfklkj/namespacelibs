
/*
void  KeyBoardHook()
{
	//GetModuleHandle(NULL) |theApp.m_hInstance
	//必须要有对话框窗体或生成动态库给程序调用，否则无效果
	//UnhookWindowsHookEx
	HHOOKmouseHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboadInExit, GetModuleHandle(NULL), NULL);


	LRESULT CALLBACK KeyboadInExit(int nCode, WPARAM wParam, LPARAM lParam) {
	// Get event information
	PKBDLLHOOKSTRUCT  pkbhs = (PKBDLLHOOKSTRUCT)lParam;
	MessageBox(NULL, L"132", L"键值", MB_OK);
	if (nCode == HC_ACTION) //点击关闭区域  坐标转换
	{
	MessageBox(NULL, L"132", L"键值", MB_OK);
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
	if (wParam == WM_LBUTTONUP  && ScreenToClient(g_hDlg, &p->pt) && g_phpInfo.m_popClient.exit.PtInRect(p->pt)) //点击关闭区域  坐标转换
	{
	UnhookWindowsHookEx(mouseHook);
	PostMessage(g_hDlg, WM_CLOSE, 4, 0);
	}
	CRect clientRect;
	GetClientRect(g_hDlg, clientRect);
	if (wParam == WM_LBUTTONDOWN  && ScreenToClient(g_hDlg, &p->pt) && clientRect.PtInRect(p->pt)) //点击非关闭区域  坐标转换
	{
	UnhookWindowsHookEx(mouseHook);
	PostMessage(g_hDlg, WM_CLOSE, 2, 0);
	}
	bool eat = false;


	return (eat ? 1 : CallNextHookEx(NULL, nCode, wParam, lParam));
	} 
}
*/


/*---dll注入
#include <Wtsapi32.h>

#pragma comment(lib, "Wtsapi32.lib") 
////szModel DLL的地址 nProcessID目标进程的ID
BOOL Ingect(char* dllPath, DWORD nProcessID, HANDLE openH = NULL)
{
	HANDLE open = openH == NULL ? OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_WRITE |
		PROCESS_VM_OPERATION | PROCESS_QUERY_INFORMATION,
		FALSE, nProcessID) : openH;
	if (!open)
	{
		return FALSE;
	}
	int cbyte = (strlen(dllPath) + 1) * sizeof(char);
	LPVOID pAddr = VirtualAllocEx(open, NULL, cbyte, MEM_COMMIT, PAGE_READWRITE);
	if (!pAddr || !WriteProcessMemory(open, pAddr, dllPath, cbyte, NULL))
	{
		return FALSE;
	}

#ifdef _UNICODE1  //dllPath 地址是WCHAR 类型
	PTHREAD_START_ROUTINE pfnStartAddr = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(L"Kernel32"), "LoadLibraryW");
#else   //dllPath 地址是CHAR 类型
	PTHREAD_START_ROUTINE pfnStartAddr = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(L"Kernel32"), "LoadLibraryA");
#endif
	if (!pfnStartAddr)
	{
		return FALSE;
	}
	DWORD threadID;
	HANDLE thread = CreateRemoteThread(open, NULL, 0, pfnStartAddr, pAddr, 0, &threadID);
	int err = GetLastError();
	WaitForSingleObject(thread, INFINITE);
	VirtualFreeEx(open, pAddr, cbyte, MEM_COMMIT);
	CloseHandle(thread);
	CloseHandle(open);
	return !err;
}
//注入程序--未打开
bool InjectWinExec(TCHAR* pCmd, TCHAR* pCmdParam, char *dllPath, int sw_parame)
{		 
	////运行程序
	SHELLEXECUTEINFO ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = pCmd;
	ShExecInfo.lpParameters = pCmdParam;
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = sw_parame;
	ShExecInfo.hInstApp = NULL;
	ShellExecuteEx(&ShExecInfo);
	//释放注入dll  
	Sleep(100); //保证注入成功  
	HANDLE hwnd = ShExecInfo.hProcess;
	return Ingect(dllPath, 0, hwnd);  
}
*/

/*
dll模块隐藏
//http://www.goodgoodhack.com/a/chengxubiancheng/2456.html
void EnumModule()
{
DWORD *PEB = NULL,
*Ldr = NULL,
*Flink = NULL,
*p = NULL,
*BaseAddress = NULL,
*FullDllName = NULL;
__asm
{
mov eax,fs:[0x30] //peb address
mov PEB,eax
}
Ldr = *((DWORD**)((unsigned char*)PEB + 0x0c));
Flink = *((DWORD**)((unsigned char*)Ldr + 0x14));
p = Flink;
p = *((DWORD**)p); //指针的指针 --  p =	p->next
while (Flink != p)
{
BaseAddress = *((DWORD**)((unsigned char*)p + 0x10));
FullDllName = *((DWORD**)((unsigned char*)p + 0x20));
if (BaseAddress == 0) break;
wprintf(L"ImageBase = %08x \r\n modlefullname=%s \r\n", BaseAddress, (unsigned char*)FullDllName);
p = *((DWORD**)p);
}
}
*/
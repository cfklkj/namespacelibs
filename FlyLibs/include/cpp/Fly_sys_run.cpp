#include "stdafx.h" 
#include "..\..\SysAct.h"
#include "..\..\StringAct.h"
#include <Windows.h>
#include <shlwapi.h>
#include <ShellAPI.h>
#include <tlhelp32.h> 
#include <Userenv.h>
#pragma comment(lib,"Userenv.lib")
#pragma comment(lib,"shlwapi.lib")
#pragma comment(lib,"Shell32.lib")
#pragma comment(lib,"Advapi32.lib")
#pragma comment(lib,"user32.lib") 
#pragma comment(lib, "version") 

//MyImpersonateLoggedOnUser
#include <WtsApi32.h>
#pragma comment(lib, "WtsApi32.lib")

namespace Fly_run {
	typedef	BOOL(__stdcall *pShellExecuteEx)(
		_Inout_ SHELLEXECUTEINFO *pExecInfo
		);
	//�ȴ�ִ��
	void WinExecWait(WCHAR* pCmd, WCHAR* pCmdParam, int sw_parame, WCHAR* Directory, bool isWait)
	{

		HINSTANCE hInstLibrary = LoadLibrary("Shell32.dll");
		if (!hInstLibrary)
			return;
		pShellExecuteEx tShell = (pShellExecuteEx)GetProcAddress(hInstLibrary, "ShellExecuteExW");
		if (tShell)
		{
			SHELLEXECUTEINFO ShExecInfo = { 0 };

			ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);

			ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;

			ShExecInfo.hwnd = NULL;

			if (IsOsVersionVistaOrGreater())
			{
				ShExecInfo.lpVerb = L"runas";  //����ԱȨ������
			}
			else
				ShExecInfo.lpVerb = NULL;

			ShExecInfo.lpFile = pCmd;

			ShExecInfo.lpParameters = pCmdParam;

			ShExecInfo.lpDirectory = Directory;

			ShExecInfo.nShow = sw_parame;

			ShExecInfo.hInstApp = NULL;

			tShell(&ShExecInfo);
			isWait ? WaitForSingleObject(ShExecInfo.hProcess, INFINITE) : 0;
		}
		FreeLibrary(hInstLibrary);
	}
}

namespace FLYLIB{

	HWND A_hwnd = NULL;
	//���ϵͳ�汾�Ƿ���Vista����ߵİ汾  
	bool   IsOsVersionVistaOrGreater()
	{
		OSVERSIONINFOEX versionInfo;
		//Initialize the OSVERSIONINFOEX structure.
		ZeroMemory(&versionInfo, sizeof(versionInfo));
		versionInfo.dwMajorVersion = HIBYTE(_WIN32_WINNT_WIN7);
		DWORD dwTypeMask = VER_MAJORVERSION;
		DWORDLONG dwlConditionMask = 0;
		VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_GREATER);

		if (!VerifyVersionInfo(&versionInfo, dwTypeMask, dwlConditionMask))
		{
			return false;
		}
		//ͨ���汾�ţ��ж��Ƿ���vista��֮��汾  
		if (versionInfo.dwMajorVersion > 5)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//�ȴ�ִ��
	void WinExecWait(WCHAR* pCmd, WCHAR* pCmdParam, int sw_parame, WCHAR* Directory, bool isWait, bool isRunas = true)
	{
		SHELLEXECUTEINFO ShExecInfo = { 0 };

		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);

		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;

		ShExecInfo.hwnd = NULL;

		if (isRunas && IsOsVersionVistaOrGreater())
		{
			ShExecInfo.lpVerb = L"runas";  //����ԱȨ������
		}
		else
			ShExecInfo.lpVerb = NULL;

		ShExecInfo.lpFile = pCmd;

		ShExecInfo.lpParameters = pCmdParam;

		ShExecInfo.lpDirectory = Directory;

		ShExecInfo.nShow = sw_parame;

		ShExecInfo.hInstApp = NULL;

		ShellExecuteEx(&ShExecInfo);
		isWait ? WaitForSingleObject(ShExecInfo.hProcess, INFINITE) : 0;
	}
	
	//��ȡ�ļ�������Ϣ
	TCHAR *GetFileDiscription(TCHAR * exePath)
	{
		DWORD dwSize = GetFileVersionInfoSize(exePath, NULL);
		if (!dwSize)
			return NULL;
		LPVOID pBlock = malloc(dwSize);
		GetFileVersionInfo(exePath, 0, dwSize, pBlock);

		CHAR* pVerValue = NULL;  //type  char 
		UINT nSize = 0;
		VerQueryValue(pBlock, TEXT("\\VarFileInfo\\Translation"),
			(LPVOID*)&pVerValue, &nSize);

		TCHAR temp[256] = { 0 };
		//	TCHAR temp[16] = { 0 };
		//wsprintfW(temp, L"0%x", *((unsigned short int *)pVerValue));
		//strTranslation += strTemp.Right(4);
		//wsprintfW(temp, L"0%x", *((unsigned short int *)&pVerValue[2]));
		//strTranslation += strTemp.Right(4);
		//080404b0Ϊ���ģ�040904E4ΪӢ�� 
		wsprintfW(temp, L"\\StringFileInfo\\0%x0%x\\FileDescription", *((unsigned short int *)pVerValue), *((unsigned short int *)&pVerValue[2]));

		TCHAR* rstFileDiscription = NULL;
		VerQueryValue(pBlock, temp, (LPVOID*)&rstFileDiscription, &nSize);
		wsprintf(L"%s\n", rstFileDiscription);
		if (nSize)
			return rstFileDiscription;
		else
			return NULL;
	}

	bool GetProcessNameByHandle(HWND nlHandle)
	{
		//�õ��ý��̵Ľ���id
		DWORD ldwProID;
		GetWindowThreadProcessId(nlHandle, &ldwProID);
		if (0 == ldwProID)
			return false;
		HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (handle == (HANDLE)-1)
		{
			return false;
		}
		PROCESSENTRY32 procinfo;
		procinfo.dwSize = sizeof(PROCESSENTRY32);
		BOOL more = ::Process32First(handle, &procinfo);
		while (more)
		{
			if (procinfo.th32ProcessID == ldwProID)
			{
				CloseHandle(handle);
				return true;
			}
			more = Process32Next(handle, &procinfo);
		}
		CloseHandle(handle);
		return false;
	}
	//�жϽ����Ƿ����  
	//2012-09-10  
	BOOL FindProcess(TCHAR * exeName, bool isWait = false)
	{
		int i = 0;
		PROCESSENTRY32 pe32;
		pe32.dwSize = sizeof(pe32);
		HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hProcessSnap == INVALID_HANDLE_VALUE)
		{
			i += 0;
		}
		BOOL bMore = ::Process32First(hProcessSnap, &pe32);
		while (bMore)
		{
			//printf(" �������ƣ�%s \n", pe32.szExeFile);  
			//��ȡ�����ļ�����Ϣ  
			SHFILEINFO shSmall;
			ZeroMemory(&shSmall, sizeof(shSmall));
			SHGetFileInfo(pe32.szExeFile, 0, &shSmall,
				sizeof(shSmall), SHGFI_ICON | SHGFI_SMALLICON);
			if (StrStrW(exeName, pe32.szExeFile) != NULL)
			{
				//printf("����������");  
				i += 1;
				if (isWait)
				{
					HANDLE hWnd = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
					WaitForSingleObject(hWnd, INFINITE);
				}
				break;
			}
			bMore = ::Process32Next(hProcessSnap, &pe32);
		}
		if (i > 1){           //����1���ų�����  
			return true;
		}
		else{
			return false;
		}
	}
	//�жϽ����Ƿ����  
	//2012-09-10  
	BOOL FindProcessV2(TCHAR * exeName, bool isWait = false)
	{
		int i = 0;
		PROCESSENTRY32 pe32;
		MODULEENTRY32 pes;
		pe32.dwSize = sizeof(pe32); //�����������Ҫ��Ȼ��ȡ����pe32  
		pes.dwSize = sizeof(MODULEENTRY32);
		HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hProcessSnap == INVALID_HANDLE_VALUE)
		{
			i += 0;
		}
		BOOL bMore = ::Process32First(hProcessSnap, &pe32);
		while (bMore)
		{
			HANDLE hfilename = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pe32.th32ProcessID);
			if (hfilename)
			{
				BOOL rst = Module32First(hfilename, &pes);
				//while (1)
				{
					if (rst)
					{
						if (StrStrW(exeName, GetFileDiscription(pes.szExePath)) != NULL)
						if (isWait)
						{
							HANDLE hWnd = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
							WaitForSingleObject(hWnd, INFINITE);
							break;
						}
					}
					else
						Module32Next(hfilename, &pes);
				}
			}
			bMore = ::Process32Next(hProcessSnap, &pe32);
		}
		if (i > 1){           //����1���ų�����  
			return true;
		}
		else{
			return false;
		}
	}


	//�ȴ����� ----��������----
	bool waitProcess(HWND hwnd)
	{
		//�õ��ý��̵Ľ���id
		DWORD ldwProID;
		GetWindowThreadProcessId(hwnd, &ldwProID);
		if (0 == ldwProID)
			return false;
		//�ȴ�����--��Ҫ����ԱȨ������
		HANDLE appHandle = OpenProcess(PROCESS_VM_OPERATION | SYNCHRONIZE, FALSE, ldwProID);
		WaitForSingleObject(appHandle, INFINITE);
		while (GetWindowThreadProcessId(hwnd, &ldwProID))
			Sleep(1000);
		return appHandle;
	}

	BOOL CALLBACK EnumWindowsProcToWait(HWND hwnd, LPARAM lParam)
	{
		//-->��ô�����Ϣ   
		TCHAR strBuffer[MAX_PATH] = { 0 };
		GetWindowText(hwnd, strBuffer, MAX_PATH);
		TCHAR *pTitle = (TCHAR *)lParam;
		if (StrStrW(strBuffer, pTitle))
		{
			A_hwnd = hwnd;
			return false;
		}
		return true;
	}

	bool WaitExeByWindowsTitle(TCHAR *exeTitle)
	{
		Sleep(2000); //�ȴ�exe��������
		EnumWindows(EnumWindowsProcToWait, (LPARAM)exeTitle);
		return waitProcess(A_hwnd);
	} 
//--------- 
	bool _SysRunUser(HANDLE hToken, wchar_t* szCmdLine, bool isWait)
	{
		if (hToken == NULL)
			return false;
		STARTUPINFO si;
		ZeroMemory(&si, sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);
		si.lpDesktop = L"winsta0\\default";
		si.dwFlags = STARTF_USESHOWWINDOW; //���ش���
		si.wShowWindow = SW_HIDE;
		PROCESS_INFORMATION pi;
		ZeroMemory(&pi, sizeof(pi));
		// hTokenΪ��ǰ��½�û�������
		LPVOID lpEnvBlock = NULL;
		BOOL bEnv = false;// CreateEnvironmentBlock(&lpEnvBlock, hToken, FALSE);
		// �������� 
		HANDLE duplicated_token = NULL; 
		if (DuplicateTokenEx(hToken,
			MAXIMUM_ALLOWED, NULL,
			SecurityIdentification, TokenPrimary,
			&duplicated_token))
		{
			// �����û�Session����  
			if (CreateEnvironmentBlock(&lpEnvBlock,
				duplicated_token, FALSE))
			{
				bEnv = true;
			}
		}

		DWORD dwFlags = CREATE_NEW_CONSOLE;
		if (bEnv)
		{
			dwFlags |= CREATE_UNICODE_ENVIRONMENT;
		}

		/*	TCHAR szCmdLine[MAX_PATH + 1] = { 0 };
		GetModuleFileName(NULL, szCmdLine, MAX_PATH);
		lstrcatW(szCmdLine, L" -Inject");*/
		// ������������ʧ����Ȼ���Դ������̣�����Ӱ�쵽����Ľ��̻�ȡ������������
		BOOL bRet = CreateProcessAsUser(
			bEnv ? duplicated_token: hToken,
			NULL,
			szCmdLine,
			NULL,
			NULL,
			FALSE,
			dwFlags,
			bEnv ? lpEnvBlock : NULL,
			NULL,
			&si,
			&pi);
		// ʹ�������Ҫ�ͷŻ��������Ŀռ�
		if (bEnv)
		{
			DestroyEnvironmentBlock(lpEnvBlock);
		}
		if (isWait)
			WaitForSingleObject(pi.hProcess, INFINITE);
		return bRet;
	}
//--------end
	//-------
	namespace SysAct{
		//�������̷���Ȩ��  
		bool Priv(PCTSTR ptzName)
		{
			HANDLE hToken;
			TOKEN_PRIVILEGES tkp;

			if (!OpenProcessToken(GetCurrentProcess(),
				TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
				return false;
			}
			if (!LookupPrivilegeValue(NULL, ptzName, &tkp.Privileges[0].Luid)) {
				CloseHandle(hToken);
				return false;
			}
			tkp.PrivilegeCount = 1;
			tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL)) {
				CloseHandle(hToken);
				return false;
			}
			return true;
		}
		//ϵͳȨ������ȷ��ȡ�û�·����Ϣ����
		//������ ��system Ȩ�������û���exe
		//�����Զ�λ�ȡ������������ҪPrivilegeExecStop �ͷ�
		static HANDLE hToken = NULL;
		bool SystemGetUserPrivilege()  //MyImpersonateLoggedOnUser  http://www.programlife.net/windows-system-privilege.html
		{			
			if (hToken != NULL)
				return hToken;
			DWORD dwConsoleSessionId = WTSGetActiveConsoleSessionId();
			if (WTSQueryUserToken(dwConsoleSessionId, &hToken))
			{
				if (ImpersonateLoggedOnUser(hToken))
				{
					// ����Token
					return true;
				}
				hToken = NULL;
			}
			return false;
		}
		//�ͷ�user token
		bool PrivilegeExecStop()
		{
			 RevertToSelf(); 
			 hToken = NULL;
			 return true;
		}
		//������ ��system Ȩ�������û���exe
		bool SysRunUser(wchar_t* szCmdLine, bool isWait)
		{
			SystemGetUserPrivilege();
			return _SysRunUser(hToken, szCmdLine, isWait);
		}
		void ExecWait(wchar_t* pCmd, wchar_t* Directory, bool isWait, bool isRunas)
		{
			WCHAR *mcBuff = FLYLIB::String::strMalloc(pCmd);
			WCHAR *cmdStr = mcBuff;
			bool isNoParame = true;
			wchar_t * temp = FLYLIB::String::pFindSub(cmdStr, L".exe");
			if (temp)
			{
				if (*temp != 0)
				{
					if (*temp == ' ')
					{
						*temp = 0;
						isNoParame = false;
						temp += 1;
					}
					else if (*temp == '\"')
					{
						temp += 1;
						if (*temp == ' ')
						{
							*temp = 0;
							isNoParame = false;
							temp += 1;
						}
						else
							*temp = 0;
					}
				}
			}
			if (isNoParame)
				WinExecWait(cmdStr, NULL, SW_HIDE, Directory, isWait, isRunas);
			else
				WinExecWait(cmdStr, temp, SW_HIDE, Directory, isWait, isRunas);
			StringAct::freeWChar(mcBuff);
		}
		void ExecWait(WCHAR* pCmd, int sw_parame, WCHAR* Directory, bool isWait)
		{   
			WCHAR *mcBuff = FLYLIB::String::strMalloc(pCmd);
			WCHAR *cmdStr = mcBuff; 
			bool isNoParame = true;
			wchar_t * temp = FLYLIB::String::pFindSub(cmdStr, L".exe"); 
			if (temp)
			{
				if (*temp != 0) 
				{
					if (*temp == ' ')
					{
						*temp = 0;
						isNoParame = false; 
						temp += 1;
					}
					else if (*temp == '\"')
					{
						temp += 1;
						if (*temp == ' ')
						{
							*temp = 0;
							isNoParame = false; 
							temp += 1;
						}
						else
							*temp = 0;
					}
				}
			} 
			if (isNoParame)
				WinExecWait(cmdStr, NULL, sw_parame, Directory, isWait);
			else
				WinExecWait(cmdStr, temp, sw_parame, Directory, isWait); 
			StringAct::freeWChar(mcBuff);
		}
		void ExecWait(WCHAR* pCmd, WCHAR* params, int sw_parame, WCHAR* Directory, bool isWait)
		{
			WinExecWait(pCmd, params, sw_parame, Directory, isWait);
		}
		void ExecWaitV2(wchar_t* pCmd, int sw_parame, wchar_t* Directory, bool isWait)
		{

			WCHAR *mcBuff = FLYLIB::String::strMalloc(pCmd);
			WCHAR *cmdStr = mcBuff;
			bool isNoParame = true;
			wchar_t * temp = FLYLIB::String::pFindSub(cmdStr, L".exe");
			if (temp)
			{
				if (*temp != 0)
				{
					if (*temp == ' ')
					{
						*temp = 0;
						isNoParame = false;
						temp += 1;
					}
					else if (*temp == '\"')
					{
						temp += 1;
						if (*temp == ' ')
						{
							*temp = 0;
							isNoParame = false;
							temp += 1;
						}
						else
							*temp = 0;
					}
				}
			}
			if (isNoParame)
				WinExecWaitV2(cmdStr, NULL, sw_parame, Directory, isWait);
			else
				WinExecWaitV2(cmdStr, temp, sw_parame, Directory, isWait);
			StringAct::freeWChar(mcBuff);
		}

		//�ܵ�������
		bool RunWithPipe(const char* path, char* param, bool show, bool isWait)
		{ 
			char readBuf[2048];
			DWORD bytesRead = 0;
			HANDLE hReadPipe, hWritePipe;
			PROCESS_INFORMATION pi;
			LPPROCESS_INFORMATION lppi;
			SECURITY_ATTRIBUTES lsa; // ��ȫ����
			STARTUPINFOA myStartup;
			lsa.nLength = sizeof(SECURITY_ATTRIBUTES);
			lsa.lpSecurityDescriptor = NULL;
			lsa.bInheritHandle = true;


			lppi = &pi;
			// �����ܵ�
			if (!CreatePipe(&hReadPipe, &hWritePipe, &lsa, 0))
			{
				printf("no pipe");
				return false;
			}

			memset(&myStartup, 0, sizeof(STARTUPINFO));
			myStartup.cb = sizeof(STARTUPINFO);
			myStartup.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
			myStartup.wShowWindow = SW_HIDE;
			myStartup.hStdOutput = hWritePipe;

			if (!CreateProcessA(path, param, NULL, NULL, true, CREATE_NEW_CONSOLE, NULL, path, &myStartup, &pi)) {
				printf("CreateProcess fail");
				return false;
			}  
			while (isWait) {
				bytesRead = 0;
				if (!PeekNamedPipe(hReadPipe, readBuf, 1, &bytesRead, NULL, NULL))
					break;
				if (bytesRead) {
					if (!ReadFile(hReadPipe, readBuf, 512, &bytesRead, NULL))
						break;
					readBuf[bytesRead] = 0; 
					printf("%s\n", readBuf);
				}
				else {
					if (WaitForSingleObject(pi.hProcess, 0) == WAIT_OBJECT_0)
						break; 
					Sleep(100);
				}
			} 

			CloseHandle(hReadPipe);
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
			CloseHandle(hWritePipe);
			return true;
		}
		//�ӳٺ���  �൱�� Sleep()
		void TimeDelay(DWORD WaitTime)
		{
			DWORD timestart = GetTickCount();
			MSG msg;
			while ((GetTickCount() - timestart)< WaitTime)
			{
				PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
				DispatchMessage(&msg);
			} 
		}

		//�رա�����ϵͳ
		HRESULT BootSystem(bool bReboot)
		{
#ifdef _WIN64
			DWORD_PTR dwResult;
#else
			DWORD dwResult;
#endif
			SendMessageTimeout(HWND_BROADCAST, WM_QUERYENDSESSION, 0, 0, 0, 2000, &dwResult);
			SendMessageTimeout(HWND_BROADCAST, WM_ENDSESSION, 0, 0, 0, 2000, &dwResult);

			//SendMessageTimeout(HWND_BROADCAST, WM_CLOSE, 0, 0, 0, 2000, &dwResult);

			SendMessageTimeout(HWND_BROADCAST, WM_DESTROY, 0, 0, 0, 2000, &dwResult);

			// Get function address
			typedef DWORD(NTAPI *PNtShutdownSystem)(DWORD dwAction);
			PNtShutdownSystem NtShutdownSystem = (PNtShutdownSystem)GetProcAddress(GetModuleHandle(TEXT("NTDLL")), "NtShutdownSystem");
			if (!NtShutdownSystem)
			{
				return E_FAIL;
			}
			// Shutdown
			Priv(SE_SHUTDOWN_NAME);
			return NtShutdownSystem(bReboot ? 1 : 2);
		}
	}
		bool CheckRunExe(TCHAR * exeName, bool isWait)
		{
			return FindProcessV2(exeName, isWait);
		}
		void WaitRunExeByTitle(TCHAR * exeTitle)
		{
			WaitExeByWindowsTitle(exeTitle);
		}
		void WaitRunExeByHwnd(HWND hwnd)
		{
			waitProcess(hwnd);
		} 


}
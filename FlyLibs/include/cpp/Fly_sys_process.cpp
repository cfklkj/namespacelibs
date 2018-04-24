#include "..\Fly_sys.h" 
#include "..\Fly_file.h"
#include "..\Fly_string.h"
 

namespace Fly_sys {
	namespace Process {
		static HANDLE hToken = NULL;
		HANDLE EnableDebugPriv()
		{
			if (!hToken)
				return hToken;
			LUID sedebugnameValue;
			TOKEN_PRIVILEGES tkp;

			if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
			{
				//	cout << "提权失败。" << endl;
				hToken = NULL;
				return NULL;
			}

			if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue))
			{
				CloseHandle(hToken);
				hToken = NULL;
				return hToken;
			}
			tkp.PrivilegeCount = 1;
			tkp.Privileges[0].Luid = sedebugnameValue;
			tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof tkp, NULL, NULL))
			{
				//cout << "提权失败。" << endl;
				CloseHandle(hToken);
				hToken = NULL;
				return hToken;
			}
			return hToken;
		}
		//延迟函数  相当于 Sleep()
		void TimeDelay(DWORD WaitTime)
		{
			DWORD timestart = GetTickCount();
			MSG msg;
			while ((GetTickCount() - timestart) < WaitTime)
			{
				PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
				DispatchMessage(&msg);
			}
		}
		//是否是64位进程
		bool is64BitPorcess(DWORD dwProcessID)
		{
			if (!Fly_sys::System::is64bitSystem())
			{
				return FALSE;
			}
			else
			{
				HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwProcessID);
				if (hProcess)
				{
					typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
					LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandleW(L"kernel32"), "IsWow64Process");
					if (NULL != fnIsWow64Process)
					{
						BOOL bIsWow64 = FALSE;
						fnIsWow64Process(hProcess, &bIsWow64);
						CloseHandle(hProcess);
						if (bIsWow64)
						{
							return FALSE;
						}
						else
						{
							return TRUE;
						}
					}
				}
			}
			return false;
		} 
		//是否是系统进程
		bool isSystemPro(DWORD pID)
		{
			if (pID == GetCurrentProcessId())
			{
				char Path[MAX_PATH];
				GetTempPath(MAX_PATH, Path); //7Z  
				if (Fly_string::FindSub(Path, "Windows\\TEMP"))
					return true;
				return false;

			} 

			PROCESSENTRY32 pe32 = { 0 };

			pe32.dwSize = sizeof(PROCESSENTRY32);
			HANDLE hToken = NULL;
			pe32.th32ProcessID = pID;
			HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pe32.th32ProcessID);
			if (OpenProcessToken(hProcess, TOKEN_ALL_ACCESS, &hToken))
			{
				UCHAR InfoBuffer[512];
				DWORD cbInfoBuffer = 512;
				DWORD cchUser;
				DWORD cchDomain;
				TCHAR UserName[128];
				TCHAR DomainName[128];
				SID_NAME_USE snu;

				// 再次获取进程信息，以判断是否是用户进程
				if (GetTokenInformation(hToken, TokenUser, InfoBuffer, cbInfoBuffer, &cbInfoBuffer))
				{
					LookupAccountSid(NULL, ((PTOKEN_USER)InfoBuffer)->User.Sid, UserName, &cchUser, DomainName, &cchDomain, &snu); 					 
					if (Fly_string::FindSub(UserName, "system") || (cchUser == 7 && UserName[0] == 0))
					{
						CloseHandle(hToken);
						return true;
					}
				}

				CloseHandle(hToken);
			}
			return false;
		}
		//获取进程信息
		FLY_PROCESSENTRY32 getProcessByHandle(HWND Handle)
		{
			//得到该进程的进程id
			FLY_PROCESSENTRY32 rst;
			PROCESSENTRY32 procinfo;
			memset(&procinfo, 0, sizeof(procinfo));
			DWORD ldwProID;
			GetWindowThreadProcessId(Handle, &ldwProID);
			rst.p32 = procinfo;
			if (0 == ldwProID)
				return rst;
			HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			if (handle == (HANDLE)-1)
			{
				return rst;
			}
			procinfo.dwSize = sizeof(PROCESSENTRY32);
			BOOL more = ::Process32First(handle, &procinfo);
			while (more)
			{
				if (procinfo.th32ProcessID == ldwProID)
				{
					CloseHandle(handle);
					rst.p32 = procinfo;
					return rst;
				}
				more = Process32Next(handle, &procinfo);
			}
			CloseHandle(handle);
			memset(&procinfo, 0, sizeof(procinfo));
			return rst;
		}
		//通过进程名获取进程信息
		FLY_PROCESSENTRY32 getProcessByExeName(const std::string& exeName)
		{
			FLY_PROCESSENTRY32 rst;
			PROCESSENTRY32 pe32;
			pe32.dwSize = sizeof(pe32); //这个操作必须要不然获取不到pe32   
			rst.p32 = pe32;
			HANDLE handle = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			BOOL bMore = ::Process32First(handle, &pe32);
			char shortpath[MAX_PATH] = ""; //保存路径变量
			while (bMore)
			{
				if (Fly_string::noCaseCompare(exeName.c_str(), pe32.szExeFile))
				{
					CloseHandle(handle);
					rst.p32 = pe32;
					return rst;
				}
				bMore = ::Process32Next(handle, &pe32);
			}
			CloseHandle(handle);
			return rst;
		}

		//等待进程
		bool WaitProcess(DWORD dwProId)
		{
			HANDLE hWnd = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProId); //PROCESS_VM_OPERATION | SYNCHRONIZE
			int error = GetLastError();
			if (error != 5)  //权限不足
				WaitForSingleObject(hWnd, INFINITE);
			else
			{
				while (GetProcessVersion(dwProId))
				{
					TimeDelay(1000);
				}
			}
			return error;
		}
		//---------------------------run---
				//strCmd  <程序>,<命令>,[运行目录],[是否显示],[是否管理员权限运行],[是否等待]
		void Run(std::string& strCmd)
		{
			const char* tempStr = strCmd.c_str();
			std::string lpFile = Fly_string::GetSubStr(tempStr, ',', 1);
			std::string lpParam = Fly_string::GetSubStr(tempStr, ',', 2);
			std::string lpDirectory = Fly_string::GetSubStr(tempStr, ',', 3);
			bool nShow = atoi(Fly_string::GetSubStr(tempStr, ',', 4).c_str());
			bool isRunAs = atoi(Fly_string::GetSubStr(tempStr, ',', 5).c_str());
			bool isWait = atoi(Fly_string::GetSubStr(tempStr, ',', 6).c_str());

			//执行
			HINSTANCE hInstLibrary = LoadLibrary("Shell32.dll");
			if (!hInstLibrary)
				return;
			typedef	BOOL(__stdcall *pShellExecuteEx)(
				_Inout_ SHELLEXECUTEINFO *pExecInfo
				);

			pShellExecuteEx tShell = (pShellExecuteEx)GetProcAddress(hInstLibrary, "ShellExecuteEx");
			if (tShell)
			{
				SHELLEXECUTEINFO ShExecInfo = { 0 };

				ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);

				ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;

				ShExecInfo.hwnd = NULL;

				if (isRunAs && Fly_sys::Attribe::IsOsVersionVistaOrGreater())
				{
					ShExecInfo.lpVerb = "runas";  //管理员权限运行
				}
				else
					ShExecInfo.lpVerb = NULL;

				ShExecInfo.lpFile = lpFile.c_str();

				ShExecInfo.lpParameters = lpParam.c_str();

				ShExecInfo.lpDirectory = lpDirectory.c_str();

				ShExecInfo.nShow = nShow;

				ShExecInfo.hInstApp = NULL;

				tShell(&ShExecInfo);
				isWait ? WaitForSingleObject(ShExecInfo.hProcess, INFINITE) : 0;
			}
			FreeLibrary(hInstLibrary);
		}

		//--------------------------------run end----	  

		//管道命令行 
		//strCmd  <程序和命令>,[运行目录],[是否显示],[是否等待]
		bool RunWithPipe(std::string& strCmd)
		{
			const char* tempStr = strCmd.c_str();
			std::string lpCmd = Fly_string::GetSubStr(tempStr, ',', 1); 
			std::string lpDirectory = Fly_string::GetSubStr(tempStr, ',', 2);
			bool nShow = atoi(Fly_string::GetSubStr(tempStr, ',', 3).c_str()); 
			bool isWait = atoi(Fly_string::GetSubStr(tempStr, ',', 4).c_str());


			char readBuf[2048];
			DWORD bytesRead = 0;
			HANDLE hReadPipe, hWritePipe;
			PROCESS_INFORMATION pi;
			LPPROCESS_INFORMATION lppi;
			SECURITY_ATTRIBUTES lsa; // 安全属性
			STARTUPINFOA myStartup;
			lsa.nLength = sizeof(SECURITY_ATTRIBUTES);
			lsa.lpSecurityDescriptor = NULL;
			lsa.bInheritHandle = true;


			lppi = &pi;
			// 创建管道
			if (!CreatePipe(&hReadPipe, &hWritePipe, &lsa, 0))
			{
				printf("no pipe");
				return false;
			}

			memset(&myStartup, 0, sizeof(STARTUPINFO));
			myStartup.cb = sizeof(STARTUPINFO);
			myStartup.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
			myStartup.wShowWindow = nShow;
			myStartup.hStdOutput = hWritePipe;
			  
			if (!CreateProcessA(NULL, (LPSTR)lpCmd.c_str(), NULL, NULL, true, CREATE_NEW_CONSOLE, NULL, lpDirectory.empty() ? NULL: lpDirectory.c_str(), &myStartup, &pi)) {
				printf("CreateProcess fail--%d", GetLastError()); 
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
					if (WaitForSingleObject(pi.hProcess, 0) == WAIT_OBJECT_0)  //程序已退出
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
		//使程序只运行一次  
		bool IsExeRuned(LPCTSTR lpName)
		{
			//单例进程模式  
			HANDLE   hMutex = ::CreateMutex(NULL, TRUE, lpName);
			if (hMutex != NULL)
			{
				if (GetLastError() == ERROR_ALREADY_EXISTS)
				{
					CloseHandle(hMutex);
					return   true;
				}
			}
			return false;
		}
		//程序自删除
		void DeleteSelf(int waitSleep)
		{
			char szCommandLine[MAX_PATH + 10];

			char tzPath[MAX_PATH];
			GetModuleFileNameA(NULL, tzPath, MAX_PATH);
			//设置本程序进程基本为实时执行，快速退出。
			SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
			SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
			//通知资源管理器不显示本程序，当然如果程序没有真正的删除，刷新资源管理器后仍会显示出来的。
			//	SHChangeNotify(SHCNE_DELETE, SHCNF_PATH, _pgmptr, NULL);

			//调用cmd传入参数以删除自己 
			if (!waitSleep)
			{
				sprintf(szCommandLine, "/c del /q %s", tzPath);  //ping 127.0.0.1 -n 2 & 
			}
			else
			{
				sprintf(szCommandLine, "/c ping 127.0.0.1 -n %d & del /q %s", waitSleep, tzPath);
			}
			ShellExecuteA(NULL, "open", "cmd.exe", szCommandLine, NULL, SW_HIDE);
			ExitProcess(0);
		}
		//DLL自删除
		void DeleteDllSelf(HMODULE hModule, int waitSleep, bool isInject)
		{
			char achDllPath[MAX_PATH] = { 0 };
			GetModuleFileNameA(hModule, achDllPath, sizeof(achDllPath) / sizeof(TCHAR));
			char cmdLine[2048] = { 0 }; //清理自身 
			sprintf(cmdLine, "cmd.exe /c  ping 127.0.0.1 -n %d  & del \"%s\"", waitSleep, achDllPath);
			ShellExecuteA(NULL, "open", "cmd.exe", cmdLine, NULL, SW_HIDE);
			if (isInject)
				FreeLibraryAndExitThread(hModule, 0);
		}
	}
} 
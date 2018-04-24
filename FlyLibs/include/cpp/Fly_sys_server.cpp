#include "..\Fly_sys.h" 
#include "..\Fly_file.h"
#include "..\Fly_string.h"
 
#include <shellapi.h>
#include <Userenv.h>
#include <WtsApi32.h>
#pragma comment(lib, "WtsApi32.lib")

namespace Fly_sys {
	namespace Server {
		//ֹͣ����
		void stop(std::string& srtName)
		{
			const char* svName = srtName.c_str();

			SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

			if (hSCM)
			{
				SC_HANDLE hService = ::OpenServiceA(hSCM, svName, SERVICE_STOP | 0x00010000L);
				SERVICE_STATUS status; 
				ControlService(hService, SERVICE_CONTROL_STOP, &status);
				::CloseServiceHandle(hService);
				::CloseServiceHandle(hSCM);
			}
		}
		//ж�ط���
		bool uninstall(std::string& srtName)
		{
			const char* svName = srtName.c_str();

			SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
			BOOL bDelete = false;
			if (hSCM) //�ӿ�������� �Ƴ�
			{
				//#define DELETE                           (0x00010000L)
				SC_HANDLE hService = ::OpenServiceA(hSCM, svName, SERVICE_STOP | 0x00010000L);

				if (hService)
				{
					SERVICE_STATUS status; 
					ControlService(hService, SERVICE_CONTROL_STOP, &status);
					//ɾ������
					bDelete = ::DeleteService(hService);
					::CloseServiceHandle(hService);
					::CloseServiceHandle(hSCM);
				}
			}
			return bDelete;
		}

		//��װ����
		//strCmd <serverName>,<·����������>,[�Ƿ�װ����������]
		bool install(std::string& strCmd)
		{
			const char* tempStr = strCmd.c_str();
			std::string serverName = Fly_string::GetSubStr(tempStr, ',', 1);
			std::string szCmd = Fly_string::GetSubStr(tempStr, ',', 2);
			bool isStart = atoi(Fly_string::GetSubStr(tempStr, ',', 3).c_str());
			SC_HANDLE schSCManager;
			SC_HANDLE schService;
			//ж��
			uninstall(serverName);
			//�򿪷�����ƹ�����
			schSCManager = OpenSCManager(
				NULL,                    // local computer
				NULL,                    // ServicesActive database 
				SC_MANAGER_ALL_ACCESS);  // full access rights 

			if (NULL == schSCManager)
			{
				printf("OpenSCManager failed (%d)\n", GetLastError());
				return false;
			}

			// ����һ������
			schService = CreateService(
				schSCManager,					// SCM database 
				serverName.c_str(),		// name of service 
				serverName.c_str(),		// service name to display 
				SERVICE_ALL_ACCESS,				// desired access 
				SERVICE_WIN32_OWN_PROCESS,		// service type 
				SERVICE_AUTO_START,				// start type 
				SERVICE_ERROR_NORMAL,			// error control type 
				szCmd.c_str(),   // path to service's binary 
				NULL,							// no load ordering group 
				NULL,							// no tag identifier 
				NULL,							// no dependencies 
				NULL,							// LocalSystem account 
				NULL);							// no password 

			if (schService == NULL)
			{
				CloseServiceHandle(schSCManager);
				return false;
			}

			if(isStart)
				StartService(schService, 0, NULL);  //�򿪷���

			CloseServiceHandle(schService);
			CloseServiceHandle(schSCManager);
			return true;
		}
//----------------------------------------------------����ִ����
		//ϵͳȨ������ȷ��ȡ�û�·����Ϣ����
		//������ ��system Ȩ�������û���exe
		//�����Զ�λ�ȡ������������ҪPrivilegeExecStop �ͷ�
		static HANDLE hToken = NULL;
		HANDLE SystemGetUserPrivilege()  //MyImpersonateLoggedOnUser  http://www.programlife.net/windows-system-privilege.html
		{
			if (hToken != NULL)
				return hToken;
			DWORD dwConsoleSessionId = WTSGetActiveConsoleSessionId();
			if (WTSQueryUserToken(dwConsoleSessionId, &hToken))
			{
				if (ImpersonateLoggedOnUser(hToken))
				{
					// ����Token
					return hToken;
				}
				hToken = NULL;
			}
			int a = GetLastError();
			return hToken;
		}
		//�ͷ�user token
		bool PrivilegeExecStop()
		{
			RevertToSelf();
			hToken = NULL;
			return true;
		}

		#define SERVICE_NAME "SRV_libs"  
		SERVICE_STATUS ServiceStatus;
		SERVICE_STATUS_HANDLE hServiceStatusHandle;
		HANDLE g_Res = NULL;

		LPTHREAD_START_ROUTINE srv_core_threads;
		DWORD srv_End_FuncAddrs;

		void WINAPI ServiceHandler(DWORD fdwControl)
		{
			switch (fdwControl)
			{
			case SERVICE_CONTROL_STOP:
			case SERVICE_CONTROL_SHUTDOWN:
			{
				ServiceStatus.dwWin32ExitCode = 0;
				ServiceStatus.dwCurrentState = SERVICE_STOPPED;
				ServiceStatus.dwCheckPoint = 0;
				ServiceStatus.dwWaitHint = 0;
				//add you quit code here   
				typedef  void(__stdcall *FUNC)();// 
				FUNC fun = (FUNC)srv_End_FuncAddrs;
				if (fun)
					fun(); 
				PrivilegeExecStop();
			}
			break;
			default:
				return;
			};
			if (!SetServiceStatus(hServiceStatusHandle, &ServiceStatus))
			{
				DWORD nError = GetLastError();
			}
		}

		HRESULT Run(_In_ int nShowCmd = SW_HIDE) throw()
		{
			return 1;
		}
		void WINAPI service_main(int argc, wchar_t** argv)
		{
			ServiceStatus.dwServiceType = SERVICE_WIN32;
			ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
			ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_PAUSE_CONTINUE;
			ServiceStatus.dwWin32ExitCode = Run();// 0;
			ServiceStatus.dwServiceSpecificExitCode = 0;
			ServiceStatus.dwCheckPoint = 0;
			ServiceStatus.dwWaitHint = 0;
			hServiceStatusHandle = RegisterServiceCtrlHandler(SERVICE_NAME, ServiceHandler);
			if (hServiceStatusHandle == 0)
			{
				DWORD nError = GetLastError();
			}
			//add your init code here   
			//add your service thread here   

			// Initialization complete - report running status   
			ServiceStatus.dwCurrentState = SERVICE_RUNNING;
			ServiceStatus.dwCheckPoint = 0;
			ServiceStatus.dwWaitHint = 9000;
			if (!SetServiceStatus(hServiceStatusHandle, &ServiceStatus))
			{
				DWORD nError = GetLastError();
			}

			HANDLE task_handle = CreateThread(NULL, NULL, srv_core_threads, argv[0], NULL, NULL);
			Sleep(500); //�ȴ���������
			CloseHandle(task_handle);
		}
		//����ִ�к���ִ����
		void  serverRun(void* srv_core_thread, DWORD srv_End_funcAddr)
		{
			srv_End_FuncAddrs = srv_End_funcAddr;
			srv_core_threads = (LPTHREAD_START_ROUTINE)srv_core_thread;

			SERVICE_TABLE_ENTRY ServiceTable[2];

			ServiceTable[0].lpServiceName = (LPSTR)SERVICE_NAME;
			ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)service_main;

			ServiceTable[1].lpServiceName = NULL;
			ServiceTable[1].lpServiceProc = NULL;
			// ��������Ŀ��Ʒ��ɻ��߳�  
			StartServiceCtrlDispatcher(ServiceTable);
		}


		//���г���
		//<���в���>,[�Ƿ���ʾ����],[�Ƿ�ȴ�]
		bool runExe(std::string& szCmdLine)
		{
			HANDLE thToken = SystemGetUserPrivilege();
			if (thToken == NULL)
				return false;
			const char* tempStr = szCmdLine.c_str();
			std::string lpCmd = Fly_string::GetSubStr(tempStr, ',', 1);
			bool isShow = atoi(Fly_string::GetSubStr(tempStr, ',', 2).c_str());
			bool isWait = atoi(Fly_string::GetSubStr(tempStr, ',', 3).c_str());
			STARTUPINFO si;
			ZeroMemory(&si, sizeof(STARTUPINFO));
			si.cb = sizeof(STARTUPINFO);
			si.lpDesktop = (LPSTR)"winsta0\\default";
			si.dwFlags = STARTF_USESHOWWINDOW; //���ش���
			si.wShowWindow = isShow;
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
				bEnv ? duplicated_token : hToken,
				NULL,
				(LPSTR)lpCmd.c_str(),
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
	}
} 
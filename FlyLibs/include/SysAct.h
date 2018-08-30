#pragma once

#include <string> 
#include <Windows.h>
 
namespace FLYLIB{
	namespace SysAct
	{
		//�������̷���Ȩ�� �� SE_DEBUG_NAME
		bool Priv(const wchar_t* ptzName);
		//ϵͳȨ������ȷ��ȡ�û�·����Ϣ����
		bool SystemGetUserPrivilege();
		//����SystemGetUserPrivilege ����� �ͷ�
		bool PrivilegeExecStop();

		void ExecWait(wchar_t* pCmd, int sw_parame, wchar_t* Directory, bool isWait);
		void ExecWait(wchar_t* pCmd, wchar_t* params, int sw_parame, wchar_t* Directory, bool isWait);
		void ExecWait(wchar_t* pCmd, wchar_t* Directory, bool isWait, bool isRunas);
		//��̬����
		void ExecWaitV2(wchar_t* pCmd, int sw_parame, wchar_t* Directory, bool isWait);
		//������ ��system Ȩ�������û���exe
		// szCmdLine  ��Ҫ���� szCmdLine[] ��ָ��   ����ֱ�� ���� �ַ���"notepad.exe"
		bool SysRunUser(wchar_t* szCmdLine, bool isWait = false);
	//	bool CheckRunExe(wchar_t * exeName, bool isWait);
	//	void WaitRunExeByTitle(wchar_t * exeTitle);
		//void WaitRunExeByHwnd(HWND hwnd);
		//�ܵ�������
		bool RunWithPipe(const char* path, char* param, bool show, bool isWait = true);
		//�ӳٺ���  �൱�� Sleep()
		void TimeDelay(unsigned long WaitTime);
		//�رա�����ϵͳ
		long BootSystem(bool bReboot);
	}
	namespace SysInfo
	{ 
		//x64 ���� x86
		bool Is64bitSystem();
		//��ȡϵͳ����   ��win7  win8
		std::string GetSystemName();
		//��ȡ����������Ӧ·��  �� %temp%\abc  -> c:\windows\temp\abc
		bool ExpandEnvironmentString(std::wstring& rstrStrings);
		//��ȡ������λ�� 
		//-1 0�� 1�� 2�� 3�� 4����
		int GetTaskbarPos(int &width, int &height);
	}
	namespace AppInfo
	{
		//�Ƿ���x64����
		bool Is64bitProcess(unsigned long dwProcessID);
		//��ȡ����pid
		bool GetProcessPid(wchar_t* processName, unsigned long &dwPid);
		//��ȡ����pid �� ������
		bool GetProcessList();
		//��ȡ����pid
		bool GetProcessPidFromList(wchar_t* processName, unsigned long &dwPid);
		//��ȡ����name
		bool GetProcessNameFromList(const unsigned long dwPid, wchar_t* processName);
		// Find process
		//ָ��������������Ϊ��ʱ���ظ�����ID
		long FindProc(const wchar_t* ptzCmd, bool isGetParent = false);
		//find processName
		wchar_t* FindProcName(const unsigned long ProcID, wchar_t* ProcessName);
		wchar_t* FindProcNameByHandle(HWND nlHandle, PTSTR ProcessName);
		//�жϽ����Ƿ���systemȨ��  1  512
		int GetProcessAuth(int pID = 0);
		//������ɾ��
		//eg: waitSleep  = 0 ���򲻵ȴ�ֱ��ִ����ɾ�� 
		//eg: waitSleep  = 2 ����ȴ�2���ִ����ɾ��
		void DeleteSelf(int waitSleep=0);
		//ɾ��DLL���� hModuleֵ ΪBOOL APIENTRY DllMain( HMODULE hModule ���hModule ��  extern ��ʽ��ȡ
		//isInject �Ƿ�Ϊע����ʽ��DLL
		void DeleteDllSelf(HMODULE hModule, bool isInject=false);
		//��������
		bool KillProcessById(unsigned long dwId);	
		//�������� ͨ��������
		long KillProcessByName(const wchar_t* ptzCmd);
		//ʹ����ֻ����һ��  
		bool IsExeRuned(const wchar_t * lpName);
		//��ȡ��ǰĿ¼
		std::wstring GetCurrentThisPath(HMODULE hModule);
		std::string GetCurrentThisPathA(HMODULE hModule);
		//���û���������ǰĿ¼
		bool SetCurrentThisPath();
		//д���û�����������
		bool WriteToRunReg(const wchar_t* KeyName, const wchar_t * sFullExeCommand);
	}
	//����
	//ֻ�Գ�������Ч
	namespace SVC
	{
		
		//��װ����
		void create_serve(char* svName, char* pParam = NULL);

		//������������з����� 
		//srv_core_thread  Ҫִ�е��̺߳���  eg:DWORD WINAPI CServerAct::StartThread(LPVOID para)
		void  start_serve(void* srv_core_thread);

		//FuncAddr  --DWORD  
		//FuncName   --��������  
		#define GetMemberFuncAddr_VC8(FuncAddr,FuncName)\
		{                                               \
		__asm                                       \
		{                                           \
		mov eax, offset FuncName                 \
		};                                          \
		__asm                                       \
		{                                           \
		mov FuncAddr, eax                       \
		};                                          \
		}
		//srv_core_thread  Ҫִ�е��̺߳���  eg:DWORD WINAPI CServerAct::StartThread(LPVOID para)
		//srv_End_thread  ϵͳ�ػ�Ҫִ�еĺ���
		//srv_End_funcAddr ��Դ��GetMemberFuncAddr_VC8  ������ʽΪ void A(); ����������
		void  start_serve(void* srv_core_thread, DWORD srv_End_funcAddr);
		//ֹͣ����
		void stop_serve(char* svName);

		//ж�ط���
		void Un_serve(char* svName);
		//������ֹͣ����������  eg:  ptzCmd = !serverName  ֹͣ  ��������
		//��Ҫ����ԱȨ��
		long Serv(const char* ptzCmd);
		//������ֹͣ����������  eg:  ptzCmd = !serverName  ֹͣ  ��������  
		//+serverName "·�� ����"��������
		//-serverName ж�ط���
		HRESULT serverMgr(const wchar_t* pctzCmd);
	}
	//ע��
	namespace INJECT
	{
		////dllPath DLL����·�� nProcessIDĿ����̵�ID
		//˵����Ŀ����̵ĸ�������ע����̵ĸ�������ͬ ����ע��ɹ�
		bool InjectPID(char* dllPath, int nProcessID);
		//appPath Ӧ�ó���·��  pCmdParam ���в���
		//dllPath dll����·��  
		//sw_parame  ��ʾӦ�ó���
		bool InjectApp(wchar_t* appPath, wchar_t* pCmdParam, char *dllPath, int sw_parame = true);
	}
	//����
	namespace DISK
	{
		//��ֵ�̷� -����
		int MountVdiskNum(int  DeviceNumber, char DriveLetter);
		//��ֵ�̷� -�ļ�
		int MountVdiskFile(char * FileName, char DriveLetter);
		//ж���̷�
		int UmountVdisk(char DriveLetter);
		//�������ƶ����̻����
		bool EjectVolume(wchar_t cDriveLetter);
		//��ȡ���ô����б�
		std::string GetDrivers();

		//��ȡ������Ϣ
		//NOType  ����Ҫ���豸����   �� DRIVE_FIXED
		std::string getDriverInfo(int NOType = -1);
		//�����Ƿ����
		bool  DiskIsExist(const wchar_t* pctzDriver);
		//��ȡ����ʣ��ռ�
		unsigned long long GetFreeDiskSpaceX(const wchar_t* pctzDriver);
		//�ж��Ƿ��ǿ��ƶ����� �磺DRIVE_FIXED    32  �ж��Ƿ��ǿ��ƶ�Ӳ��
		unsigned int GetDriveTypes(const wchar_t* pctzDriver);
		//��ȡ��������   NTFS  FAT
		const std::wstring GetVolumeInfo(const wchar_t* pszVolume);
		//��ȡ���̵�dos�ַ��� eg: pctsDriver--c:  ���� "\\"
		bool GetDriverDosName(const wchar_t* pctsDriver, std::wstring &rstDosName);
	}
	//�ļ�|Ŀ¼
	namespace DIR
	{
		//ɾ���ļ���Ŀ¼
		bool ShellDeleteFile(const wchar_t* pszFilePath);
		bool CmdDeleteFile(const wchar_t* pszFilePath);
		//��ȡ����Ŀ¼  --����dll��̬���÷�ʽ
		//--iCSIDL eg:CSIDL_APPDATA
		std::string ShellGetFolderPath(int iCSIDL);

	}
	//���뷨
	namespace KEYBOARD
	{
		//����Ĭ�����뷨
		//����������xp  win7    HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Keyboard Layouts  ��� L"e0200804"
		bool SetDefaultInput(wchar_t* pwszKLID);
		//ģ�ⰴ��
		//<��������1[_|^]>[,��������2][,��������3]...
		//����������"_"����,  "^"���ģ�ⰴ������ ����ģ�ⰴ�²�����
		//0x12_,0x09_,0x09^,0x12^     ע��ģ��Alt+Tab����������֧�� 16 ���ƺ�ʮ����
		long SendVKey(wchar_t* ptzCmd);
		//ģ�ⰴ��--֧������  "abc���!"
		void SendKeys(wchar_t *strMsg);
	}
	//ϵͳ����
	namespace SYSPARAME
	{
		//�������汳��
		bool  SetBkGround(wchar_t* jpgPath);
		//�ֱ���  --2017-12-13
		std::string GetDisplay();
		//���÷ֱ���  --2017-12-13
		bool SetDisplay(int pixX, int pixY);
		//ͬ������ʱ��
		bool UpdateSysTime(char* NTPIP = "time.windows.com");
	}
	//����
	namespace HOOK
	{
		void  MouseHook();
		void  KeyBoardHook();
	} 
}
 
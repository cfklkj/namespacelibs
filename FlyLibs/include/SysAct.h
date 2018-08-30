#pragma once

#include <string> 
#include <Windows.h>
 
namespace FLYLIB{
	namespace SysAct
	{
		//提升进程访问权限 如 SE_DEBUG_NAME
		bool Priv(const wchar_t* ptzName);
		//系统权限下正确获取用户路径信息必须
		bool SystemGetUserPrivilege();
		//调用SystemGetUserPrivilege 后必须 释放
		bool PrivilegeExecStop();

		void ExecWait(wchar_t* pCmd, int sw_parame, wchar_t* Directory, bool isWait);
		void ExecWait(wchar_t* pCmd, wchar_t* params, int sw_parame, wchar_t* Directory, bool isWait);
		void ExecWait(wchar_t* pCmd, wchar_t* Directory, bool isWait, bool isRunas);
		//动态调用
		void ExecWaitV2(wchar_t* pCmd, int sw_parame, wchar_t* Directory, bool isWait);
		//服务下 非system 权限运行用户层exe
		// szCmdLine  需要闯入 szCmdLine[] 的指针   不能直接 传入 字符串"notepad.exe"
		bool SysRunUser(wchar_t* szCmdLine, bool isWait = false);
	//	bool CheckRunExe(wchar_t * exeName, bool isWait);
	//	void WaitRunExeByTitle(wchar_t * exeTitle);
		//void WaitRunExeByHwnd(HWND hwnd);
		//管道命令行
		bool RunWithPipe(const char* path, char* param, bool show, bool isWait = true);
		//延迟函数  相当于 Sleep()
		void TimeDelay(unsigned long WaitTime);
		//关闭、重启系统
		long BootSystem(bool bReboot);
	}
	namespace SysInfo
	{ 
		//x64 还是 x86
		bool Is64bitSystem();
		//获取系统名称   如win7  win8
		std::string GetSystemName();
		//获取环境变量对应路径  如 %temp%\abc  -> c:\windows\temp\abc
		bool ExpandEnvironmentString(std::wstring& rstrStrings);
		//获取任务栏位置 
		//-1 0左 1上 2右 3下 4隐藏
		int GetTaskbarPos(int &width, int &height);
	}
	namespace AppInfo
	{
		//是否是x64进程
		bool Is64bitProcess(unsigned long dwProcessID);
		//获取进程pid
		bool GetProcessPid(wchar_t* processName, unsigned long &dwPid);
		//获取进程pid 和 进程名
		bool GetProcessList();
		//获取进程pid
		bool GetProcessPidFromList(wchar_t* processName, unsigned long &dwPid);
		//获取进程name
		bool GetProcessNameFromList(const unsigned long dwPid, wchar_t* processName);
		// Find process
		//指定进程名，参数为空时返回父进程ID
		long FindProc(const wchar_t* ptzCmd, bool isGetParent = false);
		//find processName
		wchar_t* FindProcName(const unsigned long ProcID, wchar_t* ProcessName);
		wchar_t* FindProcNameByHandle(HWND nlHandle, PTSTR ProcessName);
		//判断进程是否是system权限  1  512
		int GetProcessAuth(int pID = 0);
		//程序自删除
		//eg: waitSleep  = 0 程序不等待直接执行自删除 
		//eg: waitSleep  = 2 程序等待2秒后执行自删除
		void DeleteSelf(int waitSleep=0);
		//删除DLL自身 hModule值 为BOOL APIENTRY DllMain( HMODULE hModule 里的hModule 可  extern 方式获取
		//isInject 是否为注入形式的DLL
		void DeleteDllSelf(HMODULE hModule, bool isInject=false);
		//结束进程
		bool KillProcessById(unsigned long dwId);	
		//结束进程 通过进程名
		long KillProcessByName(const wchar_t* ptzCmd);
		//使程序只运行一次  
		bool IsExeRuned(const wchar_t * lpName);
		//获取当前目录
		std::wstring GetCurrentThisPath(HMODULE hModule);
		std::string GetCurrentThisPathA(HMODULE hModule);
		//设置环境变量当前目录
		bool SetCurrentThisPath();
		//写入用户开机启动项
		bool WriteToRunReg(const wchar_t* KeyName, const wchar_t * sFullExeCommand);
	}
	//服务
	//只对程序本身有效
	namespace SVC
	{
		
		//安装服务
		void create_serve(char* svName, char* pParam = NULL);

		//启动服务后运行服务函数 
		//srv_core_thread  要执行的线程函数  eg:DWORD WINAPI CServerAct::StartThread(LPVOID para)
		void  start_serve(void* srv_core_thread);

		//FuncAddr  --DWORD  
		//FuncName   --函数名称  
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
		//srv_core_thread  要执行的线程函数  eg:DWORD WINAPI CServerAct::StartThread(LPVOID para)
		//srv_End_thread  系统关机要执行的函数
		//srv_End_funcAddr 来源于GetMemberFuncAddr_VC8  函数形式为 void A(); 否则会出问题
		void  start_serve(void* srv_core_thread, DWORD srv_End_funcAddr);
		//停止服务
		void stop_serve(char* svName);

		//卸载服务
		void Un_serve(char* svName);
		//启动或停止第三方服务  eg:  ptzCmd = !serverName  停止  否则启动
		//需要管理员权限
		long Serv(const char* ptzCmd);
		//启动或停止第三方服务  eg:  ptzCmd = !serverName  停止  否则启动  
		//+serverName "路径 参数"创建服务
		//-serverName 卸载服务
		HRESULT serverMgr(const wchar_t* pctzCmd);
	}
	//注入
	namespace INJECT
	{
		////dllPath DLL绝对路径 nProcessID目标进程的ID
		//说明：目标进程的父进程与注入进程的父进程相同 才能注入成功
		bool InjectPID(char* dllPath, int nProcessID);
		//appPath 应用程序路径  pCmdParam 运行参数
		//dllPath dll绝对路径  
		//sw_parame  显示应用程序
		bool InjectApp(wchar_t* appPath, wchar_t* pCmdParam, char *dllPath, int sw_parame = true);
	}
	//磁盘
	namespace DISK
	{
		//赋值盘符 -磁盘
		int MountVdiskNum(int  DeviceNumber, char DriveLetter);
		//赋值盘符 -文件
		int MountVdiskFile(char * FileName, char DriveLetter);
		//卸载盘符
		int UmountVdisk(char DriveLetter);
		//弹出可移动磁盘或光驱
		bool EjectVolume(wchar_t cDriveLetter);
		//获取可用磁盘列表
		std::string GetDrivers();

		//获取磁盘信息
		//NOType  不需要的设备类型   如 DRIVE_FIXED
		std::string getDriverInfo(int NOType = -1);
		//磁盘是否可用
		bool  DiskIsExist(const wchar_t* pctzDriver);
		//获取磁盘剩余空间
		unsigned long long GetFreeDiskSpaceX(const wchar_t* pctzDriver);
		//判断是否是可移动磁盘 如：DRIVE_FIXED    32  判断是否是可移动硬盘
		unsigned int GetDriveTypes(const wchar_t* pctzDriver);
		//获取磁盘类型   NTFS  FAT
		const std::wstring GetVolumeInfo(const wchar_t* pszVolume);
		//获取磁盘的dos字符串 eg: pctsDriver--c:  不加 "\\"
		bool GetDriverDosName(const wchar_t* pctsDriver, std::wstring &rstDosName);
	}
	//文件|目录
	namespace DIR
	{
		//删除文件或目录
		bool ShellDeleteFile(const wchar_t* pszFilePath);
		bool CmdDeleteFile(const wchar_t* pszFilePath);
		//获取特殊目录  --两种dll动态调用方式
		//--iCSIDL eg:CSIDL_APPDATA
		std::string ShellGetFolderPath(int iCSIDL);

	}
	//输入法
	namespace KEYBOARD
	{
		//设置默认输入法
		//仅仅适用于xp  win7    HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Keyboard Layouts  获得 L"e0200804"
		bool SetDefaultInput(wchar_t* pwszKLID);
		//模拟按键
		//<按键代码1[_|^]>[,按键代码2][,按键代码3]...
		//按键代码以"_"结束,  "^"则仅模拟按键弹起 否则模拟按下并弹起
		//0x12_,0x09_,0x09^,0x12^     注：模拟Alt+Tab。按键代码支持 16 进制和十进制
		long SendVKey(wchar_t* ptzCmd);
		//模拟按键--支持中文  "abc你好!"
		void SendKeys(wchar_t *strMsg);
	}
	//系统参数
	namespace SYSPARAME
	{
		//设置桌面背景
		bool  SetBkGround(wchar_t* jpgPath);
		//分辨率  --2017-12-13
		std::string GetDisplay();
		//设置分辨率  --2017-12-13
		bool SetDisplay(int pixX, int pixY);
		//同步网络时间
		bool UpdateSysTime(char* NTPIP = "time.windows.com");
	}
	//钩子
	namespace HOOK
	{
		void  MouseHook();
		void  KeyBoardHook();
	} 
}
 
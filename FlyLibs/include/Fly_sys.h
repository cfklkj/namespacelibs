#pragma once
#include <Windows.h>
#include <tlhelp32.h>
#include <string>  

namespace Fly_sys {
	struct FLY_PROCESSENTRY32 {
		PROCESSENTRY32 p32;    //处理未定义错误问题
	};
	struct FLY_MODULEENTRY32 {
		MODULEENTRY32 m32;    //处理未定义错误问题
	};
	namespace Attribe {
		//检查系统版本是否是Vista或更高的版本  
		bool IsOsVersionVistaOrGreater();
	}
	namespace CPU {
		// 获取cpu占用率
		double getCPURateUsed();

		// 获取内存占用
		__int64 getMemoryUsed();

		//获取CPU内核数
		DWORD getNumberOfProcess();
	}
	namespace Process {
		//获取自身模块 
		HMODULE GetSelfModuleHandle(); 
		//延迟函数  相当于 Sleep()
		void TimeDelay(DWORD WaitTime);
		//通过进程句柄获取进程信息
		FLY_PROCESSENTRY32 getProcessByHandle(HWND Handle);
		//通过进程名获取进程信息
		//exeName  如  qq.exe  
		FLY_PROCESSENTRY32 getProcessByExeName(const std::string& exeName);
		//等待进程
		bool WaitProcess(DWORD dwProId);
		//运行程序命令
		//strCmd  <程序>,<命令>,[运行目录],[是否显示],[是否等待],[是否管理员权限运行]
		void Run(std::string& strCmd);
		//运行回调 https://www.cnblogs.com/poissonnotes/p/4396082.html
		typedef void(__stdcall *RunCallBack)(LPPROCESS_INFORMATION pi);
		//运行程序命令  程序应该是绝对地址或系统环境变量存在路径
		//strCmd  <程序>,[命令],[运行目录],[是否显示],[是否等待]
		bool RunWithPipe(std::string& strCmd, RunCallBack callBack = NULL);
		//是否是64位进程
		//false 有可能是权限问题
		bool is64BitPorcess(DWORD dwProcessID);
		//是否是系统进程
		//false 有可能是权限问题
		bool isSystemPro(DWORD pID);
		//使程序只运行一次  
		bool IsExeRuned(LPCTSTR lpName);
		//程序自删除
		void DeleteSelf(int waitSleep);
		//DLL自删除
		void DeleteDllSelf(HMODULE hModule, int waitSleep, bool isInject);
	}
	namespace Server {
		//安装服务
		//strCmd <serverName>,<路径及其命令>,[是否安装后启动服务]
		bool install(std::string& strCmd);
		//卸载服务
		bool uninstall(std::string& srtName);
		//停止服务
		void stop(std::string& srtName);
		//服务执行函数执行体
		//srv_End_funcAddr  服务结束后函数 xx() 的地址 通过GetMemberFuncAddr_VC8 获得
		void  serverRun(void* srv_core_thread, DWORD srv_End_funcAddr=NULL);
		//运行程序
		//<运行参数>,[是否显示窗体],[是否等待]
		bool runExe(std::string& szCmdLine); 
	}
	namespace System {
		//关闭、重启系统
		HRESULT bootSystem(bool bReboot);
		//是否是64位系统
		bool is64bitSystem();
		//获取系统任务栏状态位置有四种情况：左(0)、上(1)、右(2)、下(3)，此外要考虑任务栏自动隐藏(5)的情况
		//return pos
		int getTaskbarPos(int &width, int &height);
		//获取系统名称
		std::string getSystemName();
		//设置桌面背景
		bool setBkGround(std::string& strPath);
		//分辨率
		//获取分辨率信息 
		std::string getDisplay();
		//设置分辨率
		//pixX = 0 or pixY = 0  自动设置最佳分辨率
		bool setDisplay(int pixX, int pixY);
		//设置系统时间
		//[时间],[NTPIP|URL]   时间格式 2018-4-24 0:00:08
		bool setTime(std::string& strCmd);
	}
	namespace Key {

		//模拟按键
		//<按键代码1[_|^]>[,按键代码2][,按键代码3]...
		//按键代码以"_"结束,  "^"则仅模拟按键弹起 否则模拟按下并弹起
		//0x12_,0x09_,0x09^,0x12^     注：模拟Alt+Tab。按键代码支持 16 进制和十进制
		bool SendVKey(const std::string& strCmd);
		//宽字符
		void SendUnicode(wchar_t data);
		void SendAscii(wchar_t data, BOOL shift);
		//模拟按键--支持中文  "abc你好!" 
		void SendKeys(const std::string &strMsg);
	}
}
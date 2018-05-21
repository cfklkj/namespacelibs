#include <stdio.h>
#include <Windows.h> 
#include "..\include\fly_sys.h"  
#include "..\include\fly_string.h" 
#include "..\include\\FlyDefine.h"
#pragma comment(lib, "..\\libs\\debug\\flyLibs")
//#pragma comment(lib, "User32")
#define DemoSys1

 
void server_thread()
{
	std::string strCmd = "cmd.exe /c notepad,1,1";
	Fly_sys::Server::runExe(strCmd); 
}
void End()
{
	printf("end");
}

#ifdef DemoSys
void main(int argc, char* argv[])
#else
void DemoSys(int argc, char* argv[])
#endif // DEBUG
{ 
	{
		std::string info = Fly_sys::System::getDisplay().c_str();
		int step = 0;
		while (1)
		{
			printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
			printf("\n%d------%s\n", step, info.c_str());
			step++;
			Sleep(100);
		}
		getchar();  
		return;
	}
	{
		std::string strCmd = "2018-10-2 1:1:1";
		bool srtBool =  Fly_sys::System::setTime(strCmd);
		printf("setTime:%s", srtBool ? "true" : "false");
		getchar();
		strCmd = ",202.108.6.95";
		srtBool = Fly_sys::System::setTime(strCmd);
		printf("setTime:%s", srtBool ? "true" : "false"); 
		getchar();
		return;
	}
	{
		printf("%s", Fly_sys::System::getDisplay().c_str());
		getchar();
		return;
	} 
	{
		std::string strCmd = "C:\\Users\\weile\\Pictures\\Saved Pictures\\FeatureToastHeroImg.jpg";
		bool srtBool = Fly_sys::System::setBkGround(strCmd);
		printf("setBkGround:%s", srtBool ? "true" : "false");
		getchar();
		return;
	}
	{
		printf("%s", Fly_sys::System::getSystemName().c_str());
		getchar();
		return;
	}
	{ 

		if (argc > 1)
		{
			DWORD func = 0;
			GetMemberFuncAddr_VC8(func, End);
			Fly_sys::Server::serverRun(server_thread, func); 
		}
		else
		{
			std::string strCmd = "TestDemoSys,";
			strCmd += argv[0];
			strCmd += " 1,1";
			Fly_sys::Server::install(strCmd);
			getchar();
			strCmd = "TestDemoSys";
			Fly_sys::Server::uninstall(strCmd);
		}
		return;
	}
	{
		std::string strCmd = "DebugServer,cmd.exe notepad";
		bool srtBool = Fly_sys::Server::install(strCmd);
		printf("createServer:%s", srtBool ? "true" : "false");
		strCmd = "DebugServer";
		getchar();
		srtBool = Fly_sys::Server::uninstall(strCmd);
		printf("uninstall:%s", srtBool ? "true" : "false");
	}
	return;
	std::string strCmd = "cmd.exe,,0,1";
	Fly_sys::Process::RunWithPipe(strCmd);
	getchar();
	return;

	std::string ast = "BluestacksGP.exe"; 
	Fly_sys::FLY_PROCESSENTRY32 pe32 = Fly_sys::Process::getProcessByExeName(ast);//, &pe32); 
	Fly_sys::Process::WaitProcess(pe32.p32.th32ProcessID);

	strCmd = "D:\\tDemo_ iocp.exe,,,1,0,1";
	Fly_sys::Process::Run(strCmd);
}

 
#pragma once
#include <Windows.h>
#include <tlhelp32.h>
#include <string>  

namespace Fly_sys {
	struct FLY_PROCESSENTRY32 {
		PROCESSENTRY32 p32;    //����δ�����������
	};
	struct FLY_MODULEENTRY32 {
		MODULEENTRY32 m32;    //����δ�����������
	};
	namespace Attribe {
		//���ϵͳ�汾�Ƿ���Vista����ߵİ汾  
		bool IsOsVersionVistaOrGreater();
	}
	namespace CPU {
		// ��ȡcpuռ����
		double getCPURateUsed();

		// ��ȡ�ڴ�ռ��
		__int64 getMemoryUsed();

		//��ȡCPU�ں���
		DWORD getNumberOfProcess();
	}
	namespace Process {
		//�ӳٺ���  �൱�� Sleep()
		void TimeDelay(DWORD WaitTime);
		//ͨ�����̾����ȡ������Ϣ
		FLY_PROCESSENTRY32 getProcessByHandle(HWND Handle);
		//ͨ����������ȡ������Ϣ
		//exeName  ��  qq.exe  
		FLY_PROCESSENTRY32 getProcessByExeName(const std::string& exeName);
		//�ȴ�����
		bool WaitProcess(DWORD dwProId);
		//���г�������
		//strCmd  <����>,<����>,[����Ŀ¼],[�Ƿ���ʾ],[�Ƿ����ԱȨ������],[�Ƿ�ȴ�]
		void Run(std::string& strCmd);
		//���лص� https://www.cnblogs.com/poissonnotes/p/4396082.html
		typedef void(__stdcall *RunCallBack)(LPPROCESS_INFORMATION pi);
		//���г�������  ����Ӧ���Ǿ��Ե�ַ��ϵͳ������������·��
		//strCmd  <����>,[����],[����Ŀ¼],[�Ƿ���ʾ],[�Ƿ�ȴ�]
		bool RunWithPipe(std::string& strCmd, RunCallBack callBack = NULL);
		//�Ƿ���64λ����
		//false �п�����Ȩ������
		bool is64BitPorcess(DWORD dwProcessID);
		//�Ƿ���ϵͳ����
		//false �п�����Ȩ������
		bool isSystemPro(DWORD pID);
		//ʹ����ֻ����һ��  
		bool IsExeRuned(LPCTSTR lpName);
		//������ɾ��
		void DeleteSelf(int waitSleep);
		//DLL��ɾ��
		void DeleteDllSelf(HMODULE hModule, int waitSleep, bool isInject);
	}
	namespace Server {
		//��װ����
		//strCmd <serverName>,<·����������>,[�Ƿ�װ����������]
		bool install(std::string& strCmd);
		//ж�ط���
		bool uninstall(std::string& srtName);
		//ֹͣ����
		void stop(std::string& srtName);
		//����ִ�к���ִ����
		//srv_End_funcAddr  ����������� xx() �ĵ�ַ ͨ��GetMemberFuncAddr_VC8 ���
		void  serverRun(void* srv_core_thread, DWORD srv_End_funcAddr=NULL);
		//���г���
		//<���в���>,[�Ƿ���ʾ����],[�Ƿ�ȴ�]
		bool runExe(std::string& szCmdLine); 
	}
	namespace System {
		//�رա�����ϵͳ
		HRESULT bootSystem(bool bReboot);
		//�Ƿ���64λϵͳ
		bool is64bitSystem();
		//��ȡϵͳ������״̬λ���������������(0)����(1)����(2)����(3)������Ҫ�����������Զ�����(5)�����
		//return pos
		int getTaskbarPos(int &width, int &height);
		//��ȡϵͳ����
		std::string getSystemName();
		//�������汳��
		bool setBkGround(std::string& strPath);
		//�ֱ���
		//��ȡ�ֱ�����Ϣ 
		std::string getDisplay();
		//���÷ֱ���
		//pixX = 0 or pixY = 0  �Զ�������ѷֱ���
		bool setDisplay(int pixX, int pixY);
		//����ϵͳʱ��
		//[ʱ��],[NTPIP|URL]   ʱ���ʽ 2018-4-24 0:00:08
		bool setTime(std::string& strCmd);
	}
	namespace Key {

		//ģ�ⰴ��
		//<��������1[_|^]>[,��������2][,��������3]...
		//����������"_"����,  "^"���ģ�ⰴ������ ����ģ�ⰴ�²�����
		//0x12_,0x09_,0x09^,0x12^     ע��ģ��Alt+Tab����������֧�� 16 ���ƺ�ʮ����
		bool SendVKey(const std::string& strCmd);
		//���ַ�
		void SendUnicode(wchar_t data);
		void SendAscii(wchar_t data, BOOL shift);
		//ģ�ⰴ��--֧������  "abc���!" 
		void SendKeys(const std::string &strMsg);
	}
}
#include "..\Fly_sys.h" 
#include <Psapi.h>

namespace Fly_sys {
	namespace CPU {
		double getCPURateUsed()
		{
			HANDLE hProcess = ::GetCurrentProcess();
			static DWORD s_dwTickCountOld = 0;
			static LARGE_INTEGER s_lgProcessTimeOld = { 0 };
			static DWORD s_dwProcessorCoreNum = 0;
			if (!s_dwProcessorCoreNum)
			{
				SYSTEM_INFO sysInfo = { 0 };
				GetSystemInfo(&sysInfo);
				s_dwProcessorCoreNum = sysInfo.dwNumberOfProcessors;
			}
			double dbProcCpuPercent = 0;
			BOOL bRetCode = FALSE;

			FILETIME CreateTime, ExitTime, KernelTime, UserTime;
			LARGE_INTEGER lgKernelTime;
			LARGE_INTEGER lgUserTime;
			LARGE_INTEGER lgCurTime;

			bRetCode = GetProcessTimes(hProcess, &CreateTime, &ExitTime, &KernelTime, &UserTime);
			if (bRetCode)
			{
				lgKernelTime.HighPart = KernelTime.dwHighDateTime;
				lgKernelTime.LowPart = KernelTime.dwLowDateTime;
				lgUserTime.HighPart = UserTime.dwHighDateTime;
				lgUserTime.LowPart = UserTime.dwLowDateTime;
				lgCurTime.QuadPart = (lgKernelTime.QuadPart + lgUserTime.QuadPart);
				if (s_lgProcessTimeOld.QuadPart)
				{
					DWORD dwElepsedTime = ::GetTickCount() - s_dwTickCountOld;
					dbProcCpuPercent = (double)(((double)((lgCurTime.QuadPart - s_lgProcessTimeOld.QuadPart) * 100)) / dwElepsedTime) / 10000;
					dbProcCpuPercent = dbProcCpuPercent / s_dwProcessorCoreNum;
				}
				s_lgProcessTimeOld = lgCurTime;
				s_dwTickCountOld = ::GetTickCount();
			}

			return dbProcCpuPercent;
		}

		__int64 getMemoryUsed() {
			HANDLE handle = GetCurrentProcess();
			PROCESS_MEMORY_COUNTERS pmc;
			GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));
			return pmc.WorkingSetSize / 1000;
		}
		DWORD getNumberOfProcess()
		{
			SYSTEM_INFO si;
			GetSystemInfo(&si);
			return si.dwNumberOfProcessors;
		}
	}
}

 
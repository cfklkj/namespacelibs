#include "Fly_sys.h" 
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

namespace Fly_sys {
	namespace Attribe {
		//检查系统版本是否是Vista或更高的版本  
		bool IsOsVersionVistaOrGreater()
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
			//通过版本号，判断是否是vista及之后版本  
			if (versionInfo.dwMajorVersion > 5)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	} 
}
    
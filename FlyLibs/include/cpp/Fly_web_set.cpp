#include "..\Fly_web.h"
#include "..\Fly_string.h"

#include <Windows.h>
#include <wininet.h>
#pragma   comment   (lib,   "wininet.lib")
#pragma comment(lib, "Mpr")

#include <iphlpapi.h>  
#pragma comment(lib,"Iphlpapi.lib") 

#include <SetupAPI.h> 
#include <devguid.h>  
#include <cfgmgr32.h> 
#include <wlanapi.h> 
#pragma comment(lib,"setupapi.lib") 
#pragma comment(lib, "wlanapi.lib") 

#pragma warning(disable:4996)
 
namespace Fly_web
{ 
	namespace SETs {

		//�ı�״̬
		BOOL changeStatus(DWORD NewStatus, DWORD SelectedItem, HDEVINFO hDevInfo)
		{
			HCURSOR hCursor = NULL;
			SP_PROPCHANGE_PARAMS PropChangeParams = { sizeof(SP_CLASSINSTALL_HEADER) };
			SP_DEVINFO_DATA DeviceInfoData = { sizeof(SP_DEVINFO_DATA) };
			hCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));
			// Get a handle to the Selected Item. 
			if (!SetupDiEnumDeviceInfo(hDevInfo, SelectedItem, &DeviceInfoData))
			{
				//_tprintf(_T("SetupDiEnumDeviceInfo failed. %d\n", GetLastError()));
				return false;
			}
			//SP_PROPCHANGE_PARAMS PropChangeParams = { sizeof(SP_CLASSINSTALL_HEADER) };
			PropChangeParams.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;
			PropChangeParams.Scope = DICS_FLAG_GLOBAL;
			PropChangeParams.StateChange = NewStatus;

			if (!SetupDiSetClassInstallParams(hDevInfo, &DeviceInfoData, (SP_CLASSINSTALL_HEADER *)&PropChangeParams, sizeof(PropChangeParams)))
			{//����:����������Ч����Ч
				return false;
			}
			if (!SetupDiCallClassInstaller(DIF_PROPERTYCHANGE, hDevInfo, &DeviceInfoData))
			{
				return false;
			}
			return true;
		}

		//�ı�����
		bool changNetWork(DWORD NewStatus)
		{
			LPTSTR lpszMsg = NULL;
			bool isOk = false;
			try
			{
				//GUID_DEVCLASS_NET��ʾ���г������豸
				HDEVINFO hDevInfo = SetupDiGetClassDevs(&GUID_DEVCLASS_NET, NULL, NULL, DIGCF_PRESENT);
				if (INVALID_HANDLE_VALUE == hDevInfo)
				{
					//wprintf(L"INVALID_HANDLE_VALUE");
					return -1;
				}

				DWORD i;
				int Icount = 0;
				SP_DEVINFO_DATA DeviceInfoData;
				ZeroMemory(&DeviceInfoData, sizeof(SP_DEVINFO_DATA));
				DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

				for (i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &DeviceInfoData); ++i)
				{
					//Get DeviceInfo       
					LPTSTR buffer = NULL;
					DWORD buffersize = 0;
					DWORD Status, Problem;
					BOOL bDisabled = FALSE;
					while (!SetupDiGetDeviceRegistryProperty(
						hDevInfo,
						&DeviceInfoData,
						SPDRP_PHYSICAL_DEVICE_OBJECT_NAME, //Read the names of the designated card  //SPDRP_DEVICEDESC
						NULL,
						(PBYTE)buffer,
						buffersize,
						&buffersize)
						)
					{
						if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
						{
							if (buffer) LocalFree(buffer);
							buffer = (LPTSTR)LocalAlloc(LPTR, buffersize);
						}
						else
						{
							//_tprintf(_T("read network card failed!"));
						}
					}
					if (!Fly_string::FindSub(buffer, "_PCI"))  //�Ƿ��������豸
						continue;
					while (!SetupDiGetDeviceRegistryProperty(
						hDevInfo,
						&DeviceInfoData,
						SPDRP_DEVICEDESC, //Read the names of the designated card   
						NULL,
						(PBYTE)buffer,
						buffersize,
						&buffersize)
						)
					{
						if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
						{
							if (buffer) LocalFree(buffer);
							buffer = (LPTSTR)LocalAlloc(LPTR, buffersize);
						}
						else
						{
							//_tprintf(_T("read network card failed!"));
						}
					}
					if (int i = CM_Get_DevNode_Status(&Status, &Problem, DeviceInfoData.DevInst, 0) != CR_SUCCESS)
					{
						//_tprintf(_T("CM_Get_DevNode_Status failed. %d\n", GetLastError()));
						throw "failed\n";
					} 
					isOk = changeStatus(NewStatus, i, hDevInfo);// enable  
				}
				SetupDiDestroyDeviceInfoList(hDevInfo);//����һ���豸��Ϣ���ϣ������ͷ����й������ڴ�
				return isOk;
			}
			catch (TCHAR * pszError)
			{
				if (NULL != lpszMsg)
				{
					LocalFree((HLOCAL)lpszMsg);
				}
				return FALSE;
			}
		}
		//���û��������
		bool LinkWeb(bool isDiskconnect)
		{
			bool lnkState = Fly_web::RAW::isLinkInternet();
			if (!lnkState && isDiskconnect) //������ �����ɽ���
				return false;
			else if (lnkState && !isDiskconnect)//������ ����������
			{
				return false;
			}
			return changNetWork(isDiskconnect ? DICS_DISABLE : DICS_ENABLE);
		}
		//�Ƿ�������IP
		bool IsLANIP(unsigned int nIP) {
			typedef unsigned char uint8;
			uint8 nFirst = (uint8)nIP;
			uint8 nSecond = (uint8)(nIP >> 8);

			if (nFirst == 192 && nSecond == 168) // check this 1st, because those LANs IPs are mostly spreaded
				return true;

			if (nFirst == 172 && nSecond >= 16 && nSecond <= 31)
				return true;

			if (nFirst == 0 || nFirst == 10)
				return true;

			return false;
		} //��ȡMac��ַ 
		//��ȡ������ַ
		std::string GetMac()
		{ 
			PIP_ADAPTER_INFO pAdapterInfo;
			PIP_ADAPTER_INFO pAdapter = NULL;
			ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
			pAdapterInfo = (PIP_ADAPTER_INFO)malloc(ulOutBufLen);
			DWORD dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen);
			// ��һ�ε���GetAdapterInfo��ȡulOutBufLen��С
			if (dwRetVal == ERROR_BUFFER_OVERFLOW)
			{
				free(pAdapterInfo);
				pAdapterInfo = (IP_ADAPTER_INFO *)malloc(ulOutBufLen);
				dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen);
			}
			std::string rstStr = "";
			if (dwRetVal == NO_ERROR)
			{
				pAdapter = pAdapterInfo;
				while (pAdapter)
				{
					std::string IP = pAdapter->IpAddressList.IpAddress.String;
					std::string IPMask = pAdapter->IpAddressList.IpMask.String;
					std::string Gateway = pAdapter->GatewayList.IpAddress.String;
					char temp[2] = { 0 };
					sprintf(temp, "%c", IP[0]);
					if (temp[0] != '0')
					{
						char tempBuff[64];
						sprintf(tempBuff, "%02x-%02x-%02x-%02x-%02x-%02x",
							pAdapter->Address[0],
							pAdapter->Address[1],
							pAdapter->Address[2],
							pAdapter->Address[3],
							pAdapter->Address[4],
							pAdapter->Address[5]);
						rstStr = tempBuff;
						break;
					}
					pAdapter = pAdapter->Next;
				}
			} 
			free(pAdapterInfo);
			return rstStr;
		}
	} 
}
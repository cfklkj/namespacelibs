#include "..\Fly_sys.h"   
#include "..\Fly_string.h"
#include <Shlwapi.h> 
 

namespace Fly_sys {
	namespace Key {
		 
		bool SendVKey(const std::string& strCmd)
		{
			INT i = 0;
			const char* ptzCmd = strCmd.c_str();
			const char* p = ptzCmd;
			do
			{
				if ((*p == ',') || (*p == 0))
				{
					StrToIntEx(ptzCmd, STIF_SUPPORT_HEX, &i);   //字符串0x12转int
					if (*(p - 1) != '^')
					{
						keybd_event(i, 0, 0, 0);
					}
					if (*(p - 1) != '_')
					{
						keybd_event(i, 0, KEYEVENTF_KEYUP, 0);
					}
					ptzCmd = p + 1;
				}
			} while (*p++);
			return i ? true : false;
		}

		void SendAscii(wchar_t data, BOOL shift)
		{
			INPUT input[2];
			memset(input, 0, 2 * sizeof(INPUT));

			if (shift)
			{
				input[0].type = INPUT_KEYBOARD;
				input[0].ki.wVk = VK_SHIFT;
				SendInput(1, input, sizeof(INPUT));
			}

			input[0].type = INPUT_KEYBOARD;
			input[0].ki.wVk = data;

			input[1].type = INPUT_KEYBOARD;
			input[1].ki.wVk = data;
			input[1].ki.dwFlags = KEYEVENTF_KEYUP;

			SendInput(2, input, sizeof(INPUT));

			if (shift)
			{
				input[0].type = INPUT_KEYBOARD;
				input[0].ki.wVk = VK_SHIFT;
				input[0].ki.dwFlags = KEYEVENTF_KEYUP;
				SendInput(1, input, sizeof(INPUT));
			}
		}
		void SendUnicode(wchar_t data)
		{
			INPUT input[2];
			memset(input, 0, 2 * sizeof(INPUT));

			input[0].type = INPUT_KEYBOARD;
			input[0].ki.wVk = 0;
			input[0].ki.wScan = data;
			input[0].ki.dwFlags = 0x4;//KEYEVENTF_UNICODE;

			input[1].type = INPUT_KEYBOARD;
			input[1].ki.wVk = 0;
			input[1].ki.wScan = data;
			input[1].ki.dwFlags = KEYEVENTF_KEYUP | 0x4;//KEYEVENTF_UNICODE;

			SendInput(2, input, sizeof(INPUT));
		}

		//支持中文?
		void SendKeys(const std::string &strMsg)
		{
			short vk;
			BOOL shift;

			const char* data = strMsg.c_str();
			int len = strMsg.size();

			for (int i = 0;i<len;i++)
			{
				if (data[i] >= 0 && data[i]<256) //ascii字符
				{
					vk = VkKeyScanW(data[i]);

					if (vk == -1)
					{
						SendUnicode(data[i]);
					}
					else
					{
						if (vk < 0)
						{
							vk = ~vk + 0x1;
						}

						shift = vk >> 8 & 0x1;

						if (GetKeyState(VK_CAPITAL) & 0x1)
						{
							if (data[i] >= 'a' && data[i] <= 'z' || data[i] >= 'A' && data[i] <= 'Z')
							{
								shift = !shift;
							}
						}

						SendAscii(vk & 0xFF, shift);
					}
				}
				else //unicode字符
				{
					SendUnicode(data[i]);
				}
			}
		}
	}
}
 
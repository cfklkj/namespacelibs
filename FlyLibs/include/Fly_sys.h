#pragma once
#include <Windows.h>
#include <string> 

namespace Fly_sys {
	namespace CPU {
		// ��ȡcpuռ����
		double getCPURateUsed();

		// ��ȡ�ڴ�ռ��
		__int64 getMemoryUsed();

		//��ȡCPU�ں���
		DWORD getNumberOfProcess();
	}
}
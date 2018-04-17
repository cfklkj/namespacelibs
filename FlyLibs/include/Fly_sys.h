#pragma once
#include <Windows.h>
#include <string> 

namespace Fly_sys {
	namespace CPU {
		// 获取cpu占用率
		double getCPURateUsed();

		// 获取内存占用
		__int64 getMemoryUsed();

		//获取CPU内核数
		DWORD getNumberOfProcess();
	}
}
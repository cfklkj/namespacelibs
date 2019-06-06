#pragma once
#include <string>

namespace Fly_Time {
	namespace TIME {
		std::string GetShortTimeString();
		std::string GetDateString();
		std::string GetTimeString();
		std::string GetDateTimeString();
		std::string GetTimeStringWithoutDate();
		std::string GetTimeStringWithoutSpace();
		//当前时间过了nAddSecond秒后的时间 Y-M-D_h:m:s
		std::string GetDateAddString(int nAddSecond);
		std::string GetFallbackTime(int nDays);
		bool isTimeOut(const std::string strTime); //时间字符串格式 170802_161425
		int GetMonthCardTimeValue();
		int getToZeroTick();
		std::string getWeekstring(int year, int month, int day);
		std::string getWeekstring(const std::string strTime);
		tm TimeStrToTM(std::string timeString);
		/*
		string to time_t
		时间格式 2018-4-24 0:00:08 或 2018-4-24
		成功返回 0
		*/
		int stringToTime(const std::string &strDateStr, time_t &timeData);
		//秒数转时间格式  h:M:S
		std::string TickToTimeStr(int timeTick);
	}
}  

#include "fly_string.h"
#include "fly_time.h"

#include <string.h>
#include <time.h>
#include <Windows.h>
#include <vector> 

#pragma warning(disable:4996)
using namespace std;
namespace Fly_Time {
	namespace TIME {
		std::string GetShortTimeString()
		{
			SYSTEMTIME sys;
			GetLocalTime(&sys);
			return Fly_string::format("%2d%02d%02d_%02d%02d%02d", sys.wYear - 2000, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
		}
		std::string GetDateString()
		{
			SYSTEMTIME sys;
			GetLocalTime(&sys);
			return Fly_string::format("%4d-%02d-%02d", sys.wYear, sys.wMonth, sys.wDay);
		}
		std::string GetTimeString() {
			SYSTEMTIME sys;
			GetLocalTime(&sys);
			return Fly_string::format("%4d/%02d/%02d %02d:%02d:%02d.%03d", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
		}
		std::string GetDateTimeString()
		{
			SYSTEMTIME sys;
			GetLocalTime(&sys);
			return Fly_string::format("%4d-%02d-%02d %02d:%02d:%02d", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);
		}
		std::string GetTimeStringWithoutDate()
		{
			SYSTEMTIME sys;
			GetLocalTime(&sys);
			return Fly_string::format("%02d:%02d:%02d", sys.wHour, sys.wMinute, sys.wSecond);
		}
		std::string GetTimeStringWithoutSpace()
		{
			SYSTEMTIME sys;
			GetLocalTime(&sys);
			return Fly_string::format("%4d-%02d-%02d %02d_%02d_%02d.%03d", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
		}
		std::string GetDateAddString(int nAddSecond) //当前时间过了nAddSecond秒后的时间
		{
			time_t  now;
			struct tm ts;
			time(&now);
			now = now + nAddSecond; //增加多少秒
			ts = *localtime(&now);
			char Ttime[30] = { "" };	//当前时间	
			sprintf_s(Ttime, sizeof(Ttime), "%4d-%02d-%02d_%02d:%02d:%02d", ts.tm_year - 100 + 2000,
				ts.tm_mon + 1,
				ts.tm_mday,
				ts.tm_hour,
				ts.tm_min,
				ts.tm_sec);
			std::string strTime = Ttime;
			return strTime;
		}

		std::string GetFallbackTime(int nDays)
		{
			time_t  now;
			struct tm  ts;
			time(&now);
			now = now - 24 * 3600 * nDays;
			ts = *localtime(&now);
			char Ttime[30] = { "" };	//当前时间	
			sprintf_s(Ttime, sizeof(Ttime), "%2d%02d%02d_%02d%02d%02d", ts.tm_year - 100,
				ts.tm_mon + 1,
				ts.tm_mday,
				ts.tm_hour,
				ts.tm_min,
				ts.tm_sec);
			std::string strTime = Ttime;
			return strTime;
		}
		bool isTimeOut(const std::string strTime) //时间字符串格式 2017-08-23_11:25:30
		{
			tm tm_;
			int year, month, day, hour = 0, minute = 0, second = 0;
			std::vector<std::string> vTime = Fly_string::splitEx(strTime, "_");
			if (vTime.size() != 2) return false;
			char szTime[30] = { "" };
			sprintf_s(szTime, sizeof(szTime), "%s %s", vTime[0].c_str(), vTime[1].c_str());
			sscanf(szTime, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);
			tm_.tm_year = year - 1900;
			tm_.tm_mon = month - 1;
			tm_.tm_mday = day;
			tm_.tm_hour = hour;
			tm_.tm_min = minute;
			tm_.tm_sec = second;
			tm_.tm_isdst = 0;

			time_t t_ = mktime(&tm_); //已经减了8个时区  

			time_t  t_now;
			t_now = time(NULL);

			int nSecond = (int)(t_now - t_);
			if (nSecond > 0)
				return true;
			else
				return false;
		}

		/*
		string to time_t
		时间格式 2018-4-24 0:00:08 或 2018-4-24
		*/
		int stringToTime(const std::string &strDateStr, time_t &timeData)
		{
			char *pBeginPos = (char*)strDateStr.c_str();
			char stepDay = ' ';
			char stepTime = ' ';
			if (strDateStr.find('-') != -1)
				stepDay = '-';
			if (strDateStr.find('/') != -1)
				stepDay = '/';
			if (strDateStr.find(':') != -1)
				stepTime = ':';
			if (stepDay == ' ')
				return -1;
			std::string skip;
			skip.push_back(stepDay);
			char *pPos = strstr(pBeginPos, skip.c_str());
			if (pPos == NULL)
			{
				printf("strDateStr[%s] err \n", strDateStr.c_str());
				return -1;
			}
			int iYear = atoi(pBeginPos);
			int iMonth = atoi(pPos + 1);
			pPos = strstr(pPos + 1, skip.c_str());
			if (pPos == NULL)
			{
				printf("strDateStr[%s] err \n", strDateStr.c_str());
				return -1;
			}

			int iDay = atoi(pPos + 1);
			int iHour = 0;
			int iMin = 0;
			int iSec = 0;
			if (stepTime == ' ')
				return -1;
			pPos = strstr(pPos + 1, " ");
			//为了兼容有些没精确到时分秒的  
			if (pPos != NULL)
			{
				iHour = atoi(pPos + 1);
				pPos = strstr(pPos + 1, ":");
				if (pPos != NULL)
				{
					iMin = atoi(pPos + 1);
					pPos = strstr(pPos + 1, ":");
					if (pPos != NULL)
					{
						iSec = atoi(pPos + 1);
					}
				}
			}

			struct tm sourcedate;
			memset((void*)&sourcedate, 0, sizeof(sourcedate));
			sourcedate.tm_sec = iSec;
			sourcedate.tm_min = iMin;
			sourcedate.tm_hour = iHour;
			sourcedate.tm_mday = iDay;
			sourcedate.tm_mon = iMonth - 1;
			sourcedate.tm_year = iYear - 1900;
			timeData = mktime(&sourcedate);
			return 0;
		}

		std::string TickToTimeStr(int timeTick)
		{
			int h = timeTick / 3600;
			int m = timeTick / 60 % 60;
			int s = timeTick % 60;
			return Fly_string::format("%02d:%02d:%02d", h, m ,s);
		}

		int GetMonthCardTimeValue()
		{
			SYSTEMTIME sys;
			GetLocalTime(&sys);
			std::string dateStr = Fly_string::format("%2d%02d%02d", sys.wYear, sys.wMonth, sys.wDay);
			return atoi(dateStr.c_str());
		}

		bool isRunyear(int year)
		{
			if (year % 4 == 0 && year % 100 != 0 || year % 400 == 0)
			{
				return true;
			}
			else {
				return false;
			}
		}
		struct MonthDay {
			int Day;
			int month;
		};
		bool getYearMonth(int year, int day, MonthDay& retYD) {
			int iArr[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
			int count = 0, count1 = 0;
			if (isRunyear(year)) //表明是闰年 
			{
				iArr[1] = 29;
			}
			for (int i = 0; i < 11; i++)
			{
				count1 = count;
				count += iArr[i];
				if (day >= count1 && day <= count) {
					retYD.month = i + 1;
					retYD.Day = day - count1;
					break;
				}
			}
			return true;
		}

		std::string getWeekstring(int year, int month, int day)
		{
			int sum, leap;

			//计算某月以前月份的天数
			switch (month) {
			case 1:sum = 0;break;
			case 2:sum = 31;break;
			case 3:sum = 59;break;
			case 4:sum = 90;break;
			case 5:sum = 120;break;
			case 6:sum = 151;break;
			case 7:sum = 181;break;
			case 8:sum = 212;break;
			case 9:sum = 243;break;
			case 10:sum = 273;break;
			case 11:sum = 304;break;
			case 12:sum = 334;break;
			default:
				return "data error!";
				break;
			}

			//月份天数加上这个月的日数
			sum = sum + day;

			if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) //判断是否闰年
				leap = 1;
			else
				leap = 0;

			if (leap == 1 && month>2)sum = sum + 1;

			int inWeekDay = sum % 7;
			int lastWeekDay = sum + 7 - inWeekDay;
			if (inWeekDay == 0)
			{
				inWeekDay = 7;
				lastWeekDay = sum;
			}
			int headWeekDay = sum - inWeekDay + 1;
			std::string retMDstr = "";
			MonthDay td, td2;
			getYearMonth(year, headWeekDay, td);
			getYearMonth(year, lastWeekDay, td2);
			retMDstr = Fly_string::format("%02d.%02d~%02d.%02d", td.month, td.Day, td2.month, td2.Day);
			return retMDstr;
		}
		std::string getWeekstring(const std::string strTime)
		{
			int year, month, day, hour = 0, minute = 0, second = 0;
			sscanf(strTime.c_str(), "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);
			return getWeekstring(year, month, day);
		}

		int  getToZeroTick()
		{
			return  (86400 - time(NULL) % 86400 - 8 * 3600) > 0 ? 86400 - time(NULL) % 86400 - 8 * 3600 : 86400 * 2 - time(NULL) % 86400 - 8 * 3600;
		}

		tm TimeStrToTM(string timeString) {
			tm ret = {};
			auto vecStrs = Fly_string::splitEx(timeString, ":");

			if (vecStrs.size() < 2) {
				return ret;
			}

			ret.tm_hour = atoi(vecStrs[0].c_str());
			ret.tm_min = atoi(vecStrs[1].c_str());
			return ret;
		}
	}
}





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
		std::string GetDateAddString(int nAddSecond); //��ǰʱ�����nAddSecond����ʱ��
		std::string GetFallbackTime(int nDays);
		bool isTimeOut(const std::string strTime); //ʱ���ַ�����ʽ 170802_161425
		int GetMonthCardTimeValue();
		int getToZeroTick();
		std::string getWeekstring(int year, int month, int day);
		std::string getWeekstring(const std::string strTime);
		tm TimeStrToTM(std::string timeString);
		/*
		string to time_t
		ʱ���ʽ 2018-4-24 0:00:08 �� 2018-4-24
		�ɹ����� 0
		*/
		int stringToTime(const std::string &strDateStr, time_t &timeData);
	}
}  
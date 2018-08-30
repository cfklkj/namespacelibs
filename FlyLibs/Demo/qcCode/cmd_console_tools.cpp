#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include "cmd_console_tools.h"
using namespace std;

/***************************************************************************
函数名称：
功    能：设置指定的颜色
输入参数：const HANDLE hout  ：cmd窗口句柄
const int bg_color ：背景色（0-15）
const int fg_color ：前景色（0-15）
返 回 值：
说    明：颜色的取值由背景色+前景色组成，各16种
fg_color：0-7    黑   蓝   绿   青   红   粉   黄   白
8-15 亮黑 亮蓝 亮绿 亮青 亮红 亮粉 亮黄 亮白
bg_color：0-7    黑   蓝   绿   青   红   粉   黄   白
8-15 亮黑 亮蓝 亮绿 亮青 亮红 亮粉 亮黄 亮白
最终的颜色为 背景色*16+前景色
***************************************************************************/
void setcolor(const HANDLE hout, const int bg_color, const int fg_color)
{
	SetConsoleTextAttribute(hout, bg_color * 16 + fg_color);
}

/***************************************************************************
函数名称：
功    能：将光标移动到指定位置
输入参数：const HANDLE hout ：cmd窗口句柄
const int X       ：X轴坐标（列）
const int Y       ：Y轴坐标（行）
返 回 值：
说    明：屏幕左上角坐标为(0,0)，在cmd窗口的大小未被调整的情况下，缺省为：
横向x轴，对应列(0-79)
纵向y轴，对应行(0-24)
***************************************************************************/
void gotoxy(const HANDLE hout, const int X, const int Y)
{
	COORD coord;
	coord.X = X;
	coord.Y = Y;
	SetConsoleCursorPosition(hout, coord);
}

/***************************************************************************
函数名称：
功    能：改变cmd窗口的大小
输入参数：const HANDLE hout ：cmd窗口句柄
const int cols    : 新的列数
const int lines   ：新的行数
返 回 值：
说    明：
***************************************************************************/
void setconsoleborder(const HANDLE hout, const int cols, const int lines, const int buffer_lines)
{
	char cmd[80];

	/* 恢复为背景色0、前景色白（cmd窗口的缺省值）*/
	setcolor(hout, COLOR_BLACK, COLOR_WHITE);
	system("cls");	//清屏

	sprintf(cmd, "mode con cols=%d lines=%d", cols, lines);
	system(cmd); //设置新的大小

				 /* 设置屏幕缓冲区的大小（缺省则与屏幕高度一致，不用设置）*/
	if (buffer_lines >= lines) {
		COORD cr;
		cr.X = cols;
		cr.Y = buffer_lines;
		SetConsoleScreenBufferSize(hout, cr); //设置cmd窗口的屏幕缓冲区的大小
	}

	return;
}

/***************************************************************************
函数名称：
功    能：改变输出窗口的字体
输入参数：
返 回 值：
说    明：
***************************************************************************/
void setfontsize(const HANDLE hout, const wchar_t *fontname, const int fontsize)
{
	CONSOLE_FONT_INFOEX info = { 0 };
	info.cbSize = sizeof(info);
	info.dwFontSize.Y = fontsize; // leave X as zero
	info.FontWeight = FW_NORMAL;
	wcscpy(info.FaceName, fontname);	//字体
	SetCurrentConsoleFontEx(hout, NULL, &info);
}
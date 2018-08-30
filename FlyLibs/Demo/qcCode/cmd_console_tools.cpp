#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include "cmd_console_tools.h"
using namespace std;

/***************************************************************************
�������ƣ�
��    �ܣ�����ָ������ɫ
���������const HANDLE hout  ��cmd���ھ��
const int bg_color ������ɫ��0-15��
const int fg_color ��ǰ��ɫ��0-15��
�� �� ֵ��
˵    ������ɫ��ȡֵ�ɱ���ɫ+ǰ��ɫ��ɣ���16��
fg_color��0-7    ��   ��   ��   ��   ��   ��   ��   ��
8-15 ���� ���� ���� ���� ���� ���� ���� ����
bg_color��0-7    ��   ��   ��   ��   ��   ��   ��   ��
8-15 ���� ���� ���� ���� ���� ���� ���� ����
���յ���ɫΪ ����ɫ*16+ǰ��ɫ
***************************************************************************/
void setcolor(const HANDLE hout, const int bg_color, const int fg_color)
{
	SetConsoleTextAttribute(hout, bg_color * 16 + fg_color);
}

/***************************************************************************
�������ƣ�
��    �ܣ�������ƶ���ָ��λ��
���������const HANDLE hout ��cmd���ھ��
const int X       ��X�����꣨�У�
const int Y       ��Y�����꣨�У�
�� �� ֵ��
˵    ������Ļ���Ͻ�����Ϊ(0,0)����cmd���ڵĴ�Сδ������������£�ȱʡΪ��
����x�ᣬ��Ӧ��(0-79)
����y�ᣬ��Ӧ��(0-24)
***************************************************************************/
void gotoxy(const HANDLE hout, const int X, const int Y)
{
	COORD coord;
	coord.X = X;
	coord.Y = Y;
	SetConsoleCursorPosition(hout, coord);
}

/***************************************************************************
�������ƣ�
��    �ܣ��ı�cmd���ڵĴ�С
���������const HANDLE hout ��cmd���ھ��
const int cols    : �µ�����
const int lines   ���µ�����
�� �� ֵ��
˵    ����
***************************************************************************/
void setconsoleborder(const HANDLE hout, const int cols, const int lines, const int buffer_lines)
{
	char cmd[80];

	/* �ָ�Ϊ����ɫ0��ǰ��ɫ�ף�cmd���ڵ�ȱʡֵ��*/
	setcolor(hout, COLOR_BLACK, COLOR_WHITE);
	system("cls");	//����

	sprintf(cmd, "mode con cols=%d lines=%d", cols, lines);
	system(cmd); //�����µĴ�С

				 /* ������Ļ�������Ĵ�С��ȱʡ������Ļ�߶�һ�£��������ã�*/
	if (buffer_lines >= lines) {
		COORD cr;
		cr.X = cols;
		cr.Y = buffer_lines;
		SetConsoleScreenBufferSize(hout, cr); //����cmd���ڵ���Ļ�������Ĵ�С
	}

	return;
}

/***************************************************************************
�������ƣ�
��    �ܣ��ı�������ڵ�����
���������
�� �� ֵ��
˵    ����
***************************************************************************/
void setfontsize(const HANDLE hout, const wchar_t *fontname, const int fontsize)
{
	CONSOLE_FONT_INFOEX info = { 0 };
	info.cbSize = sizeof(info);
	info.dwFontSize.Y = fontsize; // leave X as zero
	info.FontWeight = FW_NORMAL;
	wcscpy(info.FaceName, fontname);	//����
	SetCurrentConsoleFontEx(hout, NULL, &info);
}
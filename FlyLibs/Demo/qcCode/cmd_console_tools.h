#include <Windows.h>

/* ∂®“Â—’…´*/
#define COLOR_BLACK		0	//∫⁄
#define COLOR_BLUE		1	//¿∂
#define COLOR_GREEN		2	//¬Ã
#define COLOR_CYAN		3	//«‡
#define COLOR_RED		4	//∫Ï
#define COLOR_PINK		5	//∑€
#define COLOR_YELLOW	6	//ª∆
#define COLOR_WHITE		7	//∞◊
#define COLOR_HBLACK	8	//¡¡∫⁄
#define COLOR_HBLUE		9	//¡¡¿∂
#define COLOR_HGREEN	10	//¡¡¬Ã
#define COLOR_HCYAN		11	//¡¡«‡
#define COLOR_HRED		12	//¡¡∫Ï
#define COLOR_HPINK		13	//¡¡∑€
#define COLOR_HYELLOW	14	//¡¡ª∆
#define COLOR_HWHITE	15	//¡¡∞◊

void setcolor(const HANDLE hout, const int bg_color, const int fg_color);
void gotoxy(const HANDLE hout, const int X, const int Y);
void setconsoleborder(const HANDLE hout, const int cols, const int lines, const int buffer_lines = -1);
void setfontsize(const HANDLE hout, const wchar_t *fontname, const int fontsize);
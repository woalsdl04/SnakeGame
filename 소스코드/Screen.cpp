
#include "Screen.h"

static int g_nScreenIndex;
static HANDLE g_hScreen[2];

void ScreenInit() {
	
	system("mode con: cols=100 lines=45");
	
	CONSOLE_CURSOR_INFO cci;

	//화면 버퍼 2개 생성
	g_hScreen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	g_hScreen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	COORD  dwSize;
	dwSize.X = 100;
	dwSize.Y = 40;

	//SetConsoleScreenBufferSize(g_hScreen[0], dwSize);
	//SetConsoleScreenBufferSize(g_hScreen[1], dwSize);

		
	//커서 숨김
	cci.dwSize = 1;
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(g_hScreen[0], &cci);
	SetConsoleCursorInfo(g_hScreen[1], &cci);
}

void ScreenFlipping() {
	SetConsoleActiveScreenBuffer(g_hScreen[g_nScreenIndex]);
	g_nScreenIndex = !g_nScreenIndex;
}

void ScreenClear() {
	COORD Coord = { 0, 0 };
	DWORD dw;
	FillConsoleOutputCharacter(g_hScreen[g_nScreenIndex], ' ', 100 * 45, Coord, &dw);
}
void ScreenRelease() {
	CloseHandle(g_hScreen[0]);
	CloseHandle(g_hScreen[1]);
}
void ScreenPrint(int x, int y, const char* string) {
	DWORD dw;
	COORD CursorPosition = { x, y };
	SetConsoleCursorPosition(g_hScreen[g_nScreenIndex], CursorPosition);
	WriteFile(g_hScreen[g_nScreenIndex], string, strlen(string), &dw, NULL);
}


void SetColor(int color) {
	SetConsoleTextAttribute(g_hScreen[g_nScreenIndex], color);
}
#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <Windows.h>
#include <stdio.h>
#include <conio.h>

void ScreenInit();
void ScreenFlipping();
void ScreenClear();
void ScreenRelease();
void ScreenPrint(int x, int y, const char* string);
void SetColor(int color);

#endif __SCREEN_H__


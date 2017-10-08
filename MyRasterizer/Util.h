#pragma once
#include "Math3D.h"

extern const int WinWidth;
extern const int WinHeight;

extern DWORD dwFrameBuffer[];

// ARGB format(a is 0)
DWORD RGBA(int r, int g, int b, int a);
// ARGB format(a is 0)
DWORD RGBA(int r, int g, int b);
// from Vec4 to DWORD
DWORD RGBA(Vec4 color);
// from DWORD to float
void ToRGB(DWORD rbg, int &r, int &g, int &b);

#define ColorBlack	RGBA(0, 0, 0)
#define ColorWhite	RGBA(255, 255, 255)
#define ColorRED	RGBA(255, 0, 0)
#define ColorGREEN	RGBA(0, 255, 0)
#define ColorBLUE	RGBA(0, 0, 255)

//std::string numberToStr(int n) {//int to string
//	char numbuf[128];
//	sprintf_s(numbuf, "%i", n);//there is no itoa in xcode
//	std::string numStr = numbuf;
//	return numStr;
//}
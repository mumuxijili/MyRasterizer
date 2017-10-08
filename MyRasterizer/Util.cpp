#include "stdafx.h"
#include "Util.h"

const int WinWidth = 1024;
const int WinHeight = 768;

DWORD dwFrameBuffer[WinWidth * WinHeight];

DWORD RGBA(int r, int g, int b, int a)
{
	return ((b)+((g) << 8) + ((r) << 16) + ((a) << 24));
}

DWORD RGBA(int r, int g, int b)
{
	return ((b)+((g) << 8) + ((r) << 16) + ((0) << 24));
}

DWORD RGBA(Vec4 color)
{
	return (((int)color.z * 255)+(((int)color.y * 255) << 8) + (((int)color.x * 255) << 16) + ((0) << 24));
}

void ToRGB(DWORD rbg, int &r, int &g, int &b)
{
	r = rbg >> 16 & 0xff;
	g = rbg >> 8 & 0xff;
	b = rbg & 0xff;
}
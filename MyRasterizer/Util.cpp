#include "stdafx.h"
#include "Util.h"

const int WinWidth = 1024;
const int WinHeight = 768;

DWORD dwFrameBuffer[WinWidth * WinHeight];
float fZBuffer[WinWidth * WinHeight];

DWORD RGBA(int r, int g, int b, int a)
{
	return ((b)+((g) << 8) + ((r) << 16) + ((a) << 24));
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

float LerpFloat(float fStart, float fEnd, float t)
{
	return fStart + (fEnd - fStart) * t;
}

DWORD LerpColor(DWORD color1, DWORD color2, float t)
{
	int r1, g1, b1, r2, g2, b2;
	ToRGB(color1, r1, g1, b1);
	ToRGB(color2, r2, g2, b2);

	int r = LerpFloat(r1, r2, t);
	int g = LerpFloat(g1, g2, t);
	int b = LerpFloat(b1, b2, t);
	return RGB(r, g, b);
}

Vec4 LerpVector(Vec4 vStart, Vec4 vEnd, float t)
{
	float x1, y1, z1, w1, x2, y2, z2, w2;
	x1 = vStart.x;
	y1 = vStart.y;
	z1 = vStart.z;
	w1 = vStart.w;
	x2 = vEnd.x;
	y2 = vEnd.y;
	z2 = vEnd.z;
	w2 = vEnd.w;
	float x = LerpFloat(x1, x2, t);
	float y = LerpFloat(y1, y2, t);
	float z = LerpFloat(z1, z2, t);
	float w = LerpFloat(w1, w2, t);
	return Vec4(x, y, z, w);
}
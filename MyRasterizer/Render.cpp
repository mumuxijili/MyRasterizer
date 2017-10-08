#include "stdafx.h"
#include "Render.h"

vector<Vertex> g_vVertexBuffer;
vector<UINT> g_vIndexBuffer;

int DrawPixel(int x, int y, DWORD color)
{
	if (x < 0 || y < 0 || x >= WinWidth || y >= WinHeight) {
		return -1;
	}
	dwFrameBuffer[y * WinWidth + x] = color;
	return 0;
}

int DrawLine(int x0, int y0, int x1, int y1, DWORD color)
{
	// Bresenham algorithm
	int x, y, dx, dy, dx2, dy2, xstep, ystep, error, index;
	x = x0;
	y = y0;
	dx = x1 - x0;
	dy = y1 - y0;

	if (dx >= 0)	// left to right
		xstep = 1;	// x step 1
	else {			// right to left
		xstep = -1; // x step -1
		dx = -dx;	// abs x
	}

	if (dy >= 0)	// top to bottom
		ystep = 1;	// y step 1
	else {			// bottom to top
		ystep = -1; // y step -1
		dy = -dy;	// abs y
	}

	dx2 = dx << 1;	// 2 * dx
	dy2 = dy << 1;	// 2 * dy

	if (dx > dy) {	// line near x
		error = dy2 - dx;
		for (index = 0; index <= dx; ++index) {
			DrawPixel(x, y, color);
			if (error >= 0) {
				error -= dx2;
				y += ystep;
			}
			error += dy2;
			x += xstep;
		}
	}
	else {			// line near y
		error = dx2 - dy;
		for (index = 0; index <= dy; ++index) {
			DrawPixel(x, y, color);
			if (error >= 0) {
				error -= dy2;
				x += xstep;
			}
			error += dx2;
			y += ystep;
		}
	}

	return 0;
}
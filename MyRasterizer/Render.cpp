#include "stdafx.h"
#include "Render.h"

vector<Vertex> g_vVertexBuffer;
vector<UINT> g_vIndexBuffer;
Camera g_cam;

void Render::InitRender()
{
	m_eRenderMode = RenderMode::RM_WIREFRAME;
}

int Render::DrawPixel(int x, int y, DWORD color)
{
	// use screen space coordinate
	if (x < 0 || y < 0 || x >= WinWidth || y >= WinHeight) {
		return -1;
	}
	dwFrameBuffer[y * WinWidth + x] = color;
	return 0;
}

int Render::DrawLine(int x0, int y0, int x1, int y1, DWORD color)
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

void Render::DrawScanLine(Vertex vLeft, Vertex vRight, int y)
{
	float leftX = vLeft.m_SSCoord.x;
	float rightX = vRight.m_SSCoord.x;
	int iLeftX = (int)(leftX + 0.5f);
	int iRightX = (int)(rightX + 0.5f);

	for (int x = iLeftX; x <= iRightX; x++)
	{
		if (x >= 0 && x < WinWidth)
		{
			float dx = rightX - leftX;
			float t = 0.0f;
			if ((dx < -EPSINON) || (dx > EPSINON))
			{
				t = (x - leftX) / dx;
			}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////!!Optimize Me: Lerp costs too much////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Vertex v;
			//v = LerpVertex(vLeft, vRight, t);
			DWORD color = LerpColor(vLeft.m_vertexColor, vRight.m_vertexColor, t);
			//DWORD color = ColorBlack;
			// depth test
			if (1)
			{
				DrawPixel(x, y, color);
			}
		}
		if (x < 0 || x >= WinWidth)
		{
			break;
		}
	}
}

// v0\------/v1
//    \    /
//     \  /
//    v2\/
void Render::DrawTrianglePlaneTop(Vertex vTopLeft, Vertex vTopRight, Vertex vBottom)
{
	float topY = vTopLeft.m_SSCoord.y;
	float bottomY = vBottom.m_SSCoord.y;
	int iTopY = (int)(topY + 0.5);
	int iBottomY = (int)(bottomY + 0.5);

	// dx / dy, use as x step
	const double dDxDyLeft = (vTopLeft.m_SSCoord.x - vBottom.m_SSCoord.x) / (vTopLeft.m_SSCoord.y - vBottom.m_SSCoord.y);
	const double dDxDyRight = (vTopRight.m_SSCoord.x - vBottom.m_SSCoord.x) / (vTopRight.m_SSCoord.y - vBottom.m_SSCoord.y);

	for (int y = iBottomY; y <= iTopY; y++)
	{

		if (y >= 0 && y < WinHeight)
		{
			// find left and right vertex
			Vertex vLeft, vRight;
			float t = (y - bottomY) / (topY - bottomY);
			vLeft = LerpVertex(vBottom, vTopLeft, t);
			vRight = LerpVertex(vBottom, vTopRight, t);

			//int xLeft = (int)(dDxDyLeft * (y - bottomY) + vBottom.m_SSCoord.x + 0.5);
			//int xRight = (int)(dDxDyRight * (y - bottomY) + vBottom.m_SSCoord.x + 0.5);
			//vLeft.m_SSCoord = Vec4(xLeft, y, 0, 0);
			//vRight.m_SSCoord = Vec4(xRight, y, 0, 0);

			DrawScanLine(vLeft, vRight, y);
		}
		if (y < 0 || y >= WinHeight)
		{
			break;
		}
	}
}

//      /\v0
//     /  \
//    /    \
// v2/______\v1
void Render::DrawTrianglePlaneBottom(Vertex vTop, Vertex vBottomRight, Vertex vBottomLeft)
{
	float topY = vTop.m_SSCoord.y;
	float bottomY = vBottomLeft.m_SSCoord.y;
	int iTopY = (int)(topY + 0.5f);
	int iBottomY = (int)(bottomY + 0.5f);
	
	// dx / dy, use as x step
	const double dDxDyLeft = (vTop.m_SSCoord.x - vBottomLeft.m_SSCoord.x) / (vTop.m_SSCoord.y - vBottomLeft.m_SSCoord.y);
	const double dDxDyRight = (vTop.m_SSCoord.x - vBottomRight.m_SSCoord.x) / (vTop.m_SSCoord.y - vBottomRight.m_SSCoord.y);

	for (int y = iBottomY; y <= iTopY; y++)
	{
		
		if (y >= 0 && y < WinHeight)
		{
			// find left and right vertex
			Vertex vLeft, vRight;
			float t = (y - bottomY) / (topY - bottomY);
			vLeft = LerpVertex(vBottomLeft, vTop, t);
			vRight = LerpVertex(vBottomRight, vTop, t);

			//int xLeft = (int)(dDxDyLeft * (y - bottomY) + vBottomLeft.m_SSCoord.x + 0.5);
			//int xRight = (int)(dDxDyRight * (y - bottomY) + vBottomRight.m_SSCoord.x + 0.5);
			//vLeft.m_SSCoord = Vec4(xLeft, y, 0, 0);
			//vRight.m_SSCoord = Vec4(xRight, y, 0, 0);
			
			DrawScanLine(vLeft, vRight, y);
		}
		if (y < 0 || y >= WinHeight)
		{
			break;
		}
	}
}

// normal triangle
void Render::DrawSolidTriangle(Vertex v0, Vertex v1, Vertex v2)
{
	// if is planetop or planebottom
	if (v0.m_SSCoord.y == v1.m_SSCoord.y)
	{
		// planetop
		if (v0.m_SSCoord.y > v2.m_SSCoord.y)
		{
			if (v0.m_SSCoord.x < v1.m_SSCoord.x)
			{
				DrawTrianglePlaneTop(v0, v1, v2);
			}
			else if (v0.m_SSCoord.x > v1.m_SSCoord.x)
			{
				DrawTrianglePlaneTop(v1, v0, v2);
			}
			else
			{
				// v0 and v1 are same
			}
		}
		// planebottom
		else if (v0.m_SSCoord.y < v2.m_SSCoord.y)
		{
			if (v0.m_SSCoord.x < v1.m_SSCoord.x)
			{
				DrawTrianglePlaneBottom(v2, v1, v0);
			}
			else if (v0.m_SSCoord.x > v1.m_SSCoord.x)
			{
				DrawTrianglePlaneBottom(v2, v0, v1);
			}
			else
			{
				// v0 and v1 are same
			}
		}
		else
		{
			// v0 v1 v2 is a line
		}
	}
	else if (v0.m_SSCoord.y == v2.m_SSCoord.y)
	{
		// planetop
		if (v0.m_SSCoord.y > v1.m_SSCoord.y)
		{
			if (v0.m_SSCoord.x < v2.m_SSCoord.x)
			{
				DrawTrianglePlaneTop(v0, v2, v1);
			}
			else if (v0.m_SSCoord.x > v2.m_SSCoord.x)
			{
				DrawTrianglePlaneTop(v2, v0, v1);
			}
			else
			{
				// v0 and v1 are same
			}
		}
		// planebottom
		else if (v0.m_SSCoord.y < v1.m_SSCoord.y)
		{
			if (v0.m_SSCoord.x < v2.m_SSCoord.x)
			{
				DrawTrianglePlaneBottom(v1, v2, v0);
			}
			else if (v0.m_SSCoord.x > v2.m_SSCoord.x)
			{
				DrawTrianglePlaneBottom(v1, v0, v2);
			}
			else
			{
				// v0 and v1 are same
			}
		}
		else
		{
			// v0 v1 v2 is a line
		}
	}
	else if (v1.m_SSCoord.y == v2.m_SSCoord.y)
	{
		// planetop
		if (v1.m_SSCoord.y > v0.m_SSCoord.y)
		{
			if (v1.m_SSCoord.x < v2.m_SSCoord.x)
			{
				DrawTrianglePlaneTop(v1, v2, v0);
			}
			else if (v1.m_SSCoord.x > v2.m_SSCoord.x)
			{
				DrawTrianglePlaneTop(v2, v1, v0);
			}
			else
			{
				// v0 and v1 are same
			}
		}
		// planebottom
		else if (v1.m_SSCoord.y < v0.m_SSCoord.y)
		{
			if (v1.m_SSCoord.x < v2.m_SSCoord.x)
			{
				DrawTrianglePlaneBottom(v0, v2, v1);
			}
			else if (v1.m_SSCoord.x > v2.m_SSCoord.x)
			{
				DrawTrianglePlaneBottom(v0, v1, v2);
			}
			else
			{
				// v0 and v1 are same
			}
		}
		else
		{
			// v0 v1 v2 is a line
		}
	}
	// normal triangle
	else
	{
		Vertex vTop, vMiddle, vBottom;
		if (v0.m_SSCoord.y < v1.m_SSCoord.y && v0.m_SSCoord.y < v2.m_SSCoord.y)
		{
			vBottom = v0;
			if (v1.m_SSCoord.y < v2.m_SSCoord.y)
			{
				vMiddle = v1;
				vTop = v2;
			}
			else
			{
				vMiddle = v2;
				vTop = v1;
			}
		}
		else if (v1.m_SSCoord.y < v0.m_SSCoord.y && v1.m_SSCoord.y < v2.m_SSCoord.y)
		{
			vBottom = v1;
			if (v0.m_SSCoord.y < v2.m_SSCoord.y)
			{
				vMiddle = v0;
				vTop = v2;
			}
			else
			{
				vMiddle = v2;
				vTop = v0;
			}
		}
		else if (v2.m_SSCoord.y < v0.m_SSCoord.y && v2.m_SSCoord.y < v1.m_SSCoord.y)
		{
			vBottom = v2;
			if (v0.m_SSCoord.y < v1.m_SSCoord.y)
			{
				vMiddle = v0;
				vTop = v1;
			}
			else
			{
				vMiddle = v1;
				vTop = v0;
			}
		}
		else
		{
			// cannot reach this branch
		}

		// find the new middle vertex
		float t = (vMiddle.m_SSCoord.y - vBottom.m_SSCoord.y) / (vTop.m_SSCoord.y - vBottom.m_SSCoord.y);
		Vertex newMiddle = LerpVertex(vBottom, vTop, t);
		if (vMiddle.m_SSCoord.x < newMiddle.m_SSCoord.x)
		{
			DrawTrianglePlaneBottom(vTop, newMiddle, vMiddle);
			DrawTrianglePlaneTop(vMiddle, newMiddle, vBottom);
		}
		else if (vMiddle.m_SSCoord.x > newMiddle.m_SSCoord.x)
		{
			DrawTrianglePlaneBottom(vTop, vMiddle, newMiddle);
			DrawTrianglePlaneTop(newMiddle, vMiddle, vBottom);
		}
		else
		{
			// is a line
		}
	}
}

void Render::DrawFrameTriangle(Vertex v0, Vertex v1, Vertex v2)
{
	DrawLine((int)v0.m_SSCoord.x, (int)v0.m_SSCoord.y, (int)v1.m_SSCoord.x, (int)v1.m_SSCoord.y, ColorBlack);
	DrawLine((int)v1.m_SSCoord.x, (int)v1.m_SSCoord.y, (int)v2.m_SSCoord.x, (int)v2.m_SSCoord.y, ColorBlack);
	DrawLine((int)v2.m_SSCoord.x, (int)v2.m_SSCoord.y, (int)v0.m_SSCoord.x, (int)v0.m_SSCoord.y, ColorBlack);
}

int Render::DrawTriangle(Vertex v0, Vertex v1, Vertex v2)
{
	// Vertex should has screen space coordinate
	// now assume (int)Vertex.m_SSCoord.xy is the screen space coordinate
	if (m_eRenderMode == RenderMode::RM_SOLID)
	{

		DrawSolidTriangle(v0, v1, v2);
		return 0;
	}
	else if (m_eRenderMode == RenderMode::RM_WIREFRAME)
	{

		DrawFrameTriangle(v0, v1, v2);
		return 0;
	}
	else
	{
		return -1;
	}
	return 0;
}

// world matrix is identity matrix, which means model doesn't move
Mat4 Render::ModeltoWorldMatrix()
{
	Mat4 worldmatrix = Mat4(); // identity matrix
	return worldmatrix;
}

Mat4 Render::WorldtoModelMatrix()
{
	Mat4 inverseworldmatrix = Mat4();
	return inverseworldmatrix;
}

Mat4 Render::WorldtoViewMatrix()
{
	Mat4 viewmatrix = Mat4();
	return viewmatrix;
}

void Render::RenderPipeline()
{
	// 1. Model to World
	// 2. World to View
	// 3. NDC
	// 4. NDC Clipping
	// 5. Projection
	// 6. Screen Space
}
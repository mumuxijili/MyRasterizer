#pragma once
#include "Math3D.h"
#include "Util.h"
#include "Mesh.h"

// global vertex buffer
extern vector<Vertex> g_vVertexBuffer;
// global index buffer
extern vector<UINT> g_vIndexBuffer;


int DrawPixel(int x, int y, DWORD color);
int DrawLine(int x0, int y0, int x1, int y1, DWORD color);
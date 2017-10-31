#pragma once
#include "Math3D.h"
#include "Util.h"
#include "Mesh.h"

// global vertex buffer
extern vector<Vertex> g_vVertexBuffer;
// global index buffer
extern vector<UINT> g_vIndexBuffer;
// global camera
extern Camera g_cam;


class Render
{
public:
	Render() {};
	~Render() {};

	void InitRender();

	void SetRenderMode(RenderMode rd) { m_eRenderMode = rd; }
	RenderMode GetRenderMode() { return m_eRenderMode; }
	int DrawPixel(int x, int y, DWORD color);
	// Line Rasterize, use screen space coordinate
	int DrawLine(int x0, int y0, int x1, int y1, DWORD color);
	void DrawScanLine(Vertex vLeft, Vertex vRight, int y);
	void DrawTrianglePlaneTop(Vertex vTopLeft, Vertex vTopRight, Vertex vBottom);
	void DrawTrianglePlaneBottom(Vertex vTop, Vertex vBottomRight, Vertex vBottomLeft);
	void DrawSolidTriangle(Vertex v0, Vertex v1, Vertex v2);
	void DrawFrameTriangle(Vertex v0, Vertex v1, Vertex v2);

	int DrawTriangle(Vertex v0, Vertex v1, Vertex v2);

	Mat4 ModeltoWorldMatrix();
	Mat4 WorldtoModelMatrix();
	Mat4 WorldtoViewMatrix(Camera cam);
	//Mat4 ViewtoWorldMatrix();
	Mat4 PerspectiveProjectionMatrix(Camera cam);
	//Mat4 OrthoProjectionMatrix();
	Mat4 PerspectiveToScreenMatrix();

	void RenderPipeline();

private:
	// render mode(wireframe or solid)
	RenderMode m_eRenderMode;
};


#pragma once
#include "Math3D.h"
#include "Util.h"

class Vertex
{
public:
	Vertex() {};
	~Vertex() {};

	Vec4 m_vertexPos;
	Vec4 m_vertexNorm;
	DWORD m_vertexColor;

	Vec4 m_SSCoord; // Screen Space Coordinate, with depth
};

class Triangle
{
public:
	Triangle() {};
	~Triangle() {};

	Vertex m_v0, m_v1, m_v2;
	//Vec4 m_n0, m_n1, m_n2;
	//Vec4 m_c0, m_c1, m_c2;

};


class Mesh
{
public:
	Mesh() {};
	virtual ~Mesh() {};

	vector<Vec4> m_vMeshPoses;
	vector<Vec4> m_vMeshNorms;
	vector<DWORD> m_vMeshColors;
	vector<UINT> m_vMeshIndices;

	void resizeVertices(int vertexCount);
	void resizeIndices(int IDtriCount) {
		m_vMeshIndices.resize(IDtriCount);

	}
	void clearAll() {
		resizeVertices(0);
		resizeIndices(0);
	}
	Vertex getVertex(int vertIndex);
	void setVertex(int vertIndex, const Vertex &v);
	void pushBackVertex(const Vertex &v);
};

Vertex LerpVertex(Vertex vStart, Vertex vEnd, float t);
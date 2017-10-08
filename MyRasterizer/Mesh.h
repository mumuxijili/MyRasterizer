#pragma once
#include "Math3D.h"

class Vertex
{
public:
	Vertex() {};
	~Vertex() {};

	Vec4 m_vertexPos;
	Vec4 m_vertexNorm;
	Vec4 m_vertexColor;
};


class Mesh
{
public:
	Mesh() {};
	virtual ~Mesh() {};

	vector<Vec4> m_vMeshPoses;
	vector<Vec4> m_vMeshNorms;
	vector<Vec4> m_vMeshColors;
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
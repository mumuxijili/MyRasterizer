#include "stdafx.h"
#include "Mesh.h"

void Mesh::resizeVertices(int vertexCount)
{
	m_vMeshPoses.resize(vertexCount);
	m_vMeshNorms.resize(vertexCount);
	m_vMeshColors.resize(vertexCount);
}

Vertex Mesh::getVertex(int vertIndex)
{
	assert(vertIndex < (int)m_vMeshPoses.size());

	Vertex v;
	v.m_vertexPos = m_vMeshPoses[vertIndex];
	v.m_vertexNorm = m_vMeshNorms[vertIndex];
	v.m_vertexColor = m_vMeshColors[vertIndex];
	return v;
}
void Mesh::setVertex(int vertIndex, const Vertex &v)
{
	assert(vertIndex < (int)m_vMeshPoses.size());

	m_vMeshPoses[vertIndex] = v.m_vertexPos;
	m_vMeshNorms[vertIndex] = v.m_vertexNorm;
	m_vMeshColors[vertIndex] = v.m_vertexColor;
}

void Mesh::pushBackVertex(const Vertex &v)
{
	m_vMeshPoses.push_back(v.m_vertexPos);
	m_vMeshNorms.push_back(v.m_vertexNorm);
	m_vMeshColors.push_back(v.m_vertexColor);
}
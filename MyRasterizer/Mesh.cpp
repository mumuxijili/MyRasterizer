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

Vertex LerpVertex(Vertex vStart, Vertex vEnd, float t)
{
	Vertex v;
	v.m_vertexPos = LerpVector(vStart.m_vertexPos, vEnd.m_vertexPos, t);
	v.m_SSCoord = LerpVector(vStart.m_SSCoord, vEnd.m_SSCoord, t);
	v.m_vertexNorm = LerpVector(vStart.m_vertexNorm, vEnd.m_vertexNorm, t);
	v.m_vertexColor = LerpColor(vStart.m_vertexColor, vEnd.m_vertexColor, t);
	
	return v;
}
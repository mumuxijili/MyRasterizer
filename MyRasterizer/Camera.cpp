#include "stdafx.h"
#include "Camera.h"

Camera::Camera(Vec4 pos, Vec4 dir, float fov, float fnear, float ffar)
{
	m_Pos = pos;
	m_Dir = dir;
	m_Fov = fov;
	m_Near = fnear;
	m_Far = ffar;
}
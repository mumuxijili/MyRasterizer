#include "stdafx.h"
#include "Math3D.h"

float Vec4::Dot(const Vec4 &v) const
{
	return x * v.x + y * v.y + z * v.z;
}

Vec4 Vec4::Cross(const Vec4 &v) const
{
	float m1 = y * v.z - z * v.y;
	float m2 = z * v.x - x * v.z;
	float m3 = x * v.y - y * v.x;
	return Vec4(m1, m2, m3, 0.0f);
}

// v1 = v * m
Vec4 Vec4::operator*(const Mat4 &mat) const
{
	float m0, m1, m2, m3;
	Vec4 c = mat.Col(0);
	m0 = x * c.x + y * c.y + z * c.z + w * c.w;
	c = mat.Col(1);
	m1 = x * c.x + y * c.y + z * c.z + w * c.w;
	c = mat.Col(2);
	m2 = x * c.x + y * c.y + z * c.z + w * c.w;
	c = mat.Col(3);
	m3 = x * c.x + y * c.y + z * c.z + w * c.w;
	return Vec4(m0, m1, m2, m3);
}

Vec4& Vec4::normalize()
{
	float length = norm();
	if (length > 0.0f)
	{
		x = x / length; y = y / length; z = z / length; w = w / length;
	}
	return *this;
}

bool Mat4::operator==(const Mat4 &mat) const {
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			if (abs(m[i][j] - mat.m[i][j]) >= 0.000001f)
				return false;
	return true;
}

// m1 = m2 + m3
Mat4 Mat4::operator+(const Mat4 &mat) const
{
	Mat4 m2;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			m2.m[i][j] = m[i][j] + mat.m[i][j];
	return m2;
}

// m1 = m2 - m3;
Mat4 Mat4::operator-(const Mat4 &mat) const
{
	Mat4 m2;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			m2.m[i][j] = m[i][j] - mat.m[i][j];
	return m2;
}

// m1 = m2 * m3
Mat4 Mat4::operator*(const Mat4 &mat) const
{
	Mat4 m2;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
		{
			m2.m[j][i] = (m[j][0] * mat.m[0][i]) +
				(m[j][1] * mat.m[1][i]) +
				(m[j][2] * mat.m[2][i]) +
				(m[j][3] * mat.m[3][i]);
		}
	return m2;
}

Mat4 Mat4::Transpose(const Mat4 &mat)
{
	return Mat4(mat.m[0][0], mat.m[1][0], mat.m[2][0], mat.m[3][0],
		mat.m[0][1], mat.m[1][1], mat.m[2][1], mat.m[3][1],
		mat.m[0][2], mat.m[1][2], mat.m[2][2], mat.m[3][2],
		mat.m[0][3], mat.m[1][3], mat.m[2][3], mat.m[3][3]);
}

void Mat4::SetZero()
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			m[i][j] = 0.0f;
}

Vec4 Mat4::Col(int c) const
{
	return Vec4(m[0][c], m[1][c], m[2][c], m[3][c]);
}

void Mat4::SetCol(int c, Vec4 col)
{
	m[0][c] = col.x;
	m[1][c] = col.y;
	m[2][c] = col.z;
	m[3][c] = col.w;
}

Vec4 Mat4::Row(int r) const
{
	return Vec4(m[r][0], m[r][1], m[r][2], m[r][3]);
}

void Mat4::SetRow(int r, Vec4 row)
{
	m[r][0] = row.x;
	m[r][1] = row.y;
	m[r][2] = row.z;
	m[r][3] = row.w;
}

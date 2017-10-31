#pragma once
#include <cmath>
#include <vector>
using namespace std;

/*
	4 element Vector, +, -, *(scalar, vector dot mul), +=, -=, *=, [], ==. !=
*/

class Mat4;

class Vec4
{
public:
	Vec4() { x = y = z = w = 0.0f; }

	Vec4(float xx, float yy, float zz, float ww) : x(xx), y(yy), z(zz), w(ww) {}

	Vec4(float xx, float yy, float zz) : x(xx), y(yy), z(zz) { w = 0.0f; }

	Vec4(const Vec4 &v) { x = v.x; y = v.y; z = v.z; w = v.w; }

	Vec4& operator=(const Vec4 &v)
	{
		x = v.x; y = v.y; z = v.z; w = v.w;
		return *this;
	}

	Vec4 operator+(const Vec4 &v) const
	{
		return Vec4(x + v.x, y + v.y, z + v.z, w + v.w);
	}

	Vec4 operator-(const Vec4 &v) const
	{
		return Vec4(x - v.x, y - v.y, z - v.z, w - v.w);
	}

	// scalar multiply
	Vec4 operator*(float s) const
	{
		return Vec4(x * s, y * s, z * s, w * s);
	}

	// per element multiply
	Vec4 operator*(const Vec4 &v) const
	{
		return Vec4(x * v.x, y * v.y, z * v.z, w * v.w);
	}
	// matrix multiply
	Vec4 operator*(const Mat4 &mat) const;
	//{
	//	return Vec4(x*mat.m[0][0] + y*mat.m[1][0] + z*mat.m[2][0] + w*mat.m[3][0],
	//		x*mat.m[0][1] + y*mat.m[1][1] + z*mat.m[2][1] + w*mat.m[3][1],
	//		x*mat.m[0][2] + y*mat.m[1][2] + z*mat.m[2][2] + w*mat.m[3][2],
	//		x*mat.m[0][3] + y*mat.m[1][3] + z*mat.m[2][3] + w*mat.m[3][3]);
	//	Vec4 v = this->operator*(mat.Col(0));
	//	return Vec4()
	//}

	// dot multiply func
	float Dot(const Vec4 &v) const;
	// cross multiply func
	Vec4 Cross(const Vec4 &v) const;

	Vec4& operator+=(const Vec4 &v)
	{
		x += v.x; y += v.y; z += v.z; w += v.w;
		return *this;
	}

	Vec4& operator-=(const Vec4 &v)
	{
		x -= v.x; y -= v.y; z -= v.z; w -= v.w;
		return *this;
	}

	// scalar multiply
	Vec4& operator*=(float s)
	{
		x *= s; y *= s; z *= s;
		return *this;
	}

	float operator[](int i) const
	{
		return (&x)[i];
	}

	float& operator[](int i)
	{
		return (&x)[i];
	}

	bool operator==(const Vec4 &v) const
	{
		return x == v.x && y == v.y && z == v.z && w == v.w;
	}

	bool operator!=(const Vec4 &v) const
	{
		return x != v.x || y != v.y || z != v.z || w != v.w;
	}

	// square of length of vector4
	float norm2() const
	{
		return x * x + y * y + z * z + w * w;
	}

	// length of vector4
	float norm() const
	{
		return sqrtf(norm2());
	}

	Vec4& normalize();

	float x, y, z, w;
};

/*class Point
{
public:
	Point() { x = y = z = 0.0f; w = 1.0f; }

	Point(float xx, float yy, float zz) : x(xx), y(yy), z(zz) { w = 1.0f; }

	Point(float xx, float yy, float zz, float ww) : x(xx), y(yy), z(zz), w(ww) {}

	Point(const Point &p) { x = p.x; y = p.y; z = p.z; w = p.w; }

	Point& operator=(const Point &p)
	{
		x = p.x; y = p.y; z = p.z; w = p.w;
		return *this;
	}

	Point operator+(const Point &p) const
	{
		return Point(x + p.x, y + p.y, z + p.z, w + p.w);
	}

	Point operator+(const Vec4 &v) const
	{
		return Point(x + v.x, y + v.y, z + v.z, w + v.w);
	}

	Vec4 operator-(const Point &p) const
	{
		return Vec4(x - p.x, y - p.y, z - p.z, w - p.w);
	}

	Point operator-(const Vec4 &v) const
	{
		return Point(x - v.x, y - v.y, z - v.z, w - v.w);
	}

	Point operator*(float s) const
	{
		return Point(x * s, y * s, z * s, w * s);
	}

	Point& operator+=(const Point &p)
	{
		x += p.x; y += p.y; z += p.z; w += p.w;
		return *this;
	}

	Point& operator+=(const Vec4 &v)
	{
		x += v.x; y += v.y; z += v.z; w += v.w;
		return *this;
	}

	Point& operator-=(const Point &p)
	{
		x -= p.x; y -= p.y; z -= p.z; w -= p.w;
		return *this;
	}

	Point& operator-=(const Vec4 &v)
	{
		x -= v.x; y -= v.y; z -= v.z; w -= v.w;
		return *this;
	}

	Point& operator*=(float s)
	{
		x *= s; y *= s; z *= s;
		return *this;
	}

	float operator[](int i) const
	{
		return (&x)[i];
	}

	float& operator[](int i)
	{
		return (&x)[i];
	}

	bool operator==(const Point &p) const
	{
		return x == p.x && y == p.y && z == p.z && w == p.w;
	}

	bool operator!=(const Point &p) const
	{
		return x != p.x || y != p.y || z != p.z || w != p.w;
	}
	float x, y, z, w;
};*/

class Mat4
{
public:
	Mat4()
	{
		m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.f;
		m[0][1] = m[0][2] = m[0][3] = m[1][0] = m[1][2] = m[1][3] = m[2][0] = m[2][1] = m[2][3] = m[3][0] = m[3][1] = m[3][2] = 0.f;
	}

	Mat4(float mat[4][4])
	{
		memcpy(m, mat, 16 * sizeof(float));
	}

	Mat4(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33) {
		m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
		m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
		m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
		m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
	}

	Mat4& operator=(const Mat4 &mat)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				m[i][j] = mat.m[i][j];
			}
		}
		return *this;
	}

	bool operator==(const Mat4 &mat) const;

	Mat4 operator+(const Mat4 &mat) const;
	Mat4 operator-(const Mat4 &mat) const;
	Mat4 operator*(const Mat4 &mat) const;

	Mat4 Transpose(const Mat4 &mat);
	void SetZero();
	Vec4 Col(int c) const;
	void SetCol(int c, Vec4 col);
	Vec4 Row(int r) const;
	void SetRow(int r, Vec4 row);
private:
	float m[4][4];
};
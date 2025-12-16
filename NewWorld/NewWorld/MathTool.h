#pragma once
#define _USE_MATH_DEFINES
#include <iostream>

#include "GamesEngineeringBase.h"
#define kuan 1920.f
#define gao 1080.f
#include <cmath>
#include <vector>
#undef min
#undef max
#include <algorithm>
using namespace std;
template<typename T>
static T clamp(const T value, const T minValue, const T maxValue)
{
	return std::max(std::min(value, maxValue), minValue);
}
namespace MathTool
{

	template<typename t>
	t simpleInterpolateAttribute(t a0, t a1, t a2, float alpha, float beta, float gamma) {
		return (a0 * alpha) + (a1 * beta) + (a2 * gamma);
	}
	template<typename t>
	t perspectiveCorrectInterpolateAttribute(t a0, t a1, t a2, float v0_w, float v1_w, float v2_w,
		float alpha, float beta, float gamma, float frag_w)
	{
		t attrib[3];
		attrib[0] = a0 * alpha * v0_w;
		attrib[1] = a1 * beta * v1_w;
		attrib[2] = a2 * gamma * v2_w;
		return ((attrib[0] + attrib[1] + attrib[2]) / frag_w);
	}

	class Vec3
	{
	public:
		union
		{
			float v[3];
			struct { float x, y, z; };
		};
		Vec3(float x, float y, float z) {

			v[0] = x;
			v[1] = y;
			v[2] = z;
		};
		Vec3() {
			v[0] = 0;
			v[1] = 0;
			v[2] = 0;
		}

		Vec3& operator+=(const Vec3& pVec)
		{
			v[0] += pVec.v[0];
			v[1] += pVec.v[1];
			v[2] += pVec.v[2];
			return *this;
		}
		Vec3& operator+=(const float val)
		{
			v[0] += val;
			v[1] += val;
			v[2] += val;
			return *this;
		}
		Vec3 operator+(const Vec3& pVec) const
		{
			return Vec3(v[0] + pVec.v[0], v[1] + pVec.v[1], v[2] + pVec.v[2]);
		}
		Vec3 operator+(const float val) const
		{
			return Vec3(v[0] + val, v[1] + val, v[2] + val);
		}

		Vec3 operator*(const Vec3& pVec) const
		{
			return Vec3(v[0] * pVec.v[0], v[1] * pVec.v[1], v[2] * pVec.v[2]);
		}
		Vec3 operator*(const float val) const
		{
			return Vec3(v[0] * val, v[1] * val, v[2] * val);
		}
		Vec3 operator-() const
		{
			return Vec3(-v[0], -v[1], -v[2]);
		}
		Vec3 operator-(const Vec3& pVec) const
		{
			return Vec3(v[0] - pVec.v[0], v[1] - pVec.v[1], v[2] - pVec.v[2]);
		}
		Vec3 operator-(const float val) const
		{
			return Vec3(v[0] - val, v[1] - val, v[2] - val);
		}
		Vec3 operator/(const float val) const
		{
			return Vec3(v[0] / val, v[1] / val, v[2] / val);
		}
		Vec3 operator/(const Vec3& pVec) const
		{
			return Vec3(v[0] / pVec.v[0], v[1] / pVec.v[1], v[2] / pVec.v[2]);
		}

		Vec3& operator-=(const Vec3 val)
		{
			v[0] -= val.x;
			v[1] -= val.y;
			v[2] -= val.z;
			return *this;
		}
		Vec3& operator-=(const float val)
		{
			v[0] -= val;
			v[1] -= val;
			v[2] -= val;
			return *this;
		}
		Vec3& operator/=(const float val)
		{
			v[0] /= val;
			v[1] /= val;
			v[2] /= val;
			return *this;
		}
		Vec3& operator/=(const Vec3& pVec)
		{
			v[0] /= pVec.v[0];
			v[1] /= pVec.v[1];
			v[2] /= pVec.v[2];
			return *this;
		}
		Vec3& operator*=(const float val)
		{
			v[0] *= val;
			v[1] *= val;
			v[2] *= val;
			return *this;
		}
		Vec3& operator*=(const Vec3& pVec)
		{
			v[0] *= pVec.v[0];
			v[1] *= pVec.v[1];
			v[2] *= pVec.v[2];
			return *this;
		}
		float length() const
		{
			return sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
		}
		float lengthSquared() const
		{
			return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
		}
		Vec3 normalize(void)
		{
			float len = 1.0f / sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
			return Vec3(x * len, y * len, z * len);
		}
		float normalize_GetLength()
		{
			float length = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
			float len = 1.0f / length;
			v[0] *= len; v[1] *= len; v[2] *= len;
			return length;
		}
		float Dot(const Vec3& pVec) const
		{
			return v[0] * pVec.v[0] + v[1] * pVec.v[1] + v[2] * pVec.v[2];
		}
		Vec3 Cross(const Vec3& v1)
		{
			return Vec3(v1.v[1] * v[2] - v1.v[2] * v[1], v1.v[2] * v[0] - v1.v[0] * v[2], v1.v[0] * v[1] - v1.v[1] * v[0]);
		}
		float Max() const
		{
			return max(x, max(y, z));
		}
		float Mix() const
		{
			return min(x, min(y, z));
		}


	};
	class Vec4 {
	public:
		union {
			float v[4];
			struct { float x, y, z, w; };
		};

		Vec4() {
			v[0] = 0;
			v[1] = 0;
			v[2] = 0;
			v[3] = 0;
		}
		Vec4(float x, float y, float z, float w) {
			v[0] = x;
			v[1] = y;
			v[2] = z;
			v[3] = w;
		}

		void DivideByW() {
			if (w != 0.0f) {
				x /= w;
				y /= w;
				z /= w;
				w = 1.0f / w;
			}

		}
		
		Vec4& operator+=(const Vec4& pVec)
		{
			v[0] += pVec.v[0];
			v[1] += pVec.v[1];
			v[2] += pVec.v[2];
			v[3] += pVec.v[3];
			return *this;
		}
		Vec4& operator+=(const float val)
		{
			v[0] += val;
			v[1] += val;
			v[2] += val;
			v[3] += val;
			return *this;
		}
		Vec4 operator+(const Vec4& pVec) const
		{
			return Vec4(v[0] + pVec.v[0], v[1] + pVec.v[1], v[2] + pVec.v[2], v[3] + pVec.v[3]);
		}
		Vec4 operator+(const float val) const
		{
			return Vec4(v[0] + val, v[1] + val, v[2] + val, v[3] + val);
		}

		Vec4 operator*(const Vec4& pVec) const
		{
			return Vec4(v[0] * pVec.v[0], v[1] * pVec.v[1], v[2] * pVec.v[2], v[3] * pVec.v[3]);
		}
		Vec4 operator*(const float val) const
		{
			return Vec4(v[0] * val, v[1] * val, v[2] * val, v[3] * val);
		}
		Vec4 operator-() const
		{
			return Vec4(-v[0], -v[1], -v[2], -v[3]);
		}
		Vec4 operator-(const Vec4& pVec) const
		{
			return Vec4(v[0] - pVec.v[0], v[1] - pVec.v[1], v[2] - pVec.v[2], v[3] - pVec.v[3]);
		}
		Vec4 operator-(const float val) const
		{
			return Vec4(v[0] - val, v[1] - val, v[2] - val, v[3] - val);
		}
		Vec4 operator/(const float val) const
		{
			return Vec4(v[0] / val, v[1] / val, v[2] / val, v[3] / val);
		}
		Vec4 operator/(const Vec4& pVec) const
		{
			return Vec4(v[0] / pVec.v[0], v[1] / pVec.v[1], v[2] / pVec.v[2], v[3] / pVec.v[3]);
		}

		Vec4& operator-=(const Vec4 val)
		{
			v[0] -= val.x;
			v[1] -= val.y;
			v[2] -= val.z;
			v[3] -= val.w;
			return *this;
		}
		Vec4& operator-=(const float val)
		{
			v[0] -= val;
			v[1] -= val;
			v[2] -= val;
			v[3] -= val;
			return *this;
		}
		Vec4& operator/=(const float val)
		{
			v[0] /= val;
			v[1] /= val;
			v[2] /= val;
			v[3] /= val;
			return *this;
		}
		Vec4& operator/=(const Vec4& pVec)
		{
			v[0] /= pVec.v[0];
			v[1] /= pVec.v[1];
			v[2] /= pVec.v[2];
			v[3] /= pVec.v[3];
			return *this;
		}
		Vec4& operator*=(const float val)
		{
			v[0] *= val;
			v[1] *= val;
			v[2] *= val;
			v[3] *= val;
			return *this;
		}
		Vec4& operator*=(const Vec4& pVec)
		{
			v[0] *= pVec.v[0];
			v[1] *= pVec.v[1];
			v[2] *= pVec.v[2];
			v[3] *= pVec.v[3];
			return *this;
		}
		float length() const
		{
			return sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3]);
		}
		float lengthSquared() const
		{
			return v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3];
		}
		Vec4 normalize(void)
		{
			float len = 1.0f / sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3]);
			return Vec4(x * len, y * len, z * len, w * len);
		}
		float normalize_GetLength()
		{
			float length = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3]);
			float len = 1.0f / length;
			v[0] *= len; v[1] *= len; v[2] *= len; v[3] *= len;
			return length;
		}
		float Dot(const Vec4& pVec) const
		{
			return v[0] * pVec.v[0] + v[1] * pVec.v[1] + v[2] * pVec.v[2] + v[3] * pVec.v[3];
		}
		Vec3 TransToVec3()
		{
			return Vec3(v[0] / w, v[1] / w, v[2] / w);
		}
		Vec3 TransToVec3RemoveW() {
			return Vec3(v[0], v[1], v[2]);
		}
		Vec4 Cross(const Vec4& v1) const
		{
			return Vec4(
				v[1] * v1.v[2] - v[2] * v1.v[1],
				v[2] * v1.v[0] - v[0] * v1.v[2],
				v[0] * v1.v[1] - v[1] * v1.v[0],
				0.0f
			);
		}
		float Max() const
		{
			return max(x, max(y, max(z, w)));
		}
		float Mix() const
		{
			return min(x, min(y, min(y, z)));
		}

	};
	class Triangle {
	public:
		union {
			Vec4 v[3];
			struct {
				Vec4 v0;
				Vec4 v1;
				Vec4 v2;
			};
		};
		vector<int> vertexindex;
		Vec3 normal;

		Triangle(const Vec4& vertex0, const Vec4& vertex1, const Vec4& vertex2)
			: v0(vertex0), v1(vertex1), v2(vertex2) {
		}




	};
	inline float edgeFunction(const Vec4& v0, const Vec4& v1, const Vec4& p)
	{
		return (((p.x - v0.x) * (v1.y - v0.y)) - ((v1.x - v0.x) * (p.y - v0.y)));
	}


	inline float bary(const Vec4& v0, const Vec4& v1, const Vec4& v2)
	{
		return (v1.x - v0.x) * (v2.y - v0.y) -
			(v1.y - v0.y) * (v2.x - v0.x);
	}


	inline Vec3 Max(const Vec3& v1, const Vec3& v2) {
		return Vec3(max(v1.v[0], v2.v[0]), max(v1.v[1], v2.v[1]), max(v1.v[2], v2.v[2]));
	}
	inline Vec3 Min(const Vec3& v1, const Vec3& v2) {
		return Vec3(min(v1.v[0], v2.v[0]), min(v1.v[1], v2.v[1]), min(v1.v[2], v2.v[2]));
	}
	inline float Dot(const Vec3& v1, const Vec3& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}
	inline std::ostream& operator<<(std::ostream& os, const Vec3& vec)
	{
		os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
		return os;
	}
	class Colour
	{
	public:
		float r, g, b, a;

		// --- Constructors ---
		Colour() : r(0), g(0), b(0), a(1) {}   // default: opaque black
		Colour(float r, float g, float b, float a = 1.0f)
			: r(r), g(g), b(b), a(a) {
		}

		// --- Basic operations ---
		Colour operator+(const Colour& c) const {
			return Colour(r + c.r, g + c.g, b + c.b, a + c.a);
		}

		Colour operator-(const Colour& c) const {
			return Colour(r - c.r, g - c.g, b - c.b, a - c.a);
		}

		Colour operator*(const Colour& c) const {
			return Colour(r * c.r, g * c.g, b * c.b, a * c.a);
		}

		Colour operator*(float s) const {
			return Colour(r * s, g * s, b * s, a * s);
		}
		Colour operator/(float scalar) const
		{
			if (scalar == 0.0f)
				return *this;

			return Colour(
				r / scalar,
				g / scalar,
				b / scalar,
				a / scalar
			);
		}
		Colour& operator+=(const Colour& c) {
			r += c.r; g += c.g; b += c.b; a += c.a; return *this;
		}

		Colour& operator-=(const Colour& c) {
			r -= c.r; g -= c.g; b -= c.b; a -= c.a; return *this;
		}

		Colour& operator*=(float s) {
			r *= s; g *= s; b *= s; a *= s; return *this;
		}

		// --- Premultiply alpha ---
		Colour premultiplied() const {
			return Colour(r * a, g * a, b * a, a);
		}

		// --- Linear interpolation ---
		static Colour Lerp(const Colour& c1, const Colour& c2, float t) {
			return Colour(
				c1.r + (c2.r - c1.r) * t,
				c1.g + (c2.g - c1.g) * t,
				c1.b + (c2.b - c1.b) * t,
				c1.a + (c2.a - c1.a) * t
			);
		}

		// --- Print (for cout) ---
		friend std::ostream& operator<<(std::ostream& os, const Colour& c) {
			os << "Colour(" << c.r << ", " << c.g << ", " << c.b << ", " << c.a << ")";
			return os;
		}
	};
	inline Vec3 Vec4ToVec3(const Vec4& v4) {
		return Vec3(v4.x, v4.y, v4.z);
	}

	class Matrix
	{
	public:
		union
		{
			float a[4][4];
			float m[16];
		};

		Matrix(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33
		)
		{
			m[0] = m00;  m[1] = m01;  m[2] = m02;  m[3] = m03;
			m[4] = m10;  m[5] = m11;  m[6] = m12;  m[7] = m13;
			m[8] = m20;  m[9] = m21;  m[10] = m22;  m[11] = m23;
			m[12] = m30;  m[13] = m31;  m[14] = m32;  m[15] = m33;
		}

		Matrix() {
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
				{

					a[i][j] = 0.0f;
					if (i == j)
						a[i][j] = 1.0f;
				}
		}
		static Matrix lookAtMatrix(Vec3 from, Vec3 to, Vec3 up) {
			Matrix lookat;
			memset(lookat.m, 0, 16 * sizeof(float));
			Vec3 dir = (to - from) / (to - from).length();
			Vec3 right = up.Cross(dir);//only cross x,y,z. 
			Vec3 realUp = dir.Cross(right);//real up vector

			lookat.a[0][0] = right.x;  lookat.a[0][1] = right.y;  lookat.a[0][2] = right.z;  lookat.a[0][3] = -(from.Dot(right));
			lookat.a[1][0] = realUp.x; lookat.a[1][1] = realUp.y; lookat.a[1][2] = realUp.z; lookat.a[1][3] = -(from.Dot(realUp));
			lookat.a[2][0] = dir.x;    lookat.a[2][1] = dir.y;    lookat.a[2][2] = dir.z;    lookat.a[2][3] = -(from.Dot(dir));
			lookat.a[3][3] = 1;
			return lookat;
		}
		static Matrix projectionMatrix(float fov, float aspect, float _near, float _far) {
			Matrix proM;
			memset(proM.m, 0, 16 * sizeof(float));
			proM.a[0][0] = 1 / (aspect * (tan(fov / 2)));//no pi
			proM.a[1][1] = 1 / (tan(fov / 2));
			proM.a[2][2] = _far / (_far - _near);
			proM.a[2][3] = -(_far * _near) / (_far - _near);
			proM.a[3][2] = 1;
			return proM;
		}
		Matrix invert()
		{
			Matrix inv;
			inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
			inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
			inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
			inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
			inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
			inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
			inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
			inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
			inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
			inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
			inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
			inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
			inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
			inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
			inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
			inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];
			float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
			if (det == 0) {
				// Handle this case
			}
			det = 1.0 / det;
			for (int i = 0; i < 16; i++) {
				inv[i] = inv[i] * det;
			}
			return inv;
		}

		Matrix Perspective(float fov, float aspect, float zNear, float zFar) {
			Matrix mat;
			float f = 1.0f / tan(fov / 2.0f);

			mat.a[0][0] = f / aspect;
			mat.a[0][1] = 0.0f;
			mat.a[0][2] = 0.0f;
			mat.a[0][3] = 0.0f;

			mat.a[1][0] = 0.0f;
			mat.a[1][1] = f;
			mat.a[1][2] = 0.0f;
			mat.a[1][3] = 0.0f;

			mat.a[2][0] = 0.0f;
			mat.a[2][1] = 0.0f;
			mat.a[2][2] = zFar / (zFar - zNear);
			mat.a[2][3] = -(zFar * zNear) / (zFar - zNear);

			mat.a[3][0] = 0.0f;
			mat.a[3][1] = 0.0f;
			mat.a[3][2] = 1.0f;
			mat.a[3][3] = 0.0f;

			return mat;
		}
		static Matrix perspective(const float n, const float f, float aspect, const float fov) // FOV in degrees, outputs transposed Matrix for DX
		{
			Matrix pers;
			memset(pers.m, 0, sizeof(float) * 16);
			float t = 1.0f / (tanf(fov * 0.5f * 3.141592654f / 180.0f));
			pers.a[0][0] = t / aspect;
			pers.a[1][1] = t;
			pers.a[2][2] = f / (f - n);
			pers.a[2][3] = -(f * n) / (f - n);
			pers.a[3][2] = 1.0f;
			return pers;
		}
		Vec4 mul(const Vec4& v)
		{
			return Vec4(v.x * m[0] + v.y * m[1] + v.z * m[2] + v.w * m[3], v.x * m[4] + v.y * m[5] + v.z * m[6] + v.w * m[7], v.x * m[8] + v.y * m[9] + v.z * m[10] + v.w * m[11], v.x * m[12] + v.y * m[13] + v.z * m[14] + v.w * m[15]);
		}
		Matrix mul(const Matrix& matrix) const
		{
			Matrix ret;
			ret.m[0] = m[0] * matrix.m[0] + m[1] * matrix.m[4] + m[2] * matrix.m[8] + m[3] * matrix.m[12];
			ret.m[1] = m[0] * matrix.m[1] + m[1] * matrix.m[5] + m[2] * matrix.m[9] + m[3] * matrix.m[13];
			ret.m[2] = m[0] * matrix.m[2] + m[1] * matrix.m[6] + m[2] * matrix.m[10] + m[3] * matrix.m[14];
			ret.m[3] = m[0] * matrix.m[3] + m[1] * matrix.m[7] + m[2] * matrix.m[11] + m[3] * matrix.m[15];
			ret.m[4] = m[4] * matrix.m[0] + m[5] * matrix.m[4] + m[6] * matrix.m[8] + m[7] * matrix.m[12];
			ret.m[5] = m[4] * matrix.m[1] + m[5] * matrix.m[5] + m[6] * matrix.m[9] + m[7] * matrix.m[13];
			ret.m[6] = m[4] * matrix.m[2] + m[5] * matrix.m[6] + m[6] * matrix.m[10] + m[7] * matrix.m[14];
			ret.m[7] = m[4] * matrix.m[3] + m[5] * matrix.m[7] + m[6] * matrix.m[11] + m[7] * matrix.m[15];
			ret.m[8] = m[8] * matrix.m[0] + m[9] * matrix.m[4] + m[10] * matrix.m[8] + m[11] * matrix.m[12];
			ret.m[9] = m[8] * matrix.m[1] + m[9] * matrix.m[5] + m[10] * matrix.m[9] + m[11] * matrix.m[13];
			ret.m[10] = m[8] * matrix.m[2] + m[9] * matrix.m[6] + m[10] * matrix.m[10] + m[11] * matrix.m[14];
			ret.m[11] = m[8] * matrix.m[3] + m[9] * matrix.m[7] + m[10] * matrix.m[11] + m[11] * matrix.m[15];
			ret.m[12] = m[12] * matrix.m[0] + m[13] * matrix.m[4] + m[14] * matrix.m[8] + m[15] * matrix.m[12];
			ret.m[13] = m[12] * matrix.m[1] + m[13] * matrix.m[5] + m[14] * matrix.m[9] + m[15] * matrix.m[13];
			ret.m[14] = m[12] * matrix.m[2] + m[13] * matrix.m[6] + m[14] * matrix.m[10] + m[15] * matrix.m[14];
			ret.m[15] = m[12] * matrix.m[3] + m[13] * matrix.m[7] + m[14] * matrix.m[11] + m[15] * matrix.m[15];
			return ret;
		}
		Matrix operator*(const Matrix& matrix)
		{
			return mul(matrix);
		}
		float& operator[](int index)
		{
			return m[index];
		}

		Matrix Multiply(const Matrix& rhs) const
		{
			Matrix result;

			for (int col = 0; col < 4; col++)
			{
				for (int row = 0; row < 4; row++)
				{
					result.m[col * 4 + row] =
						m[0 * 4 + row] * rhs.m[col * 4 + 0] +
						m[1 * 4 + row] * rhs.m[col * 4 + 1] +
						m[2 * 4 + row] * rhs.m[col * 4 + 2] +
						m[3 * 4 + row] * rhs.m[col * 4 + 3];
				}
			}

			return result;
		}
		Matrix Multiplyreference(const Matrix& rhs)
		{
			Matrix result;

			for (int col = 0; col < 4; col++)
			{
				for (int row = 0; row < 4; row++)
				{
					result.m[col * 4 + row] =
						m[0 * 4 + row] * rhs.m[col * 4 + 0] +
						m[1 * 4 + row] * rhs.m[col * 4 + 1] +
						m[2 * 4 + row] * rhs.m[col * 4 + 2] +
						m[3 * 4 + row] * rhs.m[col * 4 + 3];
				}
			}
			*this = result;
			return result;
		}

		Matrix rotationX(float angle) {
			float c = cosf(angle);
			float s = sinf(angle);
			return Multiplyreference(Matrix(
				1, 0, 0, 0,
				0, c, -s, 0,
				0, s, c, 0,
				0, 0, 0, 1
			));
		}
		Matrix rotationY(float angle) {
			float c = cosf(angle);
			float s = sinf(angle);
			return Multiplyreference(Matrix(
				c, 0, s, 0,
				0, 1, 0, 0,
				-s, 0, c, 0,
				0, 0, 0, 1
			));
		}
		Matrix rotationZ(float angle) {
			float c = cosf(angle);
			float s = sinf(angle);
			return Multiplyreference(Matrix(
				c, -s, 0, 0,
				s, c, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			));
		}
		Matrix translation(float x, float y, float z) {//执行计算
			return Multiplyreference(Matrix(
				1, 0, 0, x,
				0, 1, 0, y,
				0, 0, 1, z,
				0, 0, 0, 1
			));
		}
		static Matrix translation(const Vec3& v)//单独返回转换矩阵
		{
			Matrix mat;
			mat.a[0][3] = v.x;
			mat.a[1][3] = v.y;
			mat.a[2][3] = v.z;
			return mat;
		}
		Matrix  scale(float x, float y, float z) {//进行变化
			return Multiplyreference(Matrix(
				x, 0, 0, 0,
				0, y, 0, 0,
				0, 0, z, 0,
				0, 0, 0, 1
			));

		}
		static Matrix scaling(const Vec3& v)//单纯的变换矩阵
		{
			Matrix mat;
			mat.m[0] = v.x;
			mat.m[5] = v.y;
			mat.m[10] = v.z;
			return mat;
		}
		Matrix Transpose() {
			Matrix transposed;
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					transposed.a[i][j] = a[j][i];
				}
			}
			return transposed;
		}
		Vec3 mulPoint(const Vec3& v)
		{
			Vec3 v1 = Vec3(
				(v.x * m[0] + v.y * m[1] + v.z * m[2]) + m[3], (v.x * m[4] + v.y * m[5] + v.z * m[6]) + m[7], (v.x * m[8] + v.y * m[9] + v.z * m[10]) + m[11]);
		}
		Vec3 mulVec(const Vec3& v)
		{
			return Vec3(
				(v.x * m[0] + v.y * m[1] + v.z * m[2]), (v.x * m[4] + v.y * m[5] + v.z * m[6]), (v.x * m[8] + v.y * m[9] + v.z * m[10]));
		}
		void setIdentity() {
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
				{
					a[i][j] = 0.0f;
					if (i == j)
						a[i][j] = 1.0f;
				}
		}
		void setZero() {
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
				{
					a[i][j] = 0.0f;
				}
		}
		void settarget(int x, int y, float val) {
			a[x][y] = val;
		}
		Matrix LookatMatrix(Vec4& from, Vec4& to, Vec4& up) {
			Vec4 dir = (to - from).normalize();
			Vec4 right = (up.Cross(dir)).normalize();
			Vec4 up1 = dir.Cross(right);
			return Matrix(
				right.x, right.y, right.z, -right.Dot(from),
				up1.x, up1.y, up1.z, -up1.Dot(from),
				dir.x, dir.y, dir.z, -dir.Dot(from),
				0.0f, 0.0f, 0.0f, 1.0f
			);
		}
		Matrix LookRotation(Vec3 _forward,Vec3 up_world )
		{
			Vec3 forward =_forward.normalize();
			Vec3 right = up_world * forward;
			right = right.normalize();
			Vec3 up = forward * right;
			up = up.normalize();
	

			return Matrix(
				right.x, right.y, right.z, 0,
				up.x, up.y, up.z, 0,
				forward.x, forward.y, forward.z, 0,
				0, 0, 0, 1
			);
		}
		Vec3 MulVec3(const Matrix& M, const Vec3& p)
		{
			float x = p.x * M.m[0] + p.y * M.m[4] + p.z * M.m[8] + M.m[12];
			float y = p.x * M.m[1] + p.y * M.m[5] + p.z * M.m[9] + M.m[13];
			float z = p.x * M.m[2] + p.y * M.m[6] + p.z * M.m[10] + M.m[14];
			return Vec3(x, y, z);
		}
		static float InterpYaw(float yaw0, float yaw1, float t)
		{
			
			const float TWO_PI = 2.0f *M_PI;
			const float EPS = 0.0349066f;
			float delta = yaw1 - yaw0;

			while (delta > M_PI) delta -= TWO_PI;
			while (delta < -M_PI) delta += TWO_PI;
			if (fabs(delta) < EPS)
				return yaw1;
			return yaw0 + delta * t;
		}
		Matrix static ForwardtoOnlyTRex(Vec3 forward,Vec3* now,float t) {//rotation  the collider box and the Trex
			//models have different basic forward,so only use in TRex
			Matrix RY;
			Matrix RX;
			
			float yaw = atan2f(forward.x, forward.z);
			float yawnow = atan2f(now->x, now->z);
			//yaw = InterpYaw(yawnow, yaw, t*0.1);
			//now = new Vec3(sinf(yaw),  now->y, cosf(yaw));
			now->x = sinf(yaw);
			now->z = cosf(yaw);
			//float pitch = -asinf(forward.y);
			RY = RY.rotationY(yaw);
			RX = RX.rotationX(0);
			Matrix R = RY * RX;
			return R;

		}
		Matrix static ForwardtoTOnlyHero(Vec3 forward) {//only for hero
			Matrix RY;
			Matrix RX;
			float yaw = atan2f(forward.x, forward.z);
			float pitch = -asinf(forward.y);
			RY = RY.rotationY(yaw + M_PI);
			RX = RX.rotationX(-pitch);
			Matrix R = RY * RX;
			return R;

		}
	};

	class Matric3 {
	public:
		union {
			float a[3][3];
			float m[9];
		};

		Matric3(
			float m00, float m01, float m02,
			float m10, float m11, float m12,
			float m20, float m21, float m22

		)
		{
			m[0] = m00;  m[1] = m01;  m[2] = m02;
			m[3] = m10;  m[4] = m11;  m[5] = m12;
			m[6] = m20;  m[7] = m21;  m[8] = m22;

		}

		Matric3() {
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
				{

					a[i][j] = 0.0f;
					if (i == j)
						a[i][j] = 1.0f;
				}
		}
		Matric3 Transpose() {
			Matric3 transposed;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++) {
					transposed.a[i][j] = a[j][i];
				}
			}
			return transposed;
		}
		Vec3 Dot(const Vec3& v) {

			return Vec3(a[0][0] * v.x + a[0][1] * v.y + a[0][2] * v.z,
				a[1][0] * v.x + a[1][1] * v.y + a[1][2] * v.z,
				a[2][0] * v.x + a[2][1] * v.y + a[2][2] * v.z);
		}


	};

	class ShadingFrame {
	public:
		union {
			struct {
				Vec3 tangent;
				Vec3 bitangent;
				Vec3 normal;
			};
			Vec3 axis[3];
		};
		ShadingFrame() {

			axis[0] = Vec3(1, 0, 0);
			axis[1] = Vec3(0, 1, 0);
			axis[2] = Vec3(0, 0, 1);
		};
		ShadingFrame(const Vec3& t, const Vec3& b, const Vec3& n)
			: tangent(t), bitangent(b), normal(n) {
		};
		static ShadingFrame LocalCoordinates(Triangle v) {
			Vec3 e1 = Vec4ToVec3(v.v1 - v.v0);
			Vec3 e2 = Vec4ToVec3(v.v2 - v.v0);
			Vec3 N = e1.Cross(e2);
			N = N.normalize();
			Vec3 up = Vec3(1, 0, 0);
			if (abs(N.x) > 0.99f) {
				up = Vec3(0, 1, 0);
			}
			else {
				up = Vec3(1, 0, 0);
			}

			Vec3 U = (N.Cross(up)).normalize();
			Vec3 V = (N.Cross(U)).normalize();
			return ShadingFrame(U, V, N);

		}


	};
	class Spherical {

	public:
		float theta;
		float phi;
		float r;
		Spherical() {
			theta = 0;
			phi = 0;
			r = 1;
		}
		Spherical(float _theta, float _phi, float _r) {
			theta = _theta;
			phi = _phi;
			r = _r;
		}
		Spherical DtoSphercialZup(Vec4 v) {
			Vec3 vec = v.TransToVec3();
			r = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
			theta = acos(vec.z / r);
			phi = atan2(vec.y, vec.x);
		}
		Spherical DtoSphercialyup(Vec4 v) {
			Vec3 vec = v.TransToVec3();
			r = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
			theta = acos(vec.y / r);
			phi = atan2(vec.z, vec.x);
		}

	};
	inline Matric3 GetTBNMatrix(ShadingFrame tbn) {
		Matric3 TBN(
			tbn.tangent.x, tbn.bitangent.x, tbn.normal.x,
			tbn.tangent.y, tbn.bitangent.y, tbn.normal.y,
			tbn.tangent.z, tbn.bitangent.z, tbn.normal.z
		);
		return TBN;
	}
	inline Vec4 WorldToLocal(Vec4 p, ShadingFrame tbn, Triangle v) {
		Matric3 TBN = GetTBNMatrix(tbn);
		Vec3 lp = p.TransToVec3();
		lp = lp - (v.v0).TransToVec3();
		Vec3 localP = (TBN.Transpose()).Dot(lp);
		return Vec4(localP.x, localP.y, localP.z, 1.0f);

	}
	inline Vec4 SpaceToLocal(Vec4 p, ShadingFrame tbn, Triangle v) {
		Matric3 TBN = GetTBNMatrix(tbn);
		Vec3 lp = p.TransToVec3();
		Vec3 localP = (TBN.Transpose()).Dot(lp);
		lp = lp + v.v0.TransToVec3();
		return Vec4(localP.x, localP.y, localP.z, 1.0f);

	}

	inline Vec4 SphericaltoVec4Zup(Spherical c) {
		Vec4 scv(1, 1, 1, 1);
		scv.x = c.r * sin(c.theta) * cos(c.phi);
		scv.y = c.r * sin(c.theta) * sin(c.phi);
		scv.z = c.r * cos(c.theta);
		scv.w = 1.0f;
		return scv;
	}
	inline Vec4 SphericaltoVec4yup(Spherical c) {
		Vec4 scv(1, 1, 1, 1);
		scv.x = c.r * sin(c.theta) * cos(c.phi);
		scv.z = c.r * sin(c.theta) * sin(c.phi);
		scv.y = c.r * cos(c.theta);
		scv.w = 1.0f;
		return scv;
	}
	class Quaternion {
	public:
		union {
			struct {
				float a;
				float b;
				float c;
				float d;
			};
			float q[4];
		};
		Quaternion(float _a, float _b, float _c, float _d) {
			a = _a;
			b = _b;
			c = _c;
			d = _d;
		}
		Quaternion() {
			a = 0.0f;
			b = 0.0f;
			c = 0.0f;
			d = 0.0f;
		}
		Quaternion Normalization() {
			float len = sqrt(a * a + b * b + c * c + d * d);
			return Quaternion(a / len, b / len, c / len, d / len);
		}
		Quaternion operator*(const Quaternion& q2) const {
			return Quaternion(
				a * q2.a - b * q2.b - c * q2.c - d * q2.d,  // w
				a * q2.b + b * q2.a + c * q2.d - d * q2.c,  // x
				a * q2.c - b * q2.d + c * q2.a + d * q2.b,  // y
				a * q2.d + b * q2.c - c * q2.b + d * q2.a   // z
			);
		}
		Quaternion Conjugate() const {
			Quaternion norm(a, b, c, d);

			return Quaternion(norm.a, -norm.b, -norm.c, -norm.d);
		}
		Quaternion Inverse() const {
			Quaternion norm(a, b, c, d);
			norm = norm.Normalization().Conjugate();
			float len = sqrt(norm.a * norm.a + norm.b * norm.b + norm.c * norm.c + norm.d * norm.d);
			return Quaternion(norm.a / len, norm.b / len, norm.c / len, norm.d / len);
		}
		Quaternion operator+(const Quaternion& rhs) const {
			return Quaternion(
				a + rhs.a,
				b + rhs.b,
				c + rhs.c,
				d + rhs.d
			);
		}
		Quaternion operator-(const Quaternion& rhs) const {
			return Quaternion(
				a - rhs.a,
				b - rhs.b,
				c - rhs.c,
				d - rhs.d
			);
		}
		Quaternion operator-()
		{
			return Quaternion(-a, -b, -c, -d);
		}
		float Dot(const Quaternion& rhs) const {
			return a * rhs.a + b * rhs.b + c * rhs.c + d * rhs.d;
		}
		Quaternion operator*(float scalar) const {
			return Quaternion(
				a * scalar,
				b * scalar,
				c * scalar,
				d * scalar
			);
		}
		Matrix toMatrix() {
			float aa = a * a, ab = a * b, ac = a * c;
			float bb = b * b, bc = b * c, cc = c * c;
			float da = d * a, db = d * b, dc = d * c;
			Matrix m;
			m[0] = 1 - 2 * (bb + cc); m[1] = 2 * (ab - dc); m[2] = 2 * (ac + db); m[3] = 0;
			m[4] = 2 * (ab + dc); m[5] = 1 - 2 * (aa + cc); m[6] = 2 * (bc - da); m[7] = 0;
			m[8] = 2 * (ac - db); m[9] = 2 * (bc + da); m[10] = 1 - 2 * (aa + bb); m[11] = 0;
			m[12] = m[13] = m[14] = 0; m[15] = 1;
			return m;

		}
		Quaternion FromAxisAngle(const float axis1, const float axis2, const float axis3, float angleDeg)
		{
			float half = angleDeg * 0.5f * M_PI / 180.0f;
			float s = sin(half);

			return Quaternion(cos(half), axis1 * s, axis2 * s, axis3 * s);
		}
		static Quaternion slerp(Quaternion q1, Quaternion q2, float t)
		{
			Quaternion qr;
			float dp = q1.a * q2.a + q1.b * q2.b + q1.c * q2.c + q1.d * q2.d;
			Quaternion q11 = dp < 0 ? -q1 : q1;
			dp = dp > 0 ? dp : -dp;
			float theta = acosf(clamp(dp, -1.0f, 1.0f));
			if (theta == 0)
			{
				return q1;
			}
			float d = sinf(theta);
			float a = sinf((1 - t) * theta);
			float b = sinf(t * theta);
			float coeff1 = a / d;
			float coeff2 = b / d;
			qr.a = coeff1 * q11.a + coeff2 * q2.a;
			qr.b = coeff1 * q11.b + coeff2 * q2.b;
			qr.c = coeff1 * q11.c + coeff2 * q2.c;
			qr.d = coeff1 * q11.d + coeff2 * q2.d;
			qr = qr.Normalization();
			return qr;
		}
		

	};
	class Fragment {
	public:
		float zbuffer;
		Vec3 normal;
		Fragment() {
			zbuffer = +FLT_MAX;
			normal = Vec3(0, 0, 0);
		}
		Fragment(float z, Vec3 n) {
			zbuffer = z;
			normal = n;
		}
	};

	inline Quaternion Rotateto(const Vec4 p, Quaternion q) {
		Quaternion pq = Quaternion(0, p.x, p.y, p.z);
		Quaternion qnorm = q.Normalization();
		Quaternion qConjugate = qnorm.Conjugate();
		Quaternion rotatedP = qnorm * pq * qConjugate;
		return rotatedP;
	}
	inline Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t)
	{

		float dot = q0.a * q1.a + q0.b * q1.b + q0.c * q1.c + q0.d * q1.d;

		Quaternion q1copy = q1;


		if (dot < 0.0f) {
			dot = -dot;
			q1copy = Quaternion(-q1.a, -q1.b, -q1.c, -q1.d);
		}



		float theta = acos(dot);
		float sinTheta = sin(theta);

		float w0 = sin((1 - t) * theta) / sinTheta;
		float w1 = sin(t * theta) / sinTheta;
		return (q0 * w0) + (q1copy * w1);

	}
	inline Vec4 TransformToClipspace(Vec4& v, Matrix& view) {
		Vec4 screen = v;



		Matrix scv1;
		scv1 = scv1.Perspective(M_PI / 3, (float)kuan / (float)gao, 0.1f, 100.0f);
		screen = view.mul(screen);
		screen = scv1.mul(screen);
		return screen;
	}
	inline Vec4 TransformToClipspace(Vec4& v) {
		Vec4 screen = v;
		Matrix view;
		Vec4 position(0, 0, 1, 1);
		Vec4 to(0, 0, 0, 1);
		Vec4 up(0, 1, 0, 1);
		view = view.LookatMatrix(position, to, up);
		Matrix scv1;
		scv1 = scv1.Perspective(M_PI / 3, (float)kuan / (float)gao, 0.1f, 100.0f);
		screen = view.mul(screen);
		screen = scv1.mul(screen);
		return screen;
	}
	// 判断点是否在 Near 平面内（Clip space）
	// Near plane 条件： z >= -w  等价于 (z + w) >= 0
	inline bool InsideNear(const Vec4& v)
	{
		return (v.z + v.w) >= 0.0f;
	}

	// 求 S→P 与 Near 平面的交点
	inline Vec4 IntersectNear(const Vec4& S, const Vec4& P)
	{
		float Sval = S.z + S.w;
		float Pval = P.z + P.w;

		float t = Sval / (Sval - Pval);   // 插值参数 t

		return Vec4(
			S.x + (P.x - S.x) * t,
			S.y + (P.y - S.y) * t,
			S.z + (P.z - S.z) * t,
			S.w + (P.w - S.w) * t
		);
	}

	// Sutherland–Hodgman 三角形 Near-plane 裁剪
	inline std::vector<Vec4> ClipTriangleAgainstNearPlane(const std::vector<Vec4>& tri)
	{
		std::vector<Vec4> output;

		int n = tri.size();
		if (n != 3) return output;  // 三角形必须是三个点

		for (int i = 0; i < n; i++)
		{
			Vec4 S = tri[(i + n - 1) % n];  // 上一个点
			Vec4 P = tri[i];                // 当前点

			bool S_in = InsideNear(S);
			bool P_in = InsideNear(P);

			if (S_in && P_in)
			{
				// 都在里面 → 保留 P
				output.push_back(P);
			}
			else if (S_in && !P_in)
			{
				// S 在内，P 在外 → 加交点
				Vec4 I = IntersectNear(S, P);
				output.push_back(I);
			}
			else if (!S_in && P_in)
			{
				// S 在外，P 在内 → 交点 + P
				Vec4 I = IntersectNear(S, P);
				output.push_back(I);
				output.push_back(P);
			}
			else
			{
				// 都在外 → 什么都不加
			}
		}

		return output;
	}
	inline vector<Vec4> Clip(Triangle& v)
	{
		Triangle screen = v;
		vector<Vec4> vertices = { screen.v0,screen.v1,screen.v2 };

		vertices = ClipTriangleAgainstNearPlane(vertices);
		return vertices;

	}
	inline Vec4 ClipToScreen(Vec4& v) {
		Vec4 screen = v;


		screen.DivideByW();
		screen.x = (screen.x + 1.0f) * 0.5f * kuan;
		screen.y = (1 - ((screen.y + 1.0f) * 0.5f)) * gao;
		return screen;
	}
	inline vector<Vec3> CaculateNormal(vector<Triangle>& triangles, vector<vector<int>>& vertictoTriangle, vector<Vec3>& vertixnormals) {
		for (int i = 0; i < triangles.size(); i++) {
			Vec3 e1 = Vec4ToVec3(triangles[i].v1 - triangles[i].v0);
			Vec3 e2 = Vec4ToVec3(triangles[i].v2 - triangles[i].v0);
			Vec3 N = e1.Cross(e2);
			N = N.normalize();
			triangles[i].normal = N;
		}
		for (int i = 0; i < triangles.size(); i++) {
			for (int j = 0; j < triangles[i].vertexindex.size(); j++) {

				vertixnormals[triangles[i].vertexindex[j]] = vertixnormals[triangles[i].vertexindex[j]] + triangles[i].normal;
			}
		}
		for (int i = 0; i < vertixnormals.size(); i++) {
			vertixnormals[i] = vertixnormals[i].normalize();
		}
		return vertixnormals;
	}
	inline Colour LambertDiffuse(Vec3 normal) {//直射光
		Vec3 omega_i = Vec3(1, 1, 0).normalize();
		Vec3 N = normal;
		Colour rho(0, 1.0f, 0);
		Colour L(1.0f, 1.0f, 1.0f);
		return (rho / M_PI) * L * max(Dot(omega_i, N), 0.0f);
	}
	inline Colour AmbientLighting(Vec3 normal) {//环境光
		Vec3 omega_i = Vec3(1, 1, 0).normalize();
		Vec3 N = normal;
		Colour rho(0, 1.0f, 0);
		Colour L(1.0f, 1.0f, 1.0f);
		Colour ambient(0.5f, 0.5f, 0.5f);
		return (rho / M_PI) * (L * max(Dot(omega_i, N), 0.0f) + ambient);
	}
};


#pragma once
#include "Common.h"
#include "Constants.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Quaternion.h"
#include "Random.h"
#include "Plane.h"
#include "Sphere.h"
#include "AABB.h"
#include "OBB.h"
#include "Ray.h"

namespace Storm::Math
{
	template <class T>
	constexpr T Min(T a, T b)
	{
		return (a < b) ? a : b;
	}

	template <class T>
	constexpr T Max(T a, T b)
	{
		return (a > b) ? a : b;
	}

	template <class T>
	constexpr T Clamp(T value, T min, T max)
	{
		return Max(min, Min(max, value));
	}

	template <class T>
	constexpr T Lerp(T a, T b, float t)
	{
		return a + (b - a) * t;
	}

	template <class T>
	constexpr T Abs(T value)
	{
		return value >= 0 ? value : -value;
	}

	template <class T>
	constexpr T Sqr(T value)
	{
		return value * value;
	}
	constexpr float Dot(const Vector2& v0, const Vector2& v1) { return v0.x * v1.x + v0.y * v1.y; }
	constexpr float MagnitudeSqr(const Vector2& v) { return Dot(v, v); }
	inline float Magnitude(const Vector2& v) { return sqrt(MagnitudeSqr(v)); }
	inline Vector2 Normalize(const Vector2& v) {
		Vector2 m = v / Magnitude(v);
		return m;
	}




	constexpr float Dot(const Vector3& v0, const Vector3& v1) { return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z; }
	constexpr float MagnitudeSqr(const Vector3& v) { return Dot(v, v); }
	inline float Magnitude(const Vector3& v) { return sqrt(MagnitudeSqr(v)); }
	constexpr float DistanceSqr(const Vector3& a, const Vector3& b) {
		return MagnitudeSqr(a - b);
	}
	inline float Distance(const Vector3& a, const Vector3& b) { return Magnitude(a - b); }
	inline Vector3 Normalize(const Vector3& v) {
		Vector3 m = v / Magnitude(v);
		return m;
	}
	constexpr Vector3 Cross(const Vector3& u, const Vector3& v)
	{
		Vector3 m = { u.y * v.z - v.y * u.z, u.z * v.x - v.z * u.x,u.x * v.y - v.x * u.y };
		return m;
	}
	constexpr Vector3 Lerp(const Vector3& v0, const Vector3& v1, float t)
	{
		Vector3 m = v0 + (v1 - v0) * t;
		return m;
	}

	constexpr Vector4 Lerp(const Vector4& v0, const Vector4& v1, float t)
	{
		Vector4 m = v0 + (v1 - v0) * t;
		return m;
	}

	constexpr Matrix4 Transpose(const Matrix4& m)
	{
		return Matrix4{ m._11,m._21,m._31,m._41,
					m._12,m._22,m._32,m._42,
					m._13,m._23,m._33,m._43,
					m._14,m._24,m._34,m._44 };
	}
	constexpr float Determinant(const Matrix4& m)
	{
		float a1 = m._22 * m._33 * m._44 + m._32 * m._43 * m._24 + m._42 * m._23 * m._34 - m._24 * m._33 * m._42 - m._23 * m._32 * m._44 - m._22 * m._43 * m._34;
		float a2 = m._21 * m._33 * m._44 + m._31 * m._43 * m._24 + m._41 * m._23 * m._34 - m._24 * m._33 * m._41 - m._31 * m._23 * m._44 - m._21 * m._34 * m._43;
		float a3 = m._21 * m._32 * m._44 + m._31 * m._42 * m._24 + m._41 * m._22 * m._34 - m._24 * m._32 * m._41 - m._22 * m._31 * m._44 - m._21 * m._34 * m._42;
		float a4 = m._21 * m._32 * m._43 + m._31 * m._42 * m._23 + m._41 * m._22 * m._33 - m._23 * m._32 * m._41 - m._22 * m._31 * m._43 - m._21 * m._33 * m._42;

		return m._11 * a1 - m._12 * a2 + m._13 * a3 - m._14 * a4;
	}
	constexpr Matrix4 Cofactor(const Matrix4& m)
	{
		float a11 = (m._22 * m._33 * m._44 + m._32 * m._43 * m._24 + m._42 * m._23 * m._34 - m._24 * m._33 * m._42 - m._23 * m._32 * m._44 - m._22 * m._43 * m._34);
		float a12 = -(m._21 * m._33 * m._44 + m._31 * m._43 * m._24 + m._41 * m._23 * m._34 - m._24 * m._33 * m._41 - m._31 * m._23 * m._44 - m._21 * m._34 * m._43);
		float a13 = (m._21 * m._32 * m._44 + m._31 * m._42 * m._24 + m._41 * m._22 * m._34 - m._24 * m._32 * m._41 - m._22 * m._31 * m._44 - m._21 * m._34 * m._42);
		float a14 = -(m._21 * m._32 * m._43 + m._31 * m._42 * m._23 + m._41 * m._22 * m._33 - m._23 * m._32 * m._41 - m._22 * m._31 * m._43 - m._21 * m._33 * m._42);

		float a21 = -(m._12 * m._33 * m._44 + m._32 * m._43 * m._14 + m._42 * m._13 * m._34 - m._14 * m._33 * m._42 - m._13 * m._32 * m._44 - m._12 * m._43 * m._34);
		float a22 = (m._11 * m._33 * m._44 + m._31 * m._43 * m._14 + m._41 * m._13 * m._34 - m._14 * m._33 * m._41 - m._31 * m._13 * m._44 - m._11 * m._34 * m._43);
		float a23 = -(m._11 * m._32 * m._44 + m._31 * m._42 * m._14 + m._41 * m._12 * m._34 - m._14 * m._32 * m._41 - m._12 * m._31 * m._44 - m._11 * m._34 * m._42);
		float a24 = (m._11 * m._32 * m._43 + m._31 * m._42 * m._13 + m._41 * m._12 * m._33 - m._13 * m._32 * m._41 - m._12 * m._31 * m._43 - m._11 * m._33 * m._42);

		float a31 = (m._12 * m._23 * m._44 + m._22 * m._43 * m._14 + m._42 * m._13 * m._24 - m._14 * m._23 * m._42 - m._13 * m._22 * m._44 - m._12 * m._43 * m._24);
		float a32 = -(m._11 * m._23 * m._44 + m._21 * m._43 * m._14 + m._41 * m._13 * m._24 - m._14 * m._23 * m._41 - m._21 * m._13 * m._44 - m._11 * m._24 * m._43);
		float a33 = (m._11 * m._22 * m._44 + m._21 * m._42 * m._14 + m._41 * m._12 * m._24 - m._14 * m._22 * m._41 - m._12 * m._21 * m._44 - m._11 * m._24 * m._42);
		float a34 = -(m._11 * m._22 * m._43 + m._21 * m._42 * m._13 + m._41 * m._12 * m._23 - m._13 * m._22 * m._41 - m._12 * m._21 * m._43 - m._11 * m._23 * m._42);

		float a41 = -(m._12 * m._23 * m._34 + m._22 * m._33 * m._14 + m._32 * m._13 * m._24 - m._14 * m._23 * m._32 - m._13 * m._22 * m._34 - m._12 * m._33 * m._24);
		float a42 = (m._11 * m._23 * m._34 + m._21 * m._33 * m._14 + m._31 * m._13 * m._24 - m._14 * m._23 * m._31 - m._21 * m._13 * m._34 - m._11 * m._24 * m._33);
		float a43 = -(m._11 * m._22 * m._34 + m._21 * m._32 * m._14 + m._31 * m._12 * m._24 - m._14 * m._22 * m._31 - m._12 * m._21 * m._34 - m._11 * m._24 * m._32);
		float a44 = (m._11 * m._22 * m._33 + m._21 * m._32 * m._13 + m._31 * m._12 * m._23 - m._13 * m._22 * m._31 - m._12 * m._21 * m._33 - m._11 * m._23 * m._32);

		return Matrix4{ a11,a12,a13,a14,
					a21,a22,a23,a24,
					a31,a32,a33,a34,
					a41,a42,a43,a44 };
	}
	constexpr Matrix4 Adjoint(const Matrix4& m)
	{
		return Transpose(Cofactor(m));
	}
	constexpr Matrix4 Inverse(const Matrix4& m)
	{
		const float det = Determinant(m);
		const float invDet = 1.0f / det;
		return Adjoint(m) * invDet;
	}

	constexpr Vector3 TransformCoord(const Vector3& v, const Matrix4& m)
	{
		Matrix4 vc{ v.x,v.y,v.z,1,
					0,0,0,0,
					0,0,0,0,
					0,0,0,0 };
		vc = vc * m;
		const float invw = (vc._14 == 0.0f) ? 1.0f : 1 / vc._14;
		return { vc._11 * invw,vc._12 * invw,vc._13 * invw };

	}
	constexpr Vector3 TransformNormal(const Vector3& v, const Matrix4& m)
	{
		Matrix4 vc{ v.x,v.y,v.z,0,
					0,0,0,0,
					0,0,0,0,
					0,0,0,0 };
		vc = vc * m;

		return { vc._11,vc._12,vc._13 };
	}

	constexpr Vector3 GetRight(const Matrix4& m)
	{
		return { m._11, m._12, m._13 };
	}

	constexpr Vector3 GetUp(const Matrix4& m)
	{
		return { m._21, m._22, m._23 };
	}

	constexpr Vector3 GetLook(const Matrix4& m)
	{
		return { m._31, m._32, m._33 };
	}

	constexpr Vector3 GetTranslation(const Matrix4& m)
	{
		return { m._41, m._42, m._43 };
	}



	//add Matrix4 UnitTest
	inline float Magnitude(const Quaternion& q)
	{
		return sqrtf(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
	}
	inline Quaternion Normalize(const Quaternion& q)
	{
		float magn = Magnitude(q);
		float dmagn = 1 / magn;
		return q * dmagn;
	}
	inline Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t)
	{
		Quaternion r;
		float dotproduct = q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w;
		if (fabs(dotproduct) >= 1.0f)
		{
			return q0;
		}
		float halfTheta = acos(dotproduct);

		float sinHalfTheta = sqrtf(1.0f - dotproduct * dotproduct);
		float ratioA = sin((1 - t) * halfTheta) / sinHalfTheta;
		float ratioB = sin(t * halfTheta) / sinHalfTheta;
		if (dotproduct < 0)
		{
			dotproduct = q0.x * (-q1.x) + q0.y * (-q1.y) + q0.z * (-q1.z) + q0.w * (-q1.w);
			halfTheta = acos(dotproduct);

			sinHalfTheta = sqrtf(1.0f - dotproduct * dotproduct);
			ratioA = sin((1 - t) * halfTheta) / sinHalfTheta;
			ratioB = sin(t * halfTheta) / sinHalfTheta;
			r = q0 * ratioA + (-q1) * ratioB;
		}
		else
		{
			r = q0 * ratioA + q1 * ratioB;
		}



		return r;
	}

	bool Intersect(const Ray& ray, const Plane& plane, float& distance);
	bool Intersect(const Ray& ray, const OBB& obb,Vector3& point, Vector3& normal);

	bool Intersect(const Vector3& point, const AABB& aabb);
	bool Intersect(const Vector3& point, const OBB& obb);



	bool Intersect(const Vector3& point, const Plane& plane, float& penetration);
	bool Intersect(const Sphere& sphere, const Plane& plane, float& penetration);


}
#pragma once
#include "Common.h"
namespace Storm::Math
{
	struct Vector4
	{
		union { float x; float r; };
		union { float y; float g; };
		union { float z; float b; };
		union { float w; float a; };


		constexpr Vector4() noexcept : Vector4(0.0f) {}
		explicit constexpr Vector4(float f) noexcept : Vector4(f, f, f, f) {}
		constexpr Vector4(float x, float y, float z, float w) noexcept : x(x), y(y), z(z), w(w) {}

		// Commonly used vectors
		//const static Vector4 Zero;
		//const static Vector4 One;
		//const static Vector4 XAxis;
		//const static Vector4 YAxis;
		//const static Vector4 ZAxis;
		//const static Vector4 TAxis;

		//// Constant operators - TODOs
		constexpr Vector4 operator-() const { return{ -x,-y,-z,-w }; }
		constexpr Vector4 operator+(const Vector4 v) const { return{ x + v.x, y + v.y, z + v.z,w + v.w }; }
		constexpr Vector4 operator-(const Vector4 v) const { return{ x - v.x, y - v.y, z - v.z,w - v.w }; }
		constexpr Vector4 operator*(float s) const { return{ x * s, y * s , z * s,w * s }; }
		constexpr Vector4 operator/(float s) const
		{
			float rs = 1 / s;
			return { x * rs,y * rs, z * rs, w * rs };

		}

		//// Self operators - TODOs
		//Vector4& operator+=(const Vector4& v)
		//{
		//	x += v.x;
		//	y += v.y;
		//	z += v.z;
		//	t += v.t;
		//	return *this;
		//}
		//Vector4& operator-=(const Vector4& v)
		//{
		//	x -= v.x;
		//	y -= v.y;
		//	z -= v.z;
		//	t -= v.t;
		//	return *this;
		//}
		//Vector4& operator*=(float s)
		//{
		//	x *= s;
		//	y *= s;
		//	z *= s;
		//	t *= s;
		//	return *this;
		//}
		//Vector4& operator/=(float s)
		//{
		//	float rs = 1 / s;
		//	x *= rs;
		//	y *= rs;
		//	z *= rs;
		//	t *= rs;
		//	return *this;
		//}
	};

}

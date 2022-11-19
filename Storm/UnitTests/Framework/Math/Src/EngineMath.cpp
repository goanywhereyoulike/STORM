#include "Precompiled.h"
#include "EngineMath.h"

using namespace Storm::Math;

const Vector3 Vector3::Zero{ 0.0f };
const Vector3 Vector3::One{ 1.0f,1.0f,1.0f };
const Vector3 Vector3::XAxis{ 1.0f,0.0f,0.0f };
const Vector3 Vector3::YAxis{ 0.0f,1.0f,0.0f };
const Vector3 Vector3::ZAxis{ 0.0f,0.0f,1.0f };


const Vector2 Vector2::Zero{ 0.0f };
const Vector2 Vector2::One{ 1.0f,1.0f };
const Vector2 Vector2::XAxis{ 1.0f,0.0f };
const Vector2 Vector2::YAxis{ 0.0f,1.0f };

//const Vector4 Vector4::Zero{ 0.0f };
//const Vector4 Vector4::One{ 1.0f,1.0f,1.0f,1.0f };
//const Vector4 Vector4::XAxis{ 1.0f,0.0f,0.0f,0.0f };
//const Vector4 Vector4::YAxis{ 0.0f,1.0f,0.0f,0.0f };
//const Vector4 Vector4::ZAxis{ 0.0f,0.0f,1.0f,0.0f };
//const Vector4 Vector4::TAxis{ 0.0f,0.0f,0.0f,1.0f };
// TODO - add the rest ...

const Matrix4 Matrix4::Identity{ 1,0,0,0,
								0,1,0,0,
								0,0,1,0,
								0,0,0,1 };


const Quaternion Quaternion::Identity{ 1.0f,0.0f,0.0f,0.0f };


#pragma region other Matrix stuff
Matrix4 Matrix4::Translation(const Vector3& d)
{
	return Matrix4{
	  1, 0, 0, 0,
	  0, 1, 0, 0,
	  0, 0, 1, 0,
	  d.x, d.y, d.z, 1
	};


}
Matrix4 Matrix4::Scaling(float s)
{
	return Matrix4{
		s,0,0,0,
		0,s,0,0,
		0,0,s,0,
		0,0,0,1
	};
}
Matrix4 Matrix4::Scaling(const Vector3& s)
{
	return Matrix4{
		s.x,0,0,0,
		0,s.y,0,0,
		0,0,s.z,0,
		0,0,0,1
	};
}
Matrix4 Matrix4::RotationX(float radian)
{
	const float c = cos(radian);
	const float s = sin(radian);


	return Matrix4{
		1, 0, 0, 0,
		0,c, s, 0,
		0,-s, c, 0,
		0, 0, 0, 1
	};
}
Matrix4 Matrix4::RotationY(float radian)
{
	const float c = cos(radian);
	const float s = sin(radian);

	return Matrix4{
		c, 0, -s, 0,
		0, 1, 0, 0,
		s, 0, c, 0,
		0, 0, 0, 1
	};
}
Matrix4 Matrix4::RotationZ(float radian)
{

	const float c = cos(radian);
	const float s = sin(radian);
	return Matrix4{
		c, s, 0, 0,
		-s, c, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1

	};
}
Matrix4 Matrix4::RotationAxis(const Vector3& axis, float radian)
{
	const float c = cos(radian);
	const float s = sin(radian);
	const float ic = (1.0f - c);
	Vector3 n = Normalize(axis);
	Matrix4 m0 = { n.x * n.x * ic + c,
				n.x * n.y * ic - n.z * s,
				n.x * n.z * ic + n.y * s,
				0.0f,

				n.x * n.y * ic + n.z * s,
				n.y * n.y * ic + c,
				n.y * n.z * ic - n.x * s,
				0.0f,

				n.x * n.z * ic - n.y * s,
				n.y * n.z * ic + n.x * s,
				n.z * n.z * ic + c,
				0.0f,

				0.0f,0.0f,0.0f,1.0f };

	Matrix4 m1 = Transpose(m0);

	return m1;
}

#pragma endregion

Matrix4 Matrix4::RotationQuaternion(const Quaternion& q)
{
	float n = 1.0f / sqrtf(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
	float s = q.w * n;
	float x = q.x * n;
	float y = q.y * n;
	float z = q.z * n;
	Matrix4 m{ 1 - 2 * y * y - 2 * z * z,2 * x * y - 2 * s * z,2 * x * z + 2 * s * y,0.0f,
			  2 * x * y + 2 * s * z, 1 - 2 * x * x - 2 * z * z,2 * y * z - 2 * s * x,0.0f,
			  2 * x * z - 2 * s * y,2 * y * z + 2 * s * x,1 - 2 * x * x - 2 * y * y ,0.0f,
			   0.0f,0.0f,0.0f,1.0f };
	m = Transpose(m);
	return m;
}

Quaternion Quaternion::RotationAxis(const Vector3& axis, float rad)
{
	Vector3 m = Normalize(axis);

	return { cos(rad / 2),sin(rad / 2) * m.x,sin(rad / 2) * m.y,sin(rad / 2) * m.z };

}
Quaternion Quaternion::RotationEuler(const Vector3& eulerAngles)
{
	const float cr = cos(eulerAngles.x * 0.5f);
	const float sr = sin(eulerAngles.x * 0.5f);
	const float cy = cos(eulerAngles.z * 0.5f);
	const float sy = sin(eulerAngles.z * 0.5f);
	const float cp = cos(eulerAngles.y * 0.5f);
	const float sp = sin(eulerAngles.y * 0.5f);

	return {
		cr * cp * cy + sr * sp * sy,
		sr * cp * cy - cr * sp * sy,
		cr * sp * cy + sr * cp * sy,
		cr * cp * sy - sr * sp * cy
	};

}
Quaternion Quaternion::RotationMatrix(const Matrix4& mat)
{
	Matrix4 m = Transpose(mat);
	float qw, qx, qy, qz;

	float tr = m._11 + m._22 + m._33;

	if (tr > 0)
	{
		float w = sqrt(tr + 1.0f) * 2.0f;
		qw = 0.25f * w;
		qx = (m._32 - m._23) / w;
		qy = (m._13 - m._31) / w;
		qz = (m._21 - m._12) / w;
	}
	else if ((m._11 > m._22) && (m._11 > m._33))
	{
		float w = sqrt(1.0f + m._11 - m._22 - m._33) * 2.0f;
		qw = (m._32 - m._23) / w;
		qx = 0.25f * w;
		qy = (m._12 + m._21) / w;
		qz = (m._13 + m._31) / w;
	}
	else if (m._22 > m._33)
	{
		float w = sqrt(1.0f + m._22 - m._11 - m._33) * 2.0f;
		qw = (m._13 - m._31) / w;
		qx = (m._12 + m._21) / w;
		qy = 0.25f * w;
		qz = (m._23 + m._32) / w;
	}
	else
	{
		float w = sqrt(1.0f + m._33 - m._11 - m._22) * 2.0f;
		qw = (m._21 - m._12) / w;
		qx = (m._13 + m._31) / w;
		qy = (m._23 + m._32) / w;
		qz = 0.25f * w;
	}
	return Normalize(Quaternion(qw, qx, qy, qz));
}
Quaternion Quaternion::RotationLook(const Vector3& direction, const Vector3& up)
{
	Vector3 L = Normalize(direction);
	Vector3 R = Normalize(Cross(up, L));
	Vector3 U = Normalize(Cross(L, R));

	Matrix4 m{ R.x,R.y,R.z,0.0f,
			  U.x,U.y,U.z,0.0f,
			  L.x,L.y,L.z,0.0f,
			  0.0f,0.0f,0.0f,1.0f };

	Quaternion q = RotationMatrix(m);
	return q;
}
Quaternion Quaternion::RotationFromTo(const Vector3& from, const Vector3& to)
{
	Vector3 f = Normalize(from);
	Vector3 t = Normalize(to);
	Vector3 axis = Cross(f, t);
	float theta = acos(Dot(f, t));
	Quaternion q = RotationAxis(axis, theta);
	Normalize(q);
	return q;
}

bool Storm::Math::Intersect(const Ray& ray, const Plane& plane,float& distance)
{
	const float originDotN = Dot(ray.origin, plane.normal);
	const float directionDotN = Dot(ray.direction, plane.normal);

	if (directionDotN == 0.0f)
	{
		if (originDotN == plane.distance)
		{
			distance = 0.0f;
			return true;
		}
		else
		{
			return false;
		}

	}

	distance = (plane.distance - originDotN) / directionDotN;
	return true;
}

bool Storm::Math::Intersect(const Ray& ray, const OBB& obb, Vector3& point, Vector3& normal)
{
	// Compute the local-to-world/world-to-local matrices
	const Matrix4 matTrans = Matrix4::Translation(obb.center);
	const Matrix4 matRot = Matrix4::RotationQuaternion(obb.rotation);
	const Matrix4 matWorld = matRot * matTrans;
	const Matrix4 matWorldInv = Inverse(matWorld);

	// Transform the ray into the OBB's local space
	const Vector3 org = TransformCoord(ray.origin, matWorldInv);
	const Vector3 dir = TransformNormal(ray.direction, matWorldInv);
	const Ray localRay{ org, dir };

	Plane planes[] =
	{
		{ {  0.0f,  0.0f, -1.0f }, obb.extend.z },
		{ {  0.0f,  0.0f,  1.0f }, obb.extend.z },
		{ {  0.0f, -1.0f,  0.0f }, obb.extend.y },
		{ {  0.0f,  1.0f,  0.0f }, obb.extend.y },
		{ { -1.0f,  0.0f,  0.0f }, obb.extend.x },
		{ {  1.0f,  0.0f,  0.0f }, obb.extend.x }
	};

	uint32_t numIntersections = 0;
	for (auto& plane : planes)
	{
		const float d = Dot(org, plane.normal);
		if (d > plane.distance)
		{
			float distance = 0.0f;
			if (Intersect(localRay, plane, distance) && distance >= 0.0f)
			{
				Vector3 pt = org + (dir * distance);
				if (abs(pt.x) <= obb.extend.x + 0.01f &&
					abs(pt.y) <= obb.extend.y + 0.01f &&
					abs(pt.z) <= obb.extend.z + 0.01f)
				{
					point = pt;
					normal = plane.normal;
					++numIntersections;
				}
			}
		}
	}

	if (numIntersections == 0)
	{
		return false;
	}

	point = TransformCoord(point, matWorld);
	normal = TransformNormal(normal, matWorld);
	return true;
}


bool Storm::Math::Intersect(const Vector3& point, const AABB& aabb)
{
	const auto min = aabb.Min();
	const auto max = aabb.Max();
	if (point.x<min.x || point.x>max.x ||
		point.y<min.y || point.y>max.y ||
		point.z<min.z || point.z>max.z)
	{
		return false;
	}

	return true;
}
bool Storm::Math::Intersect(const Vector3& point, const OBB& obb)
{
	Math::Matrix4 matTrans = Math::Matrix4::Translation(obb.center);
	Math::Matrix4 matRot = Math::Matrix4::RotationQuaternion(obb.rotation);
	Math::Matrix4 matScale = Math::Matrix4::Scaling(obb.extend);
	const Math::Matrix4 transform = matScale * matRot * matTrans;

	const Math::Vector3 localPoint = TransformCoord(point, Inverse(transform));
	const AABB unitAABB = { Vector3::Zero,Vector3::One };
	return Intersect(localPoint, unitAABB);

}
bool Storm::Math::Intersect(const Vector3& point, const Plane& plane, float& penetration)
{
	const float centerDistance = Dot(point, plane.normal);
	const float distToPlane = centerDistance - plane.distance;
	if (distToPlane > 0.0f)
	{
		return false;
	}

	penetration = plane.distance - centerDistance;
	return true;
}

bool Storm::Math::Intersect(const Sphere& sphere, const Plane& plane, float& penetration)
{
	const float centerDistance = Dot(sphere.center, plane.normal);
	const float distToPlane = centerDistance - plane.distance - sphere.radius;

	if (distToPlane > 0.0f)
	{
		return false;
	}
	penetration = plane.distance - centerDistance + sphere.radius;
	return true;
}

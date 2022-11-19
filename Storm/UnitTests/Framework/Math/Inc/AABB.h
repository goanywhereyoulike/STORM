#pragma once
namespace Storm::Math
{
	struct AABB
	{

		Vector3 center = Vector3::Zero;
		Vector3 extend = Vector3::One;

		constexpr Vector3 Min() const { return center - extend; }
		constexpr Vector3 Max() const { return center + extend; }
	};

}
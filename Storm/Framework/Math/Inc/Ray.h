#pragma once
namespace Storm::Math
{
	struct Ray
	{

		Vector3 origin = Vector3::Zero;
		Vector3 direction = Vector3::ZAxis;

		Vector3 GetPoint(float distance) const
		{
			return origin + (direction * distance);
		}

	};

}
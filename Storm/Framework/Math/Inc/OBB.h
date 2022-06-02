#pragma once
namespace Storm::Math
{
	struct OBB
	{

		Vector3 center = Vector3::Zero;
		Vector3 extend = Vector3::One;

		Quaternion rotation = Quaternion::Identity;


	};
}
#include "Precompiled.h"
#include "Animation.h"
#include "Transform.h"

using namespace Storm;
using namespace Storm::Graphics;
namespace
{
	Math::Vector3 Lerpresult(const Math::Vector3& a, const Math::Vector3& b, float t)
	{
		return Math::Lerp(a, b, t);
	}

	Math::Quaternion Lerpresult(const Math::Quaternion& a, const Math::Quaternion& b, float t)
	{
		return Math::Slerp(a, b, t);
	}

	template<class T>
	size_t FindKeyframeIndex(const Keyframes<T>& keyframes, float time)
	{
		bool NotIntheChannel = false;
		for (int i = 0; i < keyframes.size(); ++i)
		{
			if (keyframes[i].time >= time)
			{

				if (i == 0)
				{
					return 0;
				}
				else
				{
					return i - 1;
				}

				NotIntheChannel = true;
				break;
			}

		}
		if (!NotIntheChannel)
		{
			return keyframes.size() - 2;
		}
		return 0;
	}

	template<class T>
	T GetValue(const Keyframes<T>& keyframes, float time, T defaultValue)
	{
		if (keyframes.empty())
		{
			return defaultValue;
		}

		const Keyframe<T>& firstKey = keyframes.front();
		const Keyframe<T>& lastKey = keyframes.back();
		if (time <= firstKey.time)
		{
			return firstKey.key;
		}
		if (time >= lastKey.time)
		{
			return lastKey.key;
		}

		const size_t index = FindKeyframeIndex(keyframes, time);
		const Keyframe<T>& fromKey = keyframes[index];
		const Keyframe<T>& toKey = keyframes[index + 1];
		const float percent = (time - fromKey.time) / (toKey.time - fromKey.time);

		return Lerpresult(fromKey.key, toKey.key, percent);
	}

}




Math::Vector3 Animation::GetPosition(float time) const
{

	return GetValue(mPositionKeys, time, Math::Vector3::Zero);
}

Math::Quaternion Animation::GetRotation(float time) const
{
	return GetValue(mRotationKeys, time, Math::Quaternion::Identity);
}

Math::Vector3 Animation::GetScale(float time) const
{
	return GetValue(mScaleKeys, time, Math::Vector3::Zero);
}

Math::Matrix4 Animation::GetTransform(float time) const
{
	const auto& position = GetPosition(time);
	const auto& rotation = GetRotation(time);
	const auto& scale = GetScale(time);

	return Transform{ position,rotation,scale }.GetMatrix4();
}

float Animation::GetDuration() const
{

	return Math::Max(mPositionKeys.back().time, Math::Max(mRotationKeys.back().time, mScaleKeys.back().time));

}

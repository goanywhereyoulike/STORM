#include "Precompiled.h"
#include "AnimationBuilder.h"

using namespace Storm;
using namespace Storm::Graphics;

AnimationBuilder& AnimationBuilder::AddPositionKey(const Math::Vector3& position, float time)
{
	auto& keyframes = mWorkingCopy.mPositionKeys;
	ASSERT(keyframes.empty() || keyframes.back().time < time, "AnimationBuilder -- Cannot add keyframe back in time.");
	keyframes.emplace_back(position, time);
	return *this;


}
AnimationBuilder& AnimationBuilder::AddRotationKey(const Math::Quaternion& rotation, float time)
{
	auto& keyframes = mWorkingCopy.mRotationKeys;
	ASSERT(keyframes.empty() || keyframes.back().time < time, "AnimationBuilder -- Cannot add keyframe back in time.");
	keyframes.emplace_back(rotation, time);
	return *this;

}
AnimationBuilder& AnimationBuilder::AddScaleKey(const Math::Vector3& scale, float time)
{
	auto& keyframes = mWorkingCopy.mScaleKeys;
	ASSERT(keyframes.empty() || keyframes.back().time < time, "AnimationBuilder -- Cannot add keyframe back in time.");
	keyframes.emplace_back(scale, time);
	return *this;

}

Animation AnimationBuilder::Get()
{
	ASSERT(!mWorkingCopy.mPositionKeys.empty(), "AnimationBuilder -- Animation has no position keys.");
	ASSERT(!mWorkingCopy.mRotationKeys.empty(), "AnimationBuilder -- Animation has no rotation keys.");
	ASSERT(!mWorkingCopy.mScaleKeys.empty(), "AnimationBuilder -- Animation has no scaled keys.");
	return std::move(mWorkingCopy);

}
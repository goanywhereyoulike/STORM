#include "Precompiled.h"
#include "AnimationIO.h"

using namespace Storm;
using namespace Storm::Graphics;

void AnimationIO::Write(FILE* file, const Animation& animation)
{
	const auto& Poskeyframes = animation.mPositionKeys;

	const uint32_t poskeyframesCount = static_cast<uint32_t>(Poskeyframes.size());
	fprintf_s(file, "PositionkeyframesCount: %u\n", poskeyframesCount);
	if (!Poskeyframes.empty())
	{
		for (uint32_t i = 0; i < poskeyframesCount; ++i)
		{
			const auto& PositionKeyFrame = Poskeyframes[i];
			fprintf_s(file, "%f\n", PositionKeyFrame.time);
			fprintf_s(file, "%f %f %f\n", PositionKeyFrame.key.x, PositionKeyFrame.key.y, PositionKeyFrame.key.z);
		}
	}

	const auto& Rotkeyframes = animation.mRotationKeys;
	const uint32_t rotkeyframesCount = static_cast<uint32_t>(Rotkeyframes.size());
	fprintf_s(file, "RotationkeyframesCount: %u\n", rotkeyframesCount);
	if (!Rotkeyframes.empty())
	{
		for (uint32_t i = 0; i < rotkeyframesCount; ++i)
		{
			const auto& RotationKeyFrame = Rotkeyframes[i];
			fprintf_s(file, "%f\n", RotationKeyFrame.time);
			fprintf_s(file, "%f %f %f %f\n", RotationKeyFrame.key.x, RotationKeyFrame.key.y, RotationKeyFrame.key.z, RotationKeyFrame.key.w);

		}
	}

	const auto& Scakeyframes = animation.mScaleKeys;
	const uint32_t scakeyframesCount = static_cast<uint32_t>(Scakeyframes.size());
	fprintf_s(file, "ScalekeyframesCount: %u\n", scakeyframesCount);
	if (!Scakeyframes.empty())
	{
		for (uint32_t i = 0; i < scakeyframesCount; ++i)
		{
			const auto& ScaleKeyFrame = Scakeyframes[i];
			fprintf_s(file, "%f\n", ScaleKeyFrame.time);
			fprintf_s(file, "%f %f %f\n", ScaleKeyFrame.key.x, ScaleKeyFrame.key.y, ScaleKeyFrame.key.z);
		}
	}

}

void AnimationIO::Read(FILE* file, Animation& animation)
{
	auto& Poskeyframes = animation.mPositionKeys;

	uint32_t poskeyframesCount = 0;
	fscanf_s(file, "PositionkeyframesCount: %u\n", &poskeyframesCount);
	if (poskeyframesCount > 0)
	{
		Poskeyframes.resize(poskeyframesCount);
		for (uint32_t i = 0; i < poskeyframesCount; ++i)
		{
			auto& PositionKeyFrame = Poskeyframes[i];
			fscanf_s(file, "%f\n", &PositionKeyFrame.time);
			fscanf_s(file, "%f %f %f\n", &PositionKeyFrame.key.x, &PositionKeyFrame.key.y, &PositionKeyFrame.key.z);

		}
	}


	auto& Rotkeyframes = animation.mRotationKeys;
	uint32_t rotkeyframesCount = 0;
	fscanf_s(file, "RotationkeyframesCount: %u\n", &rotkeyframesCount);
	if (rotkeyframesCount > 0)
	{
		Rotkeyframes.resize(rotkeyframesCount);
		for (uint32_t i = 0; i < rotkeyframesCount; ++i)
		{
			auto& RotationKeyFrame = Rotkeyframes[i];
			fscanf_s(file, "%f\n", &RotationKeyFrame.time);
			fscanf_s(file, "%f %f %f %f\n", &RotationKeyFrame.key.x, &RotationKeyFrame.key.y, &RotationKeyFrame.key.z, &RotationKeyFrame.key.w);
		}
	}

	auto& Scakeyframes = animation.mScaleKeys;
	uint32_t scakeyframesCount = 0;
	fscanf_s(file, "ScalekeyframesCount: %u\n", &scakeyframesCount);
	if (scakeyframesCount > 0)
	{
		Scakeyframes.resize(scakeyframesCount);
		for (uint32_t i = 0; i < scakeyframesCount; ++i)
		{
			auto& ScaleKeyFrame = Scakeyframes[i];
			fscanf_s(file, "%f\n", &ScaleKeyFrame.time);
			fscanf_s(file, "%f %f %f\n", &ScaleKeyFrame.key.x, &ScaleKeyFrame.key.y, &ScaleKeyFrame.key.z);
		}
	}


}

#pragma once

#include "Common.h"
#include "Animation.h"


namespace Storm::Graphics
{

	class AnimationClip
	{

	public: 
		bool HasTransform(uint32_t boneIndex) const
		{
			const auto& boneAnim = boneAnimations[boneIndex];
			return (boneAnim != nullptr);

		}
		//Transform GetTransform(float time, uint32_t boneIndex) const;

		std::string name;

		float tickDuration = 0.0f;
		float ticksPerSecond = 0.0f;
		std::vector<std::unique_ptr<Animation>> boneAnimations;

	};

}
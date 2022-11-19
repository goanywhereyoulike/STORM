#include "Precompiled.h"
#include "Animator.h"
#include "Model.h"
#include "Transform.h"

using namespace Storm;
using namespace Storm::Graphics;

void Animator::Initialize(const Model* model)
{
	mModel = model;
}

void Animator::PlayAnimation(size_t clipIndex)
{
	ASSERT(clipIndex < mModel->animationSet.size(), "Animator -- Invalid animation clip index.");
	if (mClipIndex != clipIndex)
	{
		mClipIndex = clipIndex;
		mAniamtionTick = 0;
	}

}

void Animator::Update(float deltaTime)
{
	const AnimationClip& clip = mModel->animationSet[mClipIndex];
	mAniamtionTick += clip.ticksPerSecond * deltaTime;
	while (mAniamtionTick > clip.tickDuration)
	{
		mAniamtionTick -= clip.tickDuration;
	}

}

Math::Vector3 Animator::Getposition(const Bone* bone) const
{
	Math::Vector3 postion = Math::GetTranslation(bone->toParentTransform);
	const AnimationClip& clip = mModel->animationSet[mClipIndex];
	if (clip.HasTransform(bone->index))
	{
		postion = clip.boneAnimations[bone->index]->GetPosition(mAniamtionTick);
	}

	return postion;

}

Math::Quaternion Animator::Getrotation(const Bone* bone) const
{
	Math::Quaternion rotation = Math::Quaternion::RotationMatrix(bone->toParentTransform);
	const AnimationClip& clip = mModel->animationSet[mClipIndex];
	if (clip.HasTransform(bone->index))
	{
		rotation = clip.boneAnimations[bone->index]->GetRotation(mAniamtionTick);
	}

	return rotation;
}

Math::Vector3 Animator::Getscale(const Bone* bone) const
{
	Math::Vector3 scale = Math::Vector3::One;
	const AnimationClip& clip = mModel->animationSet[mClipIndex];
	if (clip.HasTransform(bone->index))
	{
		scale = clip.boneAnimations[bone->index]->GetScale(mAniamtionTick);
	}

	return scale;
}

Math::Matrix4 Animator::GetTransform(const Bone* bone) const
{
	Math::Matrix4 toParentTransform = bone->toParentTransform;
	const AnimationClip& clip = mModel->animationSet[mClipIndex];
	if (clip.HasTransform(bone->index))
	{
		toParentTransform = clip.boneAnimations[bone->index]->GetTransform(mAniamtionTick);
	}

	return toParentTransform;

}

Animator* AnimationBlender::AddAnimator()
{
	return &mEntries.emplace_back(std::make_unique<Entry>())->animator;
}

void AnimationBlender::SetBlendWeight(size_t index, float weight)
{
	mEntries[index]->weight = weight;
}

void AnimationBlender::Update(float deltaTime)
{
	for (auto& entry : mEntries)
	{
		entry->animator.Update(deltaTime);
	}
}

Math::Matrix4 AnimationBlender::GetTransform(const Bone* bone) const
{
	Math::Vector3 position = Math::Vector3::Zero;
	Math::Quaternion rotation = Math::Quaternion::Identity;
	Math::Vector3 scale = Math::Vector3::One;

	//for (int i = 0; i < mEntries.size() - 1; ++i)
	//{
	//	position = Math::Lerp(mEntries[i]->animator.Getposition(bone), mEntries[i + 1]->animator.Getposition(bone), mEntries[i + 1]->weight);
	//	rotation = Math::Slerp(mEntries[i]->animator.Getrotation(bone), mEntries[i + 1]->animator.Getrotation(bone), mEntries[i + 1]->weight);
	//}
	//if (mEntries[1]->weight <= 0.5f)
	//{
	//	float t = mEntries[1]->weight / 0.5f;
	//	position = Math::Lerp(mEntries[0]->animator.Getposition(bone), mEntries[1]->animator.Getposition(bone), t);
	//	rotation = Math::Slerp(mEntries[0]->animator.Getrotation(bone), mEntries[1]->animator.Getrotation(bone), t);
	//}
	//if (mEntries[2]->weight > 0.5f)
	//{
	//	float t = (mEntries[2]->weight - 0.5f) / 0.5f;
	//	position = Math::Lerp(mEntries[1]->animator.Getposition(bone), mEntries[2]->animator.Getposition(bone), t);
	//	rotation = Math::Slerp(mEntries[1]->animator.Getrotation(bone), mEntries[2]->animator.Getrotation(bone), t);
	//}
	position = mEntries[0]->animator.Getposition(bone);
	rotation = mEntries[0]->animator.Getrotation(bone);
	scale = mEntries[0]->animator.Getscale(bone);
	//scale = Math::Lerp(scale, mEntries[0]->animator.Getscale(bone), mEntries[0]->weight);
	return Transform{ position, rotation, scale }.GetMatrix4();

}

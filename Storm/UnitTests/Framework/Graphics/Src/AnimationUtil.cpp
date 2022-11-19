#include "Precompiled.h"
#include "AnimationUtil.h"
#include "SimpleDraw.h"
using namespace Storm;
using namespace Storm::Graphics;

namespace
{
	void ComputeBoneTransform(Bone* bone, std::vector<Math::Matrix4>& boneTransforms)
	{
		if (bone->parent)
		{
			boneTransforms[bone->index] = bone->toParentTransform * boneTransforms[bone->parentIndex];
		}
		for (auto child : bone->children)
		{
			ComputeBoneTransform(child, boneTransforms);
		}
	}
	void ComputeTransform(Bone* bone, std::vector<Math::Matrix4>& boneTransforms, AnimationUtil::GetBoneTransform getBoneTransform)
	{
		if (bone->parent)
		{
			boneTransforms[bone->index] = getBoneTransform(bone) * boneTransforms[bone->parentIndex];
		}
		for (auto child : bone->children)
		{
			ComputeTransform(child, boneTransforms,getBoneTransform);
		}
	
	}

}

void AnimationUtil::DrawSkeleton(const Skeleton& skeleton, const std::vector<Math::Matrix4>& boneTransforms)
{
	for (auto& bone : skeleton.bones)
	{
		const Bone* parentBone = bone->parent;
		if (parentBone)
		{
			const Math::Vector3& bonePosition = Math::GetTranslation(boneTransforms[bone->index]);
			const Math::Vector3& parentPosition = Math::GetTranslation(boneTransforms[parentBone->index]);
			SimpleDraw::AddLine(bonePosition, parentPosition, Colors::Green);
			SimpleDraw::AddSphere(bonePosition, 0.05f, Colors::Blue, 3, 4);
		}
	}

}

void AnimationUtil::ComputeBindPose(const Skeleton& skeleton, std::vector<Math::Matrix4>& boneTransforms)
{
	boneTransforms.resize(skeleton.bones.size(), Math::Matrix4::Identity);
	ComputeBoneTransform(skeleton.root, boneTransforms);
}

void AnimationUtil::ComputeBoneTransforms(const Skeleton& skeleton, std::vector<Math::Matrix4>& boneTransforms, GetBoneTransform getBoneTransform)
{
	boneTransforms.resize(skeleton.bones.size(), Math::Matrix4::Identity);
	ComputeTransform(skeleton.root, boneTransforms, getBoneTransform);
}

void AnimationUtil::ApplyBoneOffset(const Skeleton& skeleton, std::vector<Math::Matrix4>& boneTransforms)
{
	boneTransforms.resize(skeleton.bones.size(), Math::Matrix4::Identity);
	for (auto& bone : skeleton.bones)
		boneTransforms[bone->index] = bone->offsetTransform * boneTransforms[bone->index];
}

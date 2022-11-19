#include "Precompiled.h"
#include "AnimatorComponent.h"

#include "GameObject.h"
#include "ModelComponent.h"

using namespace Storm;
using namespace Storm::Graphics;

MEMORY_POOL_DEFINE(AnimatorComponent, 1000);

void AnimatorComponent::Initialize()
{
	mModelComponent = GetOwner().GetComponent<ModelComponent>();
	Model& model = mModelComponent->GetModel();
	if (model.animationSet.size() == 1)
	{
		for (auto& animationFileName : mAnimatorFileNames)
		{
			ModelIO::LoadAnimationSet(animationFileName, model);
		}
	}
	mAnimator.Initialize(&model);
	//mAnimator.PlayAnimation(1);
}

void AnimatorComponent::Terminate()
{
}

void AnimatorComponent::Update(float deltaTime)
{
	mAnimator.Update(deltaTime);
}

void AnimatorComponent::DebugUI()
{
}

void AnimatorComponent::AddAnimation(std::string animationFileName)
{
	mAnimatorFileNames.emplace_back(std::move(animationFileName));
}

void AnimatorComponent::PlayAnimation(size_t index)
{
	mAnimator.PlayAnimation(index);
}

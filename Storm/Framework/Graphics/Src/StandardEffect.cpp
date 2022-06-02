#include "Precompiled.h"
#include "StandardEffect.h"
#include "Camera.h"
#include "RenderObject.h"
#include "VertexTypes.h"
#include "AnimationUtil.h"
#include "Animator.h"

using namespace Storm;
using namespace Storm::Graphics;

void StandardEffect::Initialize()
{
	mVertexShader.Initialize<Vertex>("../../Assets/Shaders/Standard.fx");
	mPixelShader.Initialize("../../Assets/Shaders/Standard.fx");
	mTransformBuffer.Initialize();
	mBoneTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mSettingsBuffer.Initialize();
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void StandardEffect::Terminate()
{
	mSampler.Terminate();
	mSettingsBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mBoneTransformBuffer.Terminate();
	mTransformBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();


}

void StandardEffect::Begin()
{
	mVertexShader.Bind();
	mPixelShader.Bind();

	mTransformBuffer.BindVS(0);

	mBoneTransformBuffer.BindVS(1);

	mLightBuffer.BindVS(2);
	mLightBuffer.BindPS(2);

	mMaterialBuffer.BindVS(3);
	mMaterialBuffer.BindPS(3);

	mSettingsBuffer.BindPS(4);
	mSettingsBuffer.BindVS(4);

	mSampler.BindVS(0);
	mSampler.BindPS(0);
}

void StandardEffect::End()
{
	if (mShadowMap != nullptr)
	{
		Texture::UnbindPS(4);
	}

}

void StandardEffect::Render(const RenderObject& renderObject)
{
	ASSERT(mCamera != nullptr, "StandardEffect -- no camera set!");
	ASSERT(mDirectionalLight != nullptr, "StandardEffect -- no light set!");
	Math::Matrix4 world = renderObject.transform.GetMatrix4();
	Math::Matrix4 view = mCamera->GetViewMatrix();
	Math::Matrix4 proj = mCamera->GetProjectionMatrix();

	TransformData transformData;
	transformData.world = Math::Transpose(world);
	transformData.wvp[0] = Math::Transpose(world * view * proj);
	if (mUseShadowMap && mLightCamera != nullptr)
	{
		view = mLightCamera->GetViewMatrix();
		proj = mLightCamera->GetProjectionMatrix();
		transformData.wvp[1] = Math::Transpose(world * view * proj);
	}
	transformData.viewPosition = mCamera->GetPosition();

	SettingsData settingsData;
	settingsData.useSpecularMap = (mUseSpecularFlag && renderObject.specularMapId != 0) ? 1 : 0;
	settingsData.useDisplacementMap = (mUseDisplacementFlag && renderObject.displacementMapId != 0) ? 1 : 0;
	settingsData.useNormalMap = (mUseNorFlag && renderObject.normalMapId != 0) ? 1 : 0;
	settingsData.displacementWeight = Math::Clamp(mDisplacementWeight, 0.0f, 1.0f);
	if (mUseShadowMap && mShadowMap != nullptr)
	{
		mShadowMap->BindPS(4);
		settingsData.useShadowMap = 1;
		settingsData.depthBias = mDepthBias;

	}

	if (renderObject.animator)
	{
		BoneTransformData boneTransformData;
		std::vector<Math::Matrix4> boneTransforms;
		AnimationUtil::ComputeBoneTransforms(*renderObject.skeleton, boneTransforms,
			[animator = renderObject.animator](const Bone* bone)
		{
			return animator->GetTransform(bone);
		});
		AnimationUtil::ApplyBoneOffset(*renderObject.skeleton, boneTransforms);

		const size_t boneCount = renderObject.skeleton->bones.size();
		for (size_t i = 0; i < boneCount && i < BoneTransformData::MaxBoneCount; ++i)
		{
			boneTransformData.boneTransforms[i] = Math::Transpose(boneTransforms[i]);

		}
		mBoneTransformBuffer.Update(boneTransformData);
		settingsData.useSkinning = 1;

	}
	else if (renderObject.skeleton)
	{
		BoneTransformData boneTransformData;

		std::vector<Math::Matrix4> boneTransforms;
		AnimationUtil::ComputeBoneTransforms(*renderObject.skeleton, boneTransforms, [](const Bone* bone) {return bone->toParentTransform; });
		AnimationUtil::ApplyBoneOffset(*renderObject.skeleton, boneTransforms);

		const size_t boneCount = renderObject.skeleton->bones.size();
		for (size_t i = 0; i < boneCount && i < BoneTransformData::MaxBoneCount; ++i)
		{
			boneTransformData.boneTransforms[i] = Math::Transpose(boneTransforms[i]);

		}

		mBoneTransformBuffer.Update(boneTransformData);
		settingsData.useSkinning = 1;

	}

	mTransformBuffer.Update(transformData);
	mLightBuffer.Update(*mDirectionalLight);
	mMaterialBuffer.Update(renderObject.material);
	mSettingsBuffer.Update(settingsData);

	auto tm = TextureManager::Get();
	tm->BindPS(renderObject.diffuseMapId, 0);
	tm->BindPS(renderObject.specularMapId, 1);
	tm->BindVS(renderObject.displacementMapId, 2);
	tm->BindPS(renderObject.normalMapId, 3);

	renderObject.meshBuffer.Render();

}

void StandardEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;

}

void StandardEffect::SetLightCamera(const Camera* camera)
{
	mLightCamera = camera;
}

void StandardEffect::SetDirectionalLight(const DirectionalLight& directionalLight)
{
	mDirectionalLight = &directionalLight;

}

void StandardEffect::SetShadowMap(const Texture* shadowMap)
{
	mShadowMap = shadowMap;
}

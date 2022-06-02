#include "Precompiled.h"
#include "ShadowEffect.h"
#include "Camera.h"
#include "RenderObject.h"
#include "VertexTypes.h"
#include "AnimationUtil.h"
#include "Animator.h"

using namespace Storm;
using namespace Storm::Graphics;

void ShadowEffect::Initialize()
{
	mLightCamera.SetMode(Camera::ProjectionMode::Orthographic);

	mVertexShader.Initialize<Vertex>("../../Assets/Shaders/Shadow.fx");
	mPixelShader.Initialize("../../Assets/Shaders/Shadow.fx");
	mTransformBuffer.Initialize();
	mBoneTransformBuffer.Initialize();
	mSettingsBuffer.Initialize();
	constexpr uint32_t depthMapSize = 4096;
	mDepthMapRenderTarget.Initialize(depthMapSize, depthMapSize, Texture::Format::RGBA_U32);
}

void ShadowEffect::Terminate()
{
	mDepthMapRenderTarget.Terminate();
	mSettingsBuffer.Terminate();
	mBoneTransformBuffer.Terminate();
	mTransformBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void ShadowEffect::Begin()
{
	UpdateLightCamera();

	mVertexShader.Bind();
	mPixelShader.Bind();

	mTransformBuffer.BindVS(0);
	mBoneTransformBuffer.BindVS(1);

	mSettingsBuffer.BindPS(2);
	mSettingsBuffer.BindVS(2);
	mDepthMapRenderTarget.BeginRender();

}

void ShadowEffect::End()
{
	mDepthMapRenderTarget.EndRender();
}

void ShadowEffect::Render(const RenderObject& renderObject)
{
	ASSERT(mDirectionalLight != nullptr, "ShadowEffect -- no light set!");
	Math::Matrix4 world = renderObject.transform.GetMatrix4();
	Math::Matrix4 view = mLightCamera.GetViewMatrix();
	Math::Matrix4 proj = mLightCamera.GetProjectionMatrix();

	TransformData transformData;
	transformData.wvp = Math::Transpose(world * view * proj);
	mTransformBuffer.Update(transformData);

	SettingsData settingsData;
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
		AnimationUtil::ComputeBindPose(*renderObject.skeleton, boneTransforms);
		AnimationUtil::ApplyBoneOffset(*renderObject.skeleton, boneTransforms);

		const size_t boneCount = renderObject.skeleton->bones.size();
		for (size_t i = 0; i < boneCount && i < BoneTransformData::MaxBoneCount; ++i)
		{
			boneTransformData.boneTransforms[i] = Math::Transpose(boneTransforms[i]);

		}

		mBoneTransformBuffer.Update(boneTransformData);
		settingsData.useSkinning = 1;

	}

	mSettingsBuffer.Update(settingsData);
	renderObject.meshBuffer.Render();
}

void ShadowEffect::SetDirectionalLight(const DirectionalLight& directionalLight)
{
	mDirectionalLight = &directionalLight;
}

void ShadowEffect::UpdateLightCamera()
{
	ASSERT(mDirectionalLight != nullptr, "ShadowEffect -- no light set!");

	const Math::Vector3 direction = mDirectionalLight->direction;
	mLightCamera.SetDirection(direction);
	mLightCamera.SetPosition(-direction * 1000.0f);
	mLightCamera.SetNearPlane(1.0f);
	mLightCamera.SetFarPlane(2000.0f);
	mLightCamera.SetSize(1000.0f, 1000.0f);

}

void ShadowEffect::SetSize(float size)
{
	mLightCamera.SetSize(size, size);
}

void ShadowEffect::SetFocus(Math::Vector3 position)
{
	mLightCamera.SetPosition(position);
}

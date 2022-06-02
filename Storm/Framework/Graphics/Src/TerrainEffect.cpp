#include "Precompiled.h"
#include "TerrainEffect.h"
#include "Camera.h"
#include "RenderObject.h"
#include "VertexTypes.h"
#include "AnimationUtil.h"
#include "Animator.h"

using namespace Storm;
using namespace Storm::Graphics;

void TerrainEffect::Initialize()
{
	mVertexShader.Initialize<Vertex>("../../Assets/Shaders/Terrain.fx");
	mPixelShader.Initialize("../../Assets/Shaders/Terrain.fx");

	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mSettingsBuffer.Initialize();

	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void TerrainEffect::Terminate()
{
	mSampler.Terminate();
	mSettingsBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mTransformBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void TerrainEffect::Begin()
{
	mVertexShader.Bind();
	mPixelShader.Bind();

	mTransformBuffer.BindVS(0);

	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);

	mMaterialBuffer.BindPS(2);

	mSettingsBuffer.BindPS(3);
	mSettingsBuffer.BindVS(3);

	mSampler.BindVS(0);
	mSampler.BindPS(0);
}

void TerrainEffect::End()
{
	if (mShadowMap != nullptr)
	{
		Texture::UnbindPS(2);
	}

}

void TerrainEffect::Render(const RenderObject& renderObject)
{
	ASSERT(mCamera != nullptr, "TerrainEffect -- no camera set!");
	ASSERT(mDirectionalLight != nullptr, "TerrainEffect -- no light set!");
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
	if (mUseShadowMap && mShadowMap != nullptr)
	{
		mShadowMap->BindPS(2);
		settingsData.useShadowMap = 1;
		settingsData.depthBias = mDepthBias;

	}

	mTransformBuffer.Update(transformData);
	mLightBuffer.Update(*mDirectionalLight);
	mMaterialBuffer.Update(renderObject.material);
	mSettingsBuffer.Update(settingsData);

	auto tm = TextureManager::Get();
	tm->BindPS(renderObject.diffuseMapId, 0);
	tm->BindPS(renderObject.specularMapId, 1);

	renderObject.meshBuffer.Render();

}

void TerrainEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;

}

void TerrainEffect::SetLightCamera(const Camera* camera)
{
	mLightCamera = camera;
}

void TerrainEffect::SetDirectionalLight(const DirectionalLight& directionalLight)
{
	mDirectionalLight = &directionalLight;

}

void TerrainEffect::SetShadowMap(const Texture* shadowMap)
{
	mShadowMap = shadowMap;
}

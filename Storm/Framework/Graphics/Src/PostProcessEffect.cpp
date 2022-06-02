#include "Precompiled.h"
#include "PostProcessEffect.h"
#include "Camera.h"
#include "RenderObject.h"
#include "VertexTypes.h"

using namespace Storm;
using namespace Storm::Graphics;

void PostProcessEffect::Initialize()
{
	mPostProcessVertexShader.Initialize<VertexPX>(L"../../Assets/Shaders/PostProcess.fx");
	mPostProcessPixelShader.Initialize(L"../../Assets/Shaders/PostProcess.fx");
	mPostProcessBuffer.Initialize();
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void PostProcessEffect::Terminate()
{
	mSampler.Terminate();
	mPostProcessBuffer.Terminate();

	mPostProcessPixelShader.Terminate();
	mPostProcessVertexShader.Terminate();
}

void PostProcessEffect::Begin()
{
	mPostProcessData.time = Storm::Core::TimeUtil::GetTime();
	mPostProcessVertexShader.Bind();
	mPostProcessPixelShader.Bind();

	switch (mPostProcessData.mode)
	{
	case Mode::Emboss:
		mPostProcessData.embossamount = Math::Clamp(mEmbossamount, 1.0f, 50.0f);
		mPostProcessData.useEmbossEffect = mUseEmbossEffect;
		break;
	case Mode::Blur:
		mPostProcessData.textureDimensions = Math::Clamp(mTextureDimensions, 1, 500);
		mPostProcessData.useBlurEffect = mUseBlurEffect;
		break;
	case Mode::Glitch:
		mPostProcessData.block_size= Math::Clamp(mBlockSize, 1.0f, 50.0f);
		mPostProcessData.speed = Math::Clamp(mSpeed, 1.0f, 50.0f);
		mPostProcessData.useGlitchEffect = mUseGlitchEffect;
		break;
	case Mode::Bloom:
		break;
	}
	mPostProcessBuffer.Update(mPostProcessData);
	mPostProcessBuffer.BindPS(0);

	for (uint32_t i = 0; i < std::size(mTextures); ++i)
	{
		auto texture = mTextures[i];
		if (texture)
			texture->BindPS(i);

	}
	mSampler.BindVS(0);
	mSampler.BindPS(0);
}

void PostProcessEffect::End()
{
	for (uint32_t i = 0; i < std::size(mTextures); ++i)
	{
		Texture::UnbindPS(i);

	}
}

void PostProcessEffect::Render(const RenderObject& renderObject)
{
	//mBaseRenderTarget.BindPS(0);
	//mVBlurRenderTarget.BindPS(1);
	renderObject.meshBuffer.Render();

}

void PostProcessEffect::SetTexture(const Texture* texture, uint32_t slot)
{
	ASSERT(slot < std::size(mTextures), "PostprocessingEffect -- Invalid slot index.");
	mTextures[slot] = texture;
}

void PostProcessEffect::SetMode(Mode mode)
{
	mPostProcessData.mode = static_cast<int>(mode);

}

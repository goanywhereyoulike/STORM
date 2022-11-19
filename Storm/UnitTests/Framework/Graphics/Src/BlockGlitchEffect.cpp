#include "Precompiled.h"
#include "BlockGlitchEffect.h"
#include "Camera.h"
#include "RenderObject.h"
#include "VertexTypes.h"

using namespace Storm;
using namespace Storm::Graphics;

void BlockGlitchEffect::Initialize()
{
	mPostProcessVertexShader.Initialize<VertexPX>(L"../../Assets/Shaders/BlockGlitch.fx");
	mPostProcessPixelShader.Initialize(L"../../Assets/Shaders/BlockGlitch.fx");
	mPostProcessBuffer.Initialize();
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void BlockGlitchEffect::Terminate()
{
	mSampler.Terminate();
	mPostProcessBuffer.Terminate();

	mPostProcessPixelShader.Terminate();
	mPostProcessVertexShader.Terminate();
}

void BlockGlitchEffect::Begin()
{
	mPostProcessData.time = Storm::Core::TimeUtil::GetTime();
	mPostProcessVertexShader.Bind();
	mPostProcessPixelShader.Bind();

	switch (mPostProcessData.mode)
	{
	case Mode::Glitch:
		mPostProcessData.block_size = Math::Clamp(block_size, 1.0f, 50.0f);
		mPostProcessData.speed = Math::Clamp(speed, 1.0f, 50.0f);
		mPostProcessData.fade = Math::Clamp(fade, 1.0f, 50.0f);
		mPostProcessData.offset = Math::Clamp(offset, 1.0f, 50.0f);
		mPostProcessData._BlockLayer1_U = Math::Clamp(_BlockLayer1_U, 1.0f, 50.0f);
		mPostProcessData._BlockLayer1_V = Math::Clamp(_BlockLayer1_V, 1.0f, 50.0f);
		mPostProcessData._BlockLayer2_U = Math::Clamp(_BlockLayer2_U, 1.0f, 50.0f);
		mPostProcessData._BlockLayer2_V = Math::Clamp(_BlockLayer2_V, 1.0f, 50.0f);
		mPostProcessData._RGBSplit_Indensity = Math::Clamp(_RGBSplit_Indensity, 1.0f, 50.0f);
		mPostProcessData._BlockLayer1_Indensity = Math::Clamp(_BlockLayer1_Indensity, 1.0f, 50.0f);
		mPostProcessData._BlockLayer2_Indensity = Math::Clamp(_BlockLayer2_Indensity, 1.0f, 50.0f);
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

void BlockGlitchEffect::End()
{
	for (uint32_t i = 0; i < std::size(mTextures); ++i)
	{
		Texture::UnbindPS(i);

	}
}

void BlockGlitchEffect::Render(const RenderObject& renderObject)
{
	//mBaseRenderTarget.BindPS(0);
	//mVBlurRenderTarget.BindPS(1);
	renderObject.meshBuffer.Render();

}

void BlockGlitchEffect::SetTexture(const Texture* texture, uint32_t slot)
{
	ASSERT(slot < std::size(mTextures), "PostprocessingEffect -- Invalid slot index.");
	mTextures[slot] = texture;
}

void BlockGlitchEffect::SetMode(Mode mode)
{
	mPostProcessData.mode = static_cast<int>(mode);

}

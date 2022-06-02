#include "Precompiled.h"
#include "GaussianBlurEffect.h"
#include "Camera.h"
#include "GraphicsSystem.h"
#include "RenderObject.h"
#include "VertexTypes.h"


using namespace Storm;
using namespace Storm::Graphics;

void GaussianBlurEffect::Initialize()
{
	auto graphicsSystem = GraphicsSystem::Get();
	const auto screenWidth = graphicsSystem->GetBackBufferWidth();
	const auto screenHeight = graphicsSystem->GetBackBufferHeight();
	mHorizontalBlurRenderTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);
	mVerticalBlurRenderTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);
	mGaussianBlurVertexShader.Initialize<VertexPX>(L"../../Assets/Shaders/GaussianBlur.fx");
	mHorizontalBlurPixelShader.Initialize(L"../../Assets/Shaders/GaussianBlur.fx", "HorizontalBlurPS");
	mVerticalBlurPixelShader.Initialize(L"../../Assets/Shaders/GaussianBlur.fx", "VerticalBlurPS");

	mBlurSettingsBuffer.Initialize();

	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void GaussianBlurEffect::Terminate()
{
	mSampler.Terminate();
	mBlurSettingsBuffer.Terminate();

	mVerticalBlurPixelShader.Terminate();
	mHorizontalBlurPixelShader.Terminate();
	mGaussianBlurVertexShader.Terminate();

	mVerticalBlurRenderTarget.Terminate();
	mHorizontalBlurRenderTarget.Terminate();

}

void GaussianBlurEffect::Begin()
{
	mGaussianBlurVertexShader.Bind();
	auto graphicsSystem = GraphicsSystem::Get();
	const auto screenWidth = graphicsSystem->GetBackBufferWidth();
	const auto screenHeight = graphicsSystem->GetBackBufferHeight();

	BlurSettingsData blurSettings;
	blurSettings.screenWidth = static_cast<float>(screenWidth);
	blurSettings.screenHeight = static_cast<float>(screenHeight);
	blurSettings.multiplier = mBlurSaturation;
	mBlurSettingsBuffer.Update(blurSettings);
	mBlurSettingsBuffer.BindPS(0);


	mSampler.BindPS(0);

}

void GaussianBlurEffect::End()
{
	auto graphicsSystem = GraphicsSystem::Get();
	graphicsSystem->ResetRenderTarget();
	graphicsSystem->ResetViewport();
}

void GaussianBlurEffect::Render(const RenderObject& renderObject)
{
	auto graphicsSystem = GraphicsSystem::Get();
	mHorizontalBlurRenderTarget.BeginRender();
	mSourceTexture->BindPS(0);
	mHorizontalBlurPixelShader.Bind();
	renderObject.meshBuffer.Render();
	Texture::UnbindPS(0);
	mHorizontalBlurRenderTarget.EndRender();

	for (int i = 0; i < mBlurIterations; ++i)
	{

		mVerticalBlurRenderTarget.BeginRender();
		mHorizontalBlurRenderTarget.BindPS(0);
		mVerticalBlurPixelShader.Bind();
		renderObject.meshBuffer.Render();
		Texture::UnbindPS(0);
		mVerticalBlurRenderTarget.EndRender();

		mHorizontalBlurRenderTarget.BeginRender();
		mVerticalBlurRenderTarget.BindPS(0);
		mHorizontalBlurPixelShader.Bind();
		renderObject.meshBuffer.Render();
		Texture::UnbindPS(0);

		mHorizontalBlurRenderTarget.EndRender();

	}



	mVerticalBlurRenderTarget.BeginRender();
	mHorizontalBlurRenderTarget.BindPS(0);
	mVerticalBlurPixelShader.Bind();
	renderObject.meshBuffer.Render();
	Texture::UnbindPS(0);
	mVerticalBlurRenderTarget.EndRender();

}


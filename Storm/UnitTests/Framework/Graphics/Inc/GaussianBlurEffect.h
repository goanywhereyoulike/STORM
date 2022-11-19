#pragma once
#include "Effect.h"
#include "ConstantBuffer.h"
#include "LightTypes.h"
#include "Material.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "RenderTarget.h"
#include "Sampler.h"

namespace Storm::Graphics
{
	class Texture;
	class GaussianBlurEffect final : public Effect
	{
	public:


		void Initialize() override;
		void Terminate() override;

		void Begin() override;
		void End() override;
		void Render(const RenderObject& renderObject) override;

		void SetSourceTexture(const Texture* texture) { mSourceTexture = texture; }
	
		const Texture& GetHorizontalBlurTexture() const { return mHorizontalBlurRenderTarget; }
		const Texture& GetVerticalBlurTexture() const { return mVerticalBlurRenderTarget; }
		const Texture& GetResultTexture() const { return mVerticalBlurRenderTarget; }

		int& BlurIterations() { return mBlurIterations; }
		float& BlurSaturation() { return mBlurSaturation; }


	private:

		struct BlurSettingsData
		{
			float screenWidth;
			float screenHeight;
			float multiplier;
			float padding;


		};

		using BlurSettingsBuffer = TypedConstantBuffer<BlurSettingsData>;

		BlurSettingsBuffer mBlurSettingsBuffer;

		Storm::Graphics::VertexShader mGaussianBlurVertexShader;
		Storm::Graphics::PixelShader mHorizontalBlurPixelShader;
		Storm::Graphics::PixelShader mVerticalBlurPixelShader;

		Storm::Graphics::RenderTarget mHorizontalBlurRenderTarget;
		Storm::Graphics::RenderTarget mVerticalBlurRenderTarget;

		const Texture* mSourceTexture = nullptr;
		Sampler mSampler;

		int mBlurIterations = 1;
		float mBlurSaturation = 1.0f;
	};



}

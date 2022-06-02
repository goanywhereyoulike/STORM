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
	class BlockGlitchEffect final : public Effect
	{
	public:
		enum Mode
		{
			Bloom,
			Glitch
		};



		void Initialize() override;
		void Terminate() override;

		void Begin() override;
		void End() override;
		void Render(const RenderObject& renderObject) override;

		void SetTexture(const Texture* texture, uint32_t slot);
		void SetMode(Mode mode);

		const Texture& GetBaseTexture() const { return mBaseRenderTarget; }

		bool& UseGlitchEffect() { return mUseGlitchEffect; }
		float& Block_size() { return block_size; }
		float& Speed() { return speed; }
		float& Fade() { return fade; }
		float& Offset() { return offset; }
		float& BlockLayer1_U() { return _BlockLayer1_U; }
		float& BlockLayer1_V() { return _BlockLayer1_V; }
		float& BlockLayer2_U() { return _BlockLayer2_U; }
		float& BlockLayer2_V() { return _BlockLayer2_V; }
		float& RGBSplit_Indensity() { return _RGBSplit_Indensity; }
		float& BlockLayer1_Indensity() { return _BlockLayer1_Indensity; }
		float& BlockLayer2_Indensity() { return _BlockLayer2_Indensity; }



	private:
		struct PostProcessData
		{
			int mode = 0;
			int useGlitchEffect = 0;
			float time = 0;
			float block_size = 0;
			float speed = 0;
			float fade = 0;
			float offset = 0;
			float _BlockLayer1_U = 0;
			float _BlockLayer1_V = 0;
			float _BlockLayer2_U = 0;
			float _BlockLayer2_V = 0;
			float _RGBSplit_Indensity = 0;
			float _BlockLayer1_Indensity = 0;
			float _BlockLayer2_Indensity = 0;
			int padding[2];

		};

		using PostProcesssBuffer = Storm::Graphics::TypedConstantBuffer<PostProcessData>;

		PostProcesssBuffer mPostProcessBuffer;
		PostProcessData mPostProcessData;

		Storm::Graphics::RenderTarget mBaseRenderTarget;

		Storm::Graphics::VertexShader mPostProcessVertexShader;
		Storm::Graphics::PixelShader mPostProcessPixelShader;

		Sampler mSampler;
		std::array<const Texture*, 2> mTextures = {};

		bool mUseGlitchEffect = 0;
		float block_size = 0;
		float speed = 0;
		float fade = 0;
		float offset = 0;
		float _BlockLayer1_U = 0;
		float _BlockLayer1_V = 0;
		float _BlockLayer2_U = 0;
		float _BlockLayer2_V = 0;
		float _RGBSplit_Indensity = 0;
		float _BlockLayer1_Indensity = 0;
		float _BlockLayer2_Indensity = 0;


	};



}

#pragma once
#include "Effect.h"
#include "ConstantBuffer.h"
#include "LightTypes.h"
#include "Material.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Sampler.h"

namespace Storm::Graphics
{
	class Camera;
	class Terrain;
	class Texture;
	class TerrainEffect final : public Effect
	{
	public:

		void Initialize() override;
		void Terminate() override;

		void Begin() override;
		void End() override;
		void Render(const RenderObject& renderObject) override;

		void SetCamera(const Camera& camera);
		void SetLightCamera(const Camera* camera);
		void SetDirectionalLight(const DirectionalLight& mDirectionalLight);
		void SetShadowMap(const Texture* shadowMap);

		bool& UseShadowFlag() { return mUseShadowMap; }
		float& DepthBias() { return mDepthBias; }


	private:
		struct TransformData
		{
			Storm::Math::Matrix4 world;
			Storm::Math::Matrix4 wvp[2];
			Storm::Math::Vector3 viewPosition;
			float padding;

		};

		struct SettingsData
		{
			int useShadowMap = 0;
			float depthBias = 0.0f;
			float padding[2] = {};
		};



		using TransformBuffer = Storm::Graphics::TypedConstantBuffer<TransformData>;
		using LightBuffer = Storm::Graphics::TypedConstantBuffer<Storm::Graphics::DirectionalLight>;
		using MaterialBuffer = Storm::Graphics::TypedConstantBuffer<Storm::Graphics::Material>;
		using SettingsBuffer = Storm::Graphics::TypedConstantBuffer<SettingsData>;

		const Camera* mCamera = nullptr;
		const Camera* mLightCamera = nullptr;
		const DirectionalLight* mDirectionalLight = nullptr;
		const Texture* mShadowMap = nullptr;

		//const Animator* animator = nullptr;

		TransformBuffer mTransformBuffer;
		LightBuffer mLightBuffer;
		MaterialBuffer mMaterialBuffer;
		SettingsBuffer mSettingsBuffer;

		VertexShader mVertexShader;
		PixelShader mPixelShader;

		Sampler mSampler;

		bool mUseShadowMap = false;
		float mDepthBias = 0.0f;

	};



}

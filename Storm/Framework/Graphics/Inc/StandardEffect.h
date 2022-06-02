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
	class Texture;
	class StandardEffect final : public Effect
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

		bool& UseSpecularFlag() { return mUseSpecularFlag; }
		bool& UseDisplacementFlag() { return mUseDisplacementFlag; }
		bool& UseNorFlag() { return mUseNorFlag; }
		bool& UseShadowFlag() { return mUseShadowMap; }
		float& DisplacementWeight() { return mDisplacementWeight; }
		float& DepthBias() { return mDepthBias; }

	private:
		struct TransformData
		{
			Storm::Math::Matrix4 world;
			Storm::Math::Matrix4 wvp[2];
			Storm::Math::Vector3 viewPosition;
			float padding;

		};

		struct BoneTransformData
		{
			static constexpr size_t MaxBoneCount = 128;
			Math::Matrix4 boneTransforms[MaxBoneCount];
		};
		struct SettingsData
		{
			int useSpecularMap = 1;
			int useDisplacementMap = 1;
			float displacementWeight = 0.3f;
			int useNormalMap = 1;

			int useShadowMap = 0;
			int useSkinning = 0;
			float depthBias = 0.0f;
			float padding[1] = {};
		};



		using TransformBuffer = Storm::Graphics::TypedConstantBuffer<TransformData>;
		using LightBuffer = Storm::Graphics::TypedConstantBuffer<Storm::Graphics::DirectionalLight>;
		using MaterialBuffer = Storm::Graphics::TypedConstantBuffer<Storm::Graphics::Material>;
		using SettingsBuffer = Storm::Graphics::TypedConstantBuffer<SettingsData>;
		using BoneTransformBuffer = Storm::Graphics::TypedConstantBuffer<BoneTransformData>;

		const Camera* mCamera = nullptr;
		const Camera* mLightCamera = nullptr;
		const DirectionalLight* mDirectionalLight = nullptr;
		const Texture* mShadowMap = nullptr;
		//const Animator* animator = nullptr;

		TransformBuffer mTransformBuffer;
		LightBuffer mLightBuffer;
		MaterialBuffer mMaterialBuffer;
		SettingsBuffer mSettingsBuffer;
		BoneTransformBuffer mBoneTransformBuffer;

		VertexShader mVertexShader;
		PixelShader mPixelShader;

		Sampler mSampler;
		bool mUseSpecularFlag = false;
		bool mUseDisplacementFlag = false;
		bool mUseNorFlag = false;
		bool mUseShadowMap = false;
		float mDisplacementWeight = 0.0f;
		float mDepthBias = 0.00001f;
	
	};



}
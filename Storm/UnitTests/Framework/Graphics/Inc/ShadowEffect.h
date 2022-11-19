#pragma once
#include "ConstantBuffer.h"
#include "Effect.h"
#include "Camera.h"
#include "LightTypes.h"
#include "RenderTarget.h"
#include "VertexShader.h"
#include "PixelShader.h"
namespace Storm::Graphics
{
	class ShadowEffect final :public Effect
	{
	public:
		void Initialize() override;
		void Terminate() override;

		void Begin() override;
		void End() override;
		void Render(const RenderObject& renderObject) override;

		void SetDirectionalLight(const DirectionalLight& directionalLight);
		const Camera& GetLightCamera()const { return mLightCamera; }
		const Texture& GetDepthMap() const { return mDepthMapRenderTarget; }

		void SetSize(float size);
		void SetFocus(Math::Vector3 position);
	private:

		void UpdateLightCamera();


		struct TransformData
		{
			Storm::Math::Matrix4 wvp;

		};

		struct BoneTransformData
		{
			static constexpr size_t MaxBoneCount = 128;
			Math::Matrix4 boneTransforms[MaxBoneCount];
		};

		struct SettingsData
		{
			int useSkinning = 0;
			int padding[3] = {};
		};

		using TransformBuffer = Storm::Graphics::TypedConstantBuffer<TransformData>;
		using BoneTransformBuffer = Storm::Graphics::TypedConstantBuffer<BoneTransformData>;
		using SettingsBuffer = Storm::Graphics::TypedConstantBuffer<SettingsData>;

		Camera mLightCamera;
		TransformBuffer mTransformBuffer;
		BoneTransformBuffer mBoneTransformBuffer;
		SettingsBuffer mSettingsBuffer;

		const DirectionalLight* mDirectionalLight = nullptr;

		VertexShader mVertexShader;
		PixelShader mPixelShader;

		RenderTarget mDepthMapRenderTarget;
	};

}
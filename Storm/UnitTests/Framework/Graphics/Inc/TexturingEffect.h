#pragma once
#include "Effect.h"
#include "ConstantBuffer.h"
#include "LightTypes.h"
#include "Material.h"
#include "BlendState.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Sampler.h"

namespace Storm::Graphics
{
	class Camera;
	class TexturingEffect final : public Effect
	{
	public:
		void Initialize() override;
		void Terminate() override;

		void Begin() override;
		void End() override;
		void Render(const RenderObject& renderObject) override;

		void SetCamera(const Camera& camera);


	private:

		using TransformBuffer = Storm::Graphics::TypedConstantBuffer<Math::Matrix4>;
		//using MaterialBuffer = Storm::Graphics::TypedConstantBuffer<Storm::Graphics::Material>;

		const Camera* mCamera = nullptr;


		TransformBuffer mTransformBuffer;

		//MaterialBuffer mMaterialBuffer;
		VertexShader mTexturingVertexShader;
		PixelShader mTexturingPixelShader;

		BlendState mBlendstate;
		Sampler mSampler;

	};



}
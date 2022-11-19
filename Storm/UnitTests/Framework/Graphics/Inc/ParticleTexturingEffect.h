#pragma once

#include "BlendState.h"
#include "ConstantBuffer.h"
#include "DepthStencilState.h"
#include "Effect.h"
#include "LightTypes.h"
#include "Material.h"
#include "Particle.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "VertexShader.h"

namespace Storm::Graphics
{
	class Camera;
	class ParticleTexturingEffect final : public Effect
	{
	public:
		void Initialize() override;
		void Terminate() override;

		void Begin() override;
		void End() override;
		void Render(const RenderObject& renderObject) override;
		void RenderParticle(const Particle& particle);

		void SetCamera(const Camera& camera);


	private:

		struct ParticleData
		{
			Storm::Math::Matrix4 wvp;
			Storm::Graphics::Color color;
			float padding[4] = {};

		};

		using ParticleBuffer = Storm::Graphics::TypedConstantBuffer<ParticleData>;
		using TransformBuffer= Storm::Graphics::TypedConstantBuffer<Math::Matrix4>;
		//using MaterialBuffer = Storm::Graphics::TypedConstantBuffer<Storm::Graphics::Material>;

		const Camera* mCamera = nullptr;

		TransformBuffer mTransformBuffer;
		ParticleBuffer mParticleBuffer;

		//MaterialBuffer mMaterialBuffer;
		VertexShader mTexturingVertexShader;
		PixelShader mTexturingPixelShader;

		BlendState mBlendstate;
		DepthStencilState mDepthStencilState;
		Sampler mSampler;

		MeshBuffer particleMeshBuffer;

	};



}
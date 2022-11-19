#pragma once
#include "ConstantBuffer.h"
#include "LightTypes.h"
#include "Material.h"
#include "BlendState.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "Particle.h"
namespace Storm::Graphics
{
	class Camera;
	class SimpleEffect
	{
	public:
		void Initialize();
		void Terminate();

		void Begin();
		void End();
		void Render(Particle& renderObject);

		void SetCamera(const Camera& camera);


	private:

		struct NormalData
		{
			Storm::Math::Matrix4 wvp;

			Storm::Graphics::Color color;
			float size;
			float padding[3] = {};

		};
		using NormalBuffer = Storm::Graphics::TypedConstantBuffer<NormalData>;
		const Camera* mCamera = nullptr;

		NormalBuffer mNormalBuffer;

		//MaterialBuffer mMaterialBuffer;
		VertexShader mVertexShader;
		PixelShader mPixelShader;

		BlendState mBlendstate;
		Sampler mSampler;

		MeshBuffer particleMeshBuffer;
	};
}
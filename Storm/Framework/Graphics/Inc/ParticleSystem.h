#pragma once
#include "Common.h"
#include "Camera.h"
#include "Colors.h"
#include "MeshTypes.h"
#include "Particle.h"
#include "ParticleTexturingEffect.h"
#include "PixelShader.h"
#include "Random.h"
#include "Texture.h"
#include "VertexShader.h"
#include "SimpleEffect.h"


struct ParticleProps
{

	Storm::Math::Vector3 position;
	Storm::Math::Vector3 velocity;
	Storm::Math::Vector3 velocityvariation{ 1.0f,1.0f,1.0f };
	float lifetime = 1.0f;
	float size = 1.0f;
	int number = 5000;
	Storm::Math::Vector3 mEmitPosOffset;
	Storm::Graphics::Color color = Storm::Graphics::Colors::White;
	Storm::Graphics::Color startColor = Storm::Graphics::Colors::White;
	Storm::Graphics::Color endColor = Storm::Graphics::Colors::White;


};


namespace Storm::Graphics
{
	class ParticleSystem
	{
	public:
		void Initialize();
		void Render();
		void Terminate();
		void Emit(ParticleProps& particlepros);
		void SetCamera(const Camera& camera);
		void UpdateParticles(float deltaTime);
		const Storm::Graphics::Texture* GetParticleTexture();

		void SaveParticleProps(std::filesystem::path filePath, ParticleProps ParticleProp);
		void LoadParticleProps(std::filesystem::path filePath, ParticleProps& ParticleProp);
	private:
		std::vector<Particle> particlepool;
		Math::Vector3 origin;
		uint32_t poolIndex = 999;

		const Camera* mCamera = nullptr;
		SimpleEffect  mSimpleEffect;
		ParticleTexturingEffect mParticleTexturingEffect;
	
		Particle particle;
	};
}

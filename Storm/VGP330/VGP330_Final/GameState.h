#pragma once
#include <Storm/Inc/Storm.h>
class GameState :public Storm::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;
private:

	Storm::Graphics::Camera mCamera;

	//Storm::Graphics::SimpleEffect mSimpleEffect;

	//Storm::Graphics::Particle mParticle;
	
	//Storm::Graphics::RenderObject mAllParticles[10];
	ParticleProps particlepros;
	Storm::Graphics::ParticleSystem particlesytem;
	Storm::Math::Vector3 mPosition = Storm::Math::Vector3::Zero;
	Storm::Math::Vector3 mVelocity = Storm::Math::Vector3::Zero;
	Storm::Math::Vector3 mAcceleration = Storm::Math::Vector3::Zero;

	float mFPS = 0.0f;
	float mTime = 0.0f;
	float mHover = 0.0f;
	float lifespan = 255.0f;
	float WaitShoot = 0.0f;

	bool mPaused = false;
};
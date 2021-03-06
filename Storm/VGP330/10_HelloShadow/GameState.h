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

	Storm::Graphics::StandardEffect mStandardEffect;
	Storm::Graphics::ShadowEffect mShadowEffect;
	Storm::Graphics::TexturingEffect mTexturingEffect;


	Storm::Graphics::RenderObject mSpaceShip;
	Storm::Graphics::RenderObject mGround;
	Storm::Graphics::RenderObject mSkydome;
	Storm::Graphics::RenderObject mEarth;
	Storm::Graphics::RenderObject mEarthCloud;

	Storm::Graphics::DirectionalLight mDirectionLight;

	Storm::Math::Vector3 mPosition = Storm::Math::Vector3::Zero;
	Storm::Math::Vector3 mRotation = Storm::Math::Vector3::Zero;

	float mFPS = 0.0f;
	float mHover = 0.0f;
	float mCloudRotation = 0.0f;

};
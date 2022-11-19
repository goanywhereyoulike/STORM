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

	Storm::Graphics::Model model;
	Storm::Graphics::Animator modelAnimator;
	//Storm::Graphics::AnimationBlender modelAnimator;


	Storm::Graphics::RenderGroup mHuman;
	//Storm::Graphics::RenderObject mSpaceShip;
	Storm::Graphics::RenderObject mGround;
	Storm::Graphics::RenderObject mSkydome;
	Storm::Graphics::DirectionalLight mDirectionLight;

	Storm::Math::Vector3 mPosition = Storm::Math::Vector3::Zero;
	Storm::Math::Vector3 mRotation = Storm::Math::Vector3::Zero;

	Storm::Graphics::Animation mShipAnimation;


	Storm::Graphics::RasterizerState mCullNone;
	Storm::Graphics::RasterizerState mWireframe;

	bool CullNone = false;
	bool Wireframe = false;


	float mFPS = 0.0f;
	float mHover = 0.0f;
	float timer = 0.0f;
	float animationspeed = 1.0f;

	float mSpeed = 0.0f;

	bool mDrawSkeleton = false;
	int mDepth = 0;




};
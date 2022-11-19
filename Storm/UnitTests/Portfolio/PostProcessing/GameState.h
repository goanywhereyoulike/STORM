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

	//Storm::Graphics::Camera mCamera;
	Storm::GameWorld mGameWorld;
	std::vector<Storm::GameObjectHandle> mGameObjectHandles;
	Storm::GameObjectHandle mGameObjectHandle;

	Storm::Graphics::Model model;
	Storm::Graphics::Animator modelAnimator;
	Storm::Graphics::RenderGroup mHuman;

	Storm::Graphics::Camera mCamera;

	Storm::Graphics::StandardEffect mStandardEffect;
	Storm::Graphics::ShadowEffect mShadowEffect;

	Storm::Math::Vector3 mPosition = Storm::Math::Vector3::Zero;
	float mScale = 1.0f;

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

	//float mFPS = 0.0f;


};
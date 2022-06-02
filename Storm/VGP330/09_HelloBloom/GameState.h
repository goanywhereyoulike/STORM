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

	void RenderScene();
	//void RenderBrightness();
	//void ApplyBlur();
	//void PostProcess();


	Storm::Graphics::Camera mCamera;

	Storm::Graphics::RenderTarget mBaseRenderTarget;
	Storm::Graphics::RenderTarget mBloomRenderTarget;

	Storm::Graphics::StandardEffect mStandardEffect;
	Storm::Graphics::TexturingEffect mTexturingEffect;
	Storm::Graphics::GaussianBlurEffect mGaussianBlurEffect;
	Storm::Graphics::PostProcessEffect mPostProcessEffect;

	Storm::Graphics::RenderObject mEarth; 
	Storm::Graphics::RenderObject mEarthNoTexture;
	Storm::Graphics::RenderObject mEarthCloud;
	Storm::Graphics::RenderObject mSun;
	Storm::Graphics::RenderObject mScreen;
	Storm::Graphics::RenderObject mSpaceShip;
	Storm::Graphics::RenderObject mSpaceShipNoTexture;

	Storm::Graphics::DirectionalLight mDirectionLight;


	float mFPS = 0.0f;
	float mCloudRoation = 0.0f;

	Storm::Math::Vector3 mPosition = Storm::Math::Vector3::Zero;
	Storm::Math::Vector3 mRotation = Storm::Math::Vector3::Zero;
	Storm::Math::Vector3 mCloudsRotation = Storm::Math::Vector3::Zero;
	Storm::Math::Vector3 mRevolution = Storm::Math::Vector3::Zero;




};
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

	Storm::Graphics::RenderTarget mBaseRenderTarget;
	Storm::Graphics::RenderTarget mNoPostProcessEffectRenderTarget;

	Storm::Graphics::StandardEffect mStandardEffect;
	Storm::Graphics::TexturingEffect mTexturingEffect;
	Storm::Graphics::PostProcessEffect mPostProcessEffect;
	Storm::Graphics::BlockGlitchEffect mBlockGlitchEffect;

	Storm::Graphics::MeshPX mScreenMesh;
	Storm::Graphics::MeshBuffer mScreenMeshBuffer;

	Storm::Graphics::RenderObject mEarth;
	Storm::Graphics::RenderObject mEarthCloud;
	Storm::Graphics::RenderObject mScreen;
	Storm::Graphics::RenderGroup mCity;
	Storm::Graphics::RenderObject mSkydome;

	Storm::Graphics::DirectionalLight mDirectionLight;

	float mFPS = 0.0f;
	float mCloudRotation = 0.0f;
	bool UsePostprocessing = false;

	Storm::Math::Vector3 mPosition = Storm::Math::Vector3::Zero;
	Storm::Math::Vector3 mRotation = Storm::Math::Vector3::Zero;
	//Storm::Math::Vector3 mCloudsRotation = Storm::Math::Vector3::Zero;

};
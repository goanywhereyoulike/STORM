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
	Storm::Physics::PhysicsWorld mPhysicsWorld;


	Storm::Graphics::StandardEffect mStandardEffect;
	Storm::Graphics::ShadowEffect mShadowEffect;
	Storm::Graphics::TexturingEffect mTextureEffect;


	Storm::Graphics::DirectionalLight mDirectionLight;
	Storm::Graphics::Model model;
	Storm::Graphics::RenderGroup mHuman;
	Storm::Graphics::RenderObject mGround;
	Storm::Graphics::RenderObject Cloth;

	Storm::Graphics::RasterizerState mCullNone;
	Storm::Graphics::RasterizerState mWireframe;

	std::vector<Storm::Physics::Tetrahedron> mTetrahedrons;

	Storm::Graphics::Mesh mMesh;

	float mFPS = 0.0f;
	float speed = 1.0f;
	bool mDrawModel = false;
	bool CullNone = false;
	bool Wireframe = false;

};
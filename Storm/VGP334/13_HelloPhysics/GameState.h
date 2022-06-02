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
	void subdivide(Storm::Math::Vector3& v1, Storm::Math::Vector3& v2, Storm::Math::Vector3& v3, long depth, std::vector< Storm::Math::Vector3>& indices);
	Storm::Graphics::Camera mCamera;
	Storm::Physics::PhysicsWorld mPhysicsWorld;


	Storm::Graphics::StandardEffect mStandardEffect;
	Storm::Graphics::ShadowEffect mShadowEffect;
	Storm::Graphics::TexturingEffect mTexturingEffect;

	Storm::Graphics::RenderObject mSkydome;

	//Cloth
	Storm::Graphics::MeshPX mPlaneMesh;
	Storm::Graphics::MeshBuffer mPlaneMeshBuffer;
	Storm::Graphics::Texture mPlaneTexture;
	Storm::Graphics::Sampler mSampler;
	Storm::Graphics::PixelShader mPixelShader;
	Storm::Graphics::VertexShader mVertexShader;


	Storm::Graphics::DirectionalLight mDirectionLight;
	Storm::Graphics::Model model;
	Storm::Graphics::RenderGroup mHuman;

	Storm::Graphics::RasterizerState mCullNone;
	Storm::Graphics::RasterizerState mWireframe;

	std::vector<Storm::Physics::Tetrahedron> mTetrahedrons;
	std::vector<Storm::Physics::Particle*> mBalls;
	std::vector<Storm::Physics::Particle*> mParticles;
	std::vector<Storm::Math::Vector3> indices;


	float mFPS = 0.0f;
	float speed = 1.0f;
	bool mDrawModel = false;
	bool CullNone = false;
	bool Wireframe = false;
	int depth = 0;

};
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

	struct TransformData
	{
		Storm::Math::Matrix4 wvp;
	};

	Storm::Graphics::Camera mCamera;
	Storm::Graphics::MeshPX mCubeMesh;
	Storm::Graphics::MeshPX mSkyboxMesh;
	Storm::Graphics::MeshPX mPlaneMesh;
	Storm::Graphics::MeshPX mCylinderMesh;
	Storm::Graphics::MeshPX mConeMesh;
	Storm::Graphics::MeshPX mSphereMesh;
	Storm::Graphics::MeshPX mSkydomeMesh;
	Storm::Graphics::MeshPX mCirclePlaneMesh;

	Storm::Graphics::TypedConstantBuffer<TransformData> mConstantBuffer;
	Storm::Graphics::MeshBuffer mCubeMeshBuffer;
	Storm::Graphics::MeshBuffer mSkyboxMeshBuffer;
	Storm::Graphics::MeshBuffer mPlaneMeshBuffer;
	Storm::Graphics::MeshBuffer mCylinderMeshBuffer;
	Storm::Graphics::MeshBuffer mConeMeshBuffer;
	Storm::Graphics::MeshBuffer mSphereMeshBuffer;
	Storm::Graphics::MeshBuffer mSkydomeMeshBuffer;
	Storm::Graphics::MeshBuffer mCirclePlaneMeshBuffer;;

	Storm::Graphics::PixelShader mPixelShader;
	Storm::Graphics::VertexShader mVertexShader;

	Storm::Graphics::Texture mCubeTexture;
	Storm::Graphics::Texture mSkyboxTexture;
	Storm::Graphics::Texture mPlaneTexture;
	Storm::Graphics::Texture mCylinderTexture;
	Storm::Graphics::Texture mConeTexture;
	Storm::Graphics::Texture mSphereTexture;
	Storm::Graphics::Texture mSkydomeTexture;
	Storm::Graphics::Texture mCirclePlaneTexture;
	Storm::Graphics::Sampler mSampler;

	bool DrawBox = false;
	bool DrawSkyBox = false;
	bool DrawPlane = false;
	bool DrawCylinder = false;
	bool DrawCircleplane = false;
	bool DrawCone = false;
	bool DrawSphere = false;
	bool DrawSkydome = false;




	float posZ = 5.0f;
	float angleY = 0.0f;
	//float posZ = 5.0f;
	float angleX = 0.0f;

};
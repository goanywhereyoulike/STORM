#pragma once
#include <Storm/Inc/Storm.h>
class GameState :public Storm::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;

private:
	//struct Vertex
	//{
	//	Storm::Math::Vector3 position;
	//	Storm::Graphics::Color color;

	//};

	struct TransformData
	{
		Storm::Math::Matrix4 wvp;


	};

	//ID3D11Buffer* mConstantBuffer = nullptr;
	Storm::Graphics::Camera mCamera;
	Storm::Graphics::MeshPC mMesh;

	Storm::Graphics::TypedConstantBuffer<TransformData> mConstantBuffer;
	Storm::Graphics::MeshBuffer mMeshBuffer;
	Storm::Graphics::PixelShader mPixelShader;
	Storm::Graphics::VertexShader mVertexShader;

	float posZ = 5.0f;
	float angleY = 0.0f;
	//float posZ = 5.0f;
	float angleX = 0.0f;

};
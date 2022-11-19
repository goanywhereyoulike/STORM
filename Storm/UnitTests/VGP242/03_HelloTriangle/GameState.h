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
	struct Vertex
	{
		Storm::Math::Vector3 position;
		Storm::Graphics::Color color;

	};

	std::vector<Vertex> mVertices;

	//ID3D11Buffer* mVertexBuffer = nullptr;
	//ID3D11VertexShader* mVertexShader = nullptr;
	//ID3D11InputLayout* mInputLayout = nullptr;

	Storm::Graphics::MeshBuffer mVertexBuffer;
	Storm::Graphics::PixelShader mPixelShader;
	Storm::Graphics::VertexShader mVertexShader;

};
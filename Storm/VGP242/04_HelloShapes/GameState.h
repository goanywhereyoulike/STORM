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

	int select = 0;
	std::vector<Vertex> mVertices[3];

	Storm::Graphics::MeshBuffer mVertexBuffer[3];
	Storm::Graphics::PixelShader mPixelShader;
	Storm::Graphics::VertexShader mVertexShader;


};

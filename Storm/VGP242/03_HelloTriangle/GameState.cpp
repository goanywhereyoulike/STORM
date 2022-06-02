#include "GameState.h"

using namespace Storm::Graphics;
using namespace Storm::Math;
void GameState::Initialize()
{

	// NDC - Normalized Device Coordinate
//
//          +-----------------+
//         /                 /|
//   1.0  +-----------------+ |
//	      |        ^        | |
//	      |        |        | |
//	      | <------+------> | |
//	      |        |        | | 1.0
//	      |        v        |/
//	-1.0  +-----------------+ 0.0
//      -1.0               1.0
//
	mVertices.push_back({ Vector3{  0.0f,  0.5f, 0.0f },Colors::Yellow });
	mVertices.push_back({ Vector3{  0.5f, -0.5f, 0.0f },Colors::Yellow });
	mVertices.push_back({ Vector3{ -0.5f, -0.5f, 0.0f } ,Colors::Yellow });

	// -------------------------------------------
	// Create vertex buffer
	mVertexBuffer.Initialize(mVertices.data(), static_cast<uint32_t>(sizeof(Vertex)), static_cast<uint32_t>(mVertices.size()));
	// -------------------------------------------


	// -------------------------------------------
	// Compile and create vertex shader
	mVertexShader.Initialize<VertexPC>(L"../../Assets/Shaders/DoSomething.fx");
	// -------------------------------------------

	// -------------------------------------------
	// Compile and create pixel shader
	mPixelShader.Initialize(L"../../Assets/Shaders/DoSomething.fx");


	// -------------------------------------------


}

void GameState::Terminate()
{
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mVertexBuffer.Terminate();

}

void GameState::Update(float deltaTime)
{


}
void GameState::Render()
{
	mVertexShader.Bind();
	mPixelShader.Bind();
	mVertexBuffer.Render();
}

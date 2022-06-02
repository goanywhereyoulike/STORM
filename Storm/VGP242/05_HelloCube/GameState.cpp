#include "GameState.h"

using namespace Storm::Graphics;
using namespace Storm::Input;
using namespace Storm::Math;

void GameState::Initialize()
{
	mMesh = MeshBuilder::CreateCubePC();


	// Create vertex buffer
	mMeshBuffer.Initialize(mMesh);
	// Create constant buffer
	mConstantBuffer.Initialize();



	// Compile and create vertex shader
	mVertexShader.Initialize<VertexPC>(L"../../Assets/Shaders/DoTransform.fx");

	// Compile and create pixel shader
	mPixelShader.Initialize(L"../../Assets/Shaders/DoTransform.fx");

}

void GameState::Terminate()
{
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mConstantBuffer.Terminate();
	mMeshBuffer.Terminate();


}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyDown(KeyCode::LEFT))
		angleY += deltaTime;
	if (inputSystem->IsKeyDown(KeyCode::RIGHT))
		angleY -= deltaTime;
	if (inputSystem->IsKeyDown(KeyCode::UP))
		angleX -= deltaTime;
	if (inputSystem->IsKeyDown(KeyCode::DOWN))
		angleX += deltaTime;
	//Camera control---------------------------------
	const float movespeed = 10.0f;
	const float turnspeed = 0.3f;
	if (inputSystem->IsKeyDown(KeyCode::W))
	{

		mCamera.Walk(movespeed * deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::A))
	{
		mCamera.Strafe(-movespeed * deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::S))
	{
		mCamera.Walk(-movespeed * deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::D))
	{
		mCamera.Strafe(movespeed * deltaTime);
	}

	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{

		float distanceX = static_cast<float>(inputSystem->GetMouseMoveX());
		mCamera.Yaw(distanceX * turnspeed * deltaTime);

		float distanceY = static_cast<float>(inputSystem->GetMouseMoveY());
		mCamera.Pitch(distanceY * turnspeed * deltaTime);

	}



}
void GameState::Render()
{
	Matrix4 world = Matrix4::RotationX(angleX) * Matrix4::RotationY(angleY) * Matrix4::Translation({ 0, 0, posZ });
	Matrix4 view = mCamera.GetViewMatrix();
	Matrix4 proj = mCamera.GetProjectionMatrix();
	//Matrix4 wvp = Transpose(world * view * proj); // DX always want the transpose

	TransformData data;
	data.wvp = Transpose(world * view * proj);
	mConstantBuffer.Update(data);
	mConstantBuffer.BindVS(0);
	mVertexShader.Bind();
	mPixelShader.Bind();
	mMeshBuffer.Render();
}

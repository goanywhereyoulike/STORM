#include "GameState.h"

using namespace Storm::Graphics;
using namespace Storm::Input;
using namespace Storm::Math;

void GameState::Initialize()
{
	mCamera.SetPosition(Vector3{ 0.0f,5.0f,0.0f });
	mCamera.SetDirection(Vector3{ 0.0f,-1.0f,1.0f });

}

void GameState::Terminate()
{

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
	//SimpleDraw::AddLine(Vector3{0.0f,0.0f,0.0 }, Vector3{ 10.0f,10.0f,0.0f }, Colors::White);
	//SimpleDraw::AddAABB(Vector3{ 3.0f,0.0f,3.0f }, Vector3{ 2.0f,2.0f,2.0f }, Colors::White);
	SimpleDraw::AddCapsule(Vector3{ 5.0f,3.0f,3.0f }, 1.0f, 10.0f, Colors::White);
	SimpleDraw::AddGroundPlane(20.0f, Colors::White);
	SimpleDraw::AddTransform(Matrix4{ 1.0f,0.0f,0.0f,0.0f,
									 0.0f,1.0f,0.0f,0.0f,
									 0.0f,0.0f,1.0f,0.0f,
									 0.0f,0.0f,0.0f,1.0f });
	//SimpleDraw::AddCylinder(Vector3{ 3.0f,2.0f,3.0f }, 1.0f, 3.0f, Colors::White);
	//SimpleDraw::AddSphere(Vector3{ 4.0f,5.0f,6.0f }, 2.0f, Colors::White);

	SimpleDraw::Render(mCamera);

}

void GameState::DebugUI()
{
	ImGui::ShowDemoWindow();
}

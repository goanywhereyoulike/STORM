#include "PlayerControllerComponent.h"
//#include <Storm/Inc/TransformComponent.h>
//#include <Storm/Inc/AnimatorComponent.h>

using namespace Storm;
using namespace Storm::Input;
using namespace Storm::Math;
//
MEMORY_POOL_DEFINE(PlayerControllerComponent, 1000)

void PlayerControllerComponent::Initialize()
{
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
	mRigidBodyComponent = GetOwner().GetComponent<RigidBodyComponent>();
	//mAnimatorComponent = GetOwner().GetComponent<AnimatorComponent>();
}

void PlayerControllerComponent::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	const float moveSpeed = inputSystem->IsKeyDown(KeyCode::LSHIFT) ? mRunSpeed : mWalkSpeed;

	const float turnSpeed = 0.01f;
	accleration = Vector3::Zero;
	Math::Quaternion rotation = Math::Quaternion::Identity;
	Matrix4 transform = mTransformComponent->GetMatrix4();
	if (inputSystem->IsKeyDown(KeyCode::UP))
	{
		accleration = GetLook(transform) * moveSpeed;
		//velocity = GetLook(transform) * moveSpeed;
	}
	if (inputSystem->IsKeyDown(KeyCode::DOWN))
	{
		accleration = GetLook(transform) * -moveSpeed;

	}
	if (inputSystem->IsKeyDown(KeyCode::RIGHT))
	{
		accleration += GetRight(transform) * turnSpeed * 2.0f;
		mTurnAngle += turnSpeed;
	}
	if (inputSystem->IsKeyDown(KeyCode::LEFT))
	{
		accleration += GetRight(transform) * -turnSpeed * 2.0f;
		mTurnAngle -= turnSpeed;
	}
	//mRigidBodyComponent->SetForce(accleration);
	Vector3 position = GetTranslation(transform);
	//accleration -= velocity;
	velocity += accleration * deltaTime - velocity * 0.02f;

	position += velocity * deltaTime;
	// Snap to terrain
	auto terrainService = GetOwner().GetWorld().GetService<TerrainService>();
	float height = terrainService->GetHeight(position) + 2.0f;
	auto newPosition = Math::Vector3{ position.x, height, position.z };

	mTransformComponent->position = newPosition;
	const Vector3 offsets[] =
	{
		{-1.0f,0.0f,-2.0f},//left back
		{-1.0f,0.0f,+2.0f},//left front
		{+1.0f,0.0f,+2.0f},//right front
		{+1.0f,0.0f,-2.0f},//right back

	};
	Math::Matrix4 turnMatrix = Math::Matrix4::RotationY(mTurnAngle);
	Vector3 cornerLB = newPosition + TransformCoord(offsets[0], turnMatrix);
	Vector3 cornerLF = newPosition + TransformCoord(offsets[1], turnMatrix);
	Vector3 cornerRF = newPosition + TransformCoord(offsets[2], turnMatrix);
	Vector3 cornerRB = newPosition + TransformCoord(offsets[3], turnMatrix);
	cornerLB.y = terrainService->GetHeight(cornerLB);
	cornerLF.y = terrainService->GetHeight(cornerLF);
	cornerRF.y = terrainService->GetHeight(cornerRF);
	cornerRB.y = terrainService->GetHeight(cornerRB);

	Vector3 side = cornerRB - cornerLB;
	Vector3 look = (cornerLF + cornerRF) * 0.5 - (cornerRB + cornerLB) * 0.5f;
	Vector3 up = Cross(look, side);

	// Rotation
	//auto mouseX = inputSystem->GetMouseMoveX() * deltaTime;
	rotation = Quaternion::RotationEuler({ 0.0f, mTurnAngle, 0.0f });
	mTransformComponent->rotation = Quaternion::RotationLook(look, up);
}

void PlayerControllerComponent::DebugUI()
{

}

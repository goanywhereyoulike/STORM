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
	//mAnimatorComponent = GetOwner().GetComponent<AnimatorComponent>();
}

void PlayerControllerComponent::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	const float moveSpeed = inputSystem->IsKeyDown(KeyCode::LSHIFT) ? mRunSpeed : mWalkSpeed;

	const float turnSpeed = 0.1f;

	Matrix4 transform = mTransformComponent->GetMatrix4();
	Vector3 velocity = Vector3::Zero;

	if (inputSystem->IsKeyDown(KeyCode::UP))
	{
		velocity = GetLook(transform) * moveSpeed;
	}
	if (inputSystem->IsKeyDown(KeyCode::DOWN))
	{
		velocity = GetLook(transform) * -moveSpeed;
	}
	if (inputSystem->IsKeyDown(KeyCode::RIGHT))
	{
		velocity = GetRight(transform) * moveSpeed;
	}
	if (inputSystem->IsKeyDown(KeyCode::LEFT))
	{
		velocity = GetRight(transform) * -moveSpeed;
	}

	Vector3 position = GetTranslation(transform);
	position += velocity * deltaTime;

	//if (MagnitudeSqr(velocity) > 0.0f)
	//{
	//	mAnimatorComponent->PlayAnimation(1);
	//}
	//else
	//{
	//	mAnimatorComponent->PlayAnimation(0);
	//}
	// Snap to terrain
	auto terrainService = GetOwner().GetWorld().GetService<TerrainService>();
	float height = terrainService->GetHeight(position);
	auto newPosition = Math::Vector3{ position.x, height, position.z };
	mTransformComponent->position = newPosition;

	// Rotation
	auto mouseX = inputSystem->GetMouseMoveX() * deltaTime;
	auto rotation = Quaternion::RotationEuler({ 0.0f, mouseX, 0.0f });
	mTransformComponent->rotation = mTransformComponent->rotation * rotation;
}

void PlayerControllerComponent::DebugUI()
{

}

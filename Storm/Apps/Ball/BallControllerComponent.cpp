#include "BallControllerComponent.h"


using namespace Storm;
using namespace Storm::Input;
using namespace Storm::Math;
//
MEMORY_POOL_DEFINE(BallControllerComponent, 1000)

void BallControllerComponent::Initialize()
{
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
	mRigidBodyComponent = GetOwner().GetComponent<RigidBodyComponent>();
	//mAnimatorComponent = GetOwner().GetComponent<AnimatorComponent>();
}

void BallControllerComponent::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();

	const float turnSpeed = 0.1f;

	Matrix4 transform = mTransformComponent->GetMatrix4();
	Vector3 force = Vector3::Zero;
	if (inputSystem->IsKeyDown(KeyCode::UP))
	{
		force = GetLook(transform) * mMoveSpeed;
		//velocity = GetLook(transform) * moveSpeed;
	}
	if (inputSystem->IsKeyDown(KeyCode::DOWN))
	{
		force = GetLook(transform) * -mMoveSpeed;

	}
	if (inputSystem->IsKeyDown(KeyCode::RIGHT))
	{
		force += GetRight(transform) * mMoveSpeed;

	}
	if (inputSystem->IsKeyDown(KeyCode::LEFT))
	{
		force += GetRight(transform) * -mMoveSpeed;

	}
	if (inputSystem->IsKeyDown(KeyCode::SPACE))
	{
		force += GetUp(transform) * mMoveSpeed * 0.5f;

	}
	mRigidBodyComponent->SetForce(force);
}

void BallControllerComponent::DebugUI()
{

}
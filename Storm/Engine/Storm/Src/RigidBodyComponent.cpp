#include "Precompiled.h"
#include "RigidBodyComponent.h"

#include "GameObject.h"
#include "ColliderService.h"
#include "TransformComponent.h"
#include "GameWorld.h"

using namespace Storm;
MEMORY_POOL_DEFINE(RigidBodyComponent, 1000);
void RigidBodyComponent::Initialize()
{
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
}

void RigidBodyComponent::DebugUI()
{
}

void RigidBodyComponent::Update(float deltaTime)
{
	mAccleration = mForce / mass + mGravity;
	mVelocity += mAccleration * deltaTime - mVelocity * 0.02f;
	auto position = mTransformComponent->position;
	position += mVelocity * deltaTime;
	if (position.y <= 0)
	{
		position.y = 0;
	}
	mTransformComponent->position = position;


}

void Storm::RigidBodyComponent::SetForce(const Math::Vector3& force)
{
	mForce = force;
}

void Storm::RigidBodyComponent::SetVelocity(const Math::Vector3& velocity)
{
	mVelocity = velocity;
}

Math::Vector3& Storm::RigidBodyComponent::GetVelocity()
{
	return mVelocity;
}

const Math::Vector3& Storm::RigidBodyComponent::GetVelocity() const
{
	return mVelocity;
}

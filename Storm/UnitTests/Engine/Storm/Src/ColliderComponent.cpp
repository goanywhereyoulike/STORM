#include "Precompiled.h"
#include "ColliderComponent.h"

#include "GameObject.h"
#include "ColliderService.h"
#include "PhysicsService.h"
#include "TransformComponent.h"
#include "GameWorld.h"

using namespace Storm;

MEMORY_POOL_DEFINE(ColliderComponent, 1000);
void ColliderComponent::Initialize()
{
	auto colliderService = GetOwner().GetWorld().GetService<ColliderService>();
	colliderService->Register(this);
	auto physicsService = GetOwner().GetWorld().GetService<PhysicsService>();
	//physicsService->Register(this);
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();

}

void ColliderComponent::DebugUI()
{
	//const auto& aabb = GetAABB();
	//Graphics::SimpleDraw::AddAABB(aabb.center, aabb.extend, Graphics::Colors::Green);
	//const auto& obb = GetOBB();
	//Graphics::SimpleDraw::AddOBB(obb, Graphics::Colors::Green);
	const auto& sphere = GetSphere();
	Graphics::SimpleDraw::AddSphere(sphere.center, sphere.radius, Graphics::Colors::Green);
	//Graphics::SimpleDraw::AddTransform(Math::Matrix4::Translation({1.0f,1.01f,1.0f}));
}

void ColliderComponent::Update(float deltaTime)
{


}

void Storm::ColliderComponent::Terminate()
{
	auto colliderService = GetOwner().GetWorld().GetService<ColliderService>();
	colliderService->Unregister(this);
	//auto physicsService = GetOwner().GetWorld().GetService<PhysicsService>();
	//physicsService->Unregister(this);
}

Math::AABB ColliderComponent::GetAABB() const
{
	Math::AABB aabb;
	aabb.center = mTransformComponent->position + mCenter;
	aabb.extend = mExtend;
	return aabb;
}

Math::OBB ColliderComponent::GetOBB() const
{
	Math::OBB obb;
	obb.center = mTransformComponent->position + mCenter;
	obb.extend = mExtend;
	obb.rotation = mTransformComponent->rotation;
	return obb;
}

Math::Sphere Storm::ColliderComponent::GetSphere() const
{
	Math::Sphere sphere;
	sphere.center = mTransformComponent->position + Math::Vector3(0, 1, 0);
	sphere.radius = 4.0f;
	return sphere;
}

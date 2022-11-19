#include "Precompiled.h"
#include "PhysicsService.h"

#include "ColliderComponent.h"

using namespace Storm;
using namespace Storm::Graphics;

void Storm::PhysicsService::Initialize()
{			
}			
			
void Storm::PhysicsService::Terminate()
{			
}			
			
void Storm::PhysicsService::Update(float deltaTime)
{			
}			
			
void Storm::PhysicsService::DebugUI()
{			
}			
			
void Storm::PhysicsService::Register(ColliderComponent* colliderComponent)
{			
	mColliderEntries.push_back(colliderComponent);
}			
			
void Storm::PhysicsService::Unregister(ColliderComponent* colliderComponent)
{
	mColliderEntries.clear();
}
#include "Precompiled.h"
#include "ColliderService.h"

#include "ColliderComponent.h"
#include "TransformComponent.h"
#include "RigidBodyComponent.h"
#include "GameObject.h"
#include "GameWorld.h"

using namespace Storm;
using namespace Storm::Graphics;

void Storm::ColliderService::Initialize()
{

}

void Storm::ColliderService::Terminate()
{
}

void Storm::ColliderService::Update(float deltaTime)
{
	for (int i = 0; i < mColliderEntries.size(); ++i)
	{
		for (int j = i + 1; j < mColliderEntries.size(); ++j)
		{
			//if(Math::Intersect())
			auto sphere1 = mColliderEntries[i]->GetSphere();
			auto sphere2 = mColliderEntries[j]->GetSphere();
			auto distance = Math::DistanceSqr(sphere1.center, sphere2.center);
			auto radius = sphere1.radius + sphere2.radius;
			if (distance <= (radius * radius))
			{
				auto transform1 = mColliderEntries[i]->GetOwner().GetComponent<TransformComponent>();
				auto transform2 = mColliderEntries[j]->GetOwner().GetComponent<TransformComponent>();

				auto direction1 = Math::Normalize(sphere1.center - sphere2.center);
				direction1.y = 0;
				//transform1->position += direction1;

				auto rigidbody1 = mColliderEntries[i]->GetOwner().GetComponent<RigidBodyComponent>();
				auto rigidbody2 = mColliderEntries[j]->GetOwner().GetComponent<RigidBodyComponent>();

				auto v1 = rigidbody1->GetVelocity();
				auto v2 = rigidbody2->GetVelocity();
				auto reflect = direction1;
				reflect.x = direction1.y;
				reflect.y = -direction1.x;

				v1 = reflect * (2.0f * Dot(v1, reflect)) - v1;
				v2 = reflect * (2.0f * Dot(v2, reflect)) - v2;

				rigidbody1->SetVelocity(v1);
				rigidbody2->SetVelocity(v2);


					//transform1->position += direction1;
			}


		}

	}

}

void Storm::ColliderService::DebugUI()
{
}

void Storm::ColliderService::Register(ColliderComponent* colliderComponent)
{
	mColliderEntries.push_back(colliderComponent);
}

void Storm::ColliderService::Unregister(ColliderComponent* colliderComponent)
{
	mColliderEntries.clear();
}

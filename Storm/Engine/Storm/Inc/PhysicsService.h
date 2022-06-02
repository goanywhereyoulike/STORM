#pragma once
#include "Service.h"

namespace Storm
{
	class ColliderComponent;

	class PhysicsService final : public Service
	{
	public:
		SET_TYPE_ID(ServiceId::Physics);
		void Initialize() override;
		void Terminate() override;
		void Update(float deltaTime) override;
		void DebugUI() override;

		void Register(ColliderComponent* colliderComponent);
		void Unregister( ColliderComponent* colliderComponent);

	private:
		std::vector<ColliderComponent*> mColliderEntries;
		Storm::Physics::PhysicsWorld mPhysicsWorld;

	};


}

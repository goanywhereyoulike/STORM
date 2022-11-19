#pragma once
#include "Service.h"

namespace Storm
{
	class ColliderComponent;
	class TransformComponent;
	class RigidBodyComponent;
	class ColliderService final : public Service
	{
	public:
		SET_TYPE_ID(ServiceId::Collider);
		void Initialize() override;
		void Terminate() override;
		void Update(float deltaTime) override;
		void DebugUI() override;

		void Register(ColliderComponent* colliderComponent);
		void Unregister(ColliderComponent* colliderComponent);

	private:
		std::vector<ColliderComponent*> mColliderEntries;
		RigidBodyComponent* mRigidBodyComponent = nullptr;
	};


}

#pragma once
#include "Component.h"

namespace Storm
{
	class TransformComponent;
	class RigidBodyComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::RigidBody);
		MEMORY_POOL_DECLARE;

		void Initialize() override;
		void DebugUI() override;
		void Update(float deltaTime) override;
		void SetForce(const Math::Vector3& force);
		void SetVelocity(const Math::Vector3& velocity);
		Math::Vector3& GetVelocity();
		const Math::Vector3& GetVelocity() const;

	private:
		TransformComponent* mTransformComponent = nullptr;
		Math::Vector3 mGravity = Math::Vector3{ 0.0f,-9.8f,0.0f };
		float mass = 1.0f;
		float friction = 0.1f;
		std::string mTargetName;
		Math::Vector3 mForce = Math::Vector3::Zero;
		Storm::Math::Vector3 mVelocity = Math::Vector3::Zero;
		Storm::Math::Vector3 mAccleration = Math::Vector3::Zero;
	};

}

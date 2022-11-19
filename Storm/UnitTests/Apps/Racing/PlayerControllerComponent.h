#pragma once

#include "TypeIds.h"

class PlayerControllerComponent final : public Storm::Component
{
public:
	SET_TYPE_ID(ComponentId::PlayerController)
	MEMORY_POOL_DECLARE

	void Initialize() override;

	void Update(float deltaTime) override;
	void DebugUI() override;

	void SetWalkSpeed(float walkSpeed) { mWalkSpeed = walkSpeed; }
	void SetRunSpeed(float runSpeed) { mRunSpeed = runSpeed; }

private:
	Storm::TransformComponent* mTransformComponent = nullptr;
	Storm::RigidBodyComponent* mRigidBodyComponent = nullptr;
	Storm::AnimatorComponent* mAnimatorComponent = nullptr;
	float mWalkSpeed = 10.0f;
	float mRunSpeed = 10.0f;
	Storm::Math::Vector3 velocity;
	Storm::Math::Vector3 accleration;
	float mTurnAngle = 0.0f;
};
#pragma once

#include "TypeIds.h"

class BallControllerComponent final : public Storm::Component
{
public:
	SET_TYPE_ID(ComponentId::BallController)
	MEMORY_POOL_DECLARE

	void Initialize() override;

	void Update(float deltaTime) override;
	void DebugUI() override;

	void SetMoveSpeed(float Speed) { mMoveSpeed = Speed; }

private:
	Storm::TransformComponent* mTransformComponent = nullptr;
	Storm::RigidBodyComponent* mRigidBodyComponent = nullptr;
	float mMoveSpeed = 1.0f;
};

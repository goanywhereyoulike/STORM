#pragma once

#include "Component.h"

namespace Storm
{
	class CameraComponent;

	class FollowCameraControllerComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::FollowCameraController)
		MEMORY_POOL_DECLARE

		void Initialize() override;
		void Update(float deltaTime) override;

		void SetTarget(std::string name) { mTargetName = std::move(name); }
		void SetDistance(float distance) { mDistance = distance; }
		void SetCameraElevation(float elevation) { mCameraElevation = elevation; }
		void SetLookElevation(float elevation) { mLookElevation = elevation; }

	private:
		CameraComponent* mCameraComponent = nullptr;
		std::string mTargetName;
		float mDistance = 0.0f;
		float mCameraElevation = 0.0f;
		float mLookElevation = 0.0f;
	};
}
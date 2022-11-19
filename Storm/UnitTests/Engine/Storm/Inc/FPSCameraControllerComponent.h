#pragma once
#include "Component.h"

namespace Storm
{
	class CameraComponent;
	class FPSCameraControllerComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::FPSCameraController);
		MEMORY_POOL_DECLARE;

		void Initialize() override;
		void Update(float deltaTime) override;

	private:
		CameraComponent* mCameraComponent = nullptr;

	};

}

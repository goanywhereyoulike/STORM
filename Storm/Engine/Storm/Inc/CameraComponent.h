#pragma once
#include "Component.h"

namespace Storm
{
	class CameraComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::Camera);
		MEMORY_POOL_DECLARE;

		void Initialize() override;
		void Terminate() override;

		Graphics::Camera& GetCamera() { return mCamera; };
		const Graphics::Camera& GetCamera() const { return mCamera; };

	private:
		Graphics::Camera mCamera;
	};

}
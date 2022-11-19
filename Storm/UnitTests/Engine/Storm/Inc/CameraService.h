#pragma once
#include "Service.h"

namespace Storm
{
	class CameraComponent;

	class CameraService final : public Service
	{

	public:
		SET_TYPE_ID(ServiceId::Camera)


		void Register(const CameraComponent* cameraComponent);
		void Unregister(const CameraComponent* cameraComponent);

		const Graphics::Camera& GetCamera() const;
	private:
		std::vector<const CameraComponent*> mCameraEntries;
		const CameraComponent* mCurrentCamera;


	};


}

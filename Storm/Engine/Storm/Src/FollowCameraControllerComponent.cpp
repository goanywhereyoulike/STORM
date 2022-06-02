#include "Precompiled.h"
#include "FollowCameraControllerComponent.h"

#include "CameraComponent.h"
#include "GameObject.h"
#include "GameWorld.h"
#include "TransformComponent.h"

using namespace Storm;

MEMORY_POOL_DEFINE(FollowCameraControllerComponent, 1000);

void FollowCameraControllerComponent::Initialize()
{
	mCameraComponent = GetOwner().GetComponent<CameraComponent>();
}

void FollowCameraControllerComponent::Update(float deltaTime)
{
	auto gameObject = GetOwner().GetWorld().FindGameObject(mTargetName);
	if (gameObject)
	{
		const auto transformComponent = gameObject->GetComponent<TransformComponent>();
		const auto transform = transformComponent->GetMatrix4();
		const auto position = Math::GetTranslation(transform);
		const auto distance = Math::GetLook(transform) * -mDistance;
		const auto cameraElevation = Math::Vector3::YAxis * mCameraElevation;
		const auto lookElevation = Math::Vector3::YAxis * mLookElevation;

		auto& camera = mCameraComponent->GetCamera();
		camera.SetPosition(position + distance + cameraElevation);
		camera.SetLookAt(position + lookElevation);

	}
}
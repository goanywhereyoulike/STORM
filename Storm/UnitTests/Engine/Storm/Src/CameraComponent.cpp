#include "Precompiled.h"
#include "CameraComponent.h"

#include "CameraService.h"
#include "GameObject.h"
#include "GameWorld.h"

using namespace Storm;

MEMORY_POOL_DEFINE(CameraComponent, 1000);
void CameraComponent::Initialize()
{
	auto cameraService = GetOwner().GetWorld().GetService<CameraService>();
	cameraService->Register(this);
	mCamera.SetPosition({ 0.0f,1.0f,-3.0f });
	mCamera.SetLookAt({ 0.0f,1.0f,0.0f });
	//mCamera.SetMode(Graphics::Camera::ProjectionMode::Perspective);
	//mCamera.SetSize(40.0f, 20.0f);

}

void CameraComponent::Terminate()
{
	auto cameraService = GetOwner().GetWorld().GetService<CameraService>();
	cameraService->Unregister(this);
}

#include "Precompiled.h"
#include "CameraService.h"

#include "CameraComponent.h"

using namespace Storm;
using namespace Storm::Graphics;

void CameraService::Register(const CameraComponent* cameraComponent)
{
	mCameraEntries.push_back(cameraComponent);
	if (mCurrentCamera == nullptr)
	{
		mCurrentCamera = cameraComponent;
	}

}

void CameraService::Unregister(const CameraComponent* cameraComponent)
{
	auto iter = std::find(mCameraEntries.begin(), mCameraEntries.end(), cameraComponent);
	if (iter != mCameraEntries.end())
	{
		if (mCurrentCamera == *iter)
		{
			if (mCameraEntries.empty())
			{
				mCurrentCamera = nullptr;
			}
			else
			{
				mCurrentCamera = mCameraEntries.front();
			}
		}
		mCameraEntries.erase(iter);
	}
}

const Camera& Storm::CameraService::GetCamera() const
{
	return mCurrentCamera->GetCamera();
}

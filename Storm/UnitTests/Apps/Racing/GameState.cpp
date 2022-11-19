#include "GameState.h"
#include "PlayerControllerComponent.h"

using namespace Storm;
using namespace Storm::Graphics;
using namespace Storm::Input;
using namespace Storm::Math;
using namespace Storm::Physics;

namespace
{
	bool OnMake(const char* componentName, const rapidjson::Value& value, GameObject& gameObject)
	{
		if (strcmp(componentName, "PlayerControllerComponent") == 0)
		{
			auto playerControllerComponent = gameObject.AddComponent<PlayerControllerComponent>();
			if (value.HasMember("WalkSpeed"))
			{
				const float& walkSpeed = value["WalkSpeed"].GetFloat();
				playerControllerComponent->SetWalkSpeed(walkSpeed);
			}
			if (value.HasMember("RunSpeed"))
			{
				const float& runSpeed = value["RunSpeed"].GetFloat();
				playerControllerComponent->SetRunSpeed(runSpeed);
			}
			return true;
		}
		return false;
	}
}

void GameState::Initialize()
{
	mGameWorld.AddService<RenderService>();
	mGameWorld.AddService<CameraService>();
	mGameWorld.AddService<TerrainService>();
	mGameWorld.AddService<ColliderService>();
	mGameWorld.Initialize(1000);
	GameObjectFactory::SetMakeOverride(OnMake);
	mGameWorld.CreateGameObject("../../Assets/Templates/follow_camera.json");
	mGameObjectHandles.push_back(mGameWorld.CreateGameObject("../../Assets/Templates/car2.json")->GetHandle());
	auto gameObject = mGameWorld.CreateGameObject("../../Assets/Templates/car.json");
	gameObject->SetName("Player");
	mGameObjectHandle = gameObject->GetHandle();
	mGameObjectHandles.push_back(mGameObjectHandle);

}

void GameState::Terminate()
{
	mGameWorld.Terminate();
}

void GameState::Update(float deltaTime)
{
	mGameWorld.Update(deltaTime);
}

void GameState::Render()
{
	for (auto& handle : mGameObjectHandles)
	{
		auto gameObject = mGameWorld.GetGameObject(handle);
		if (gameObject != nullptr)
		{
			gameObject->DebugUI();
		}
	}


	mGameWorld.Render();
	auto& camera = mGameWorld.GetService<CameraService>()->GetCamera();
}

void GameState::DebugUI()
{
	mGameWorld.DebugUI();
}

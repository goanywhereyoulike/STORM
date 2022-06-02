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
	mGameWorld.CreateGameObject("../../Assets/Templates/fps_camera.json");
	auto gameObject = mGameWorld.CreateGameObject("../../Assets/Templates/player.json");
	gameObject->SetName("Player");
	mGameObjectHandle = gameObject->GetHandle();
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
	auto gameObject = mGameWorld.GetGameObject(mGameObjectHandle);
	if (gameObject != nullptr)
	{
		gameObject->DebugUI();
	}

	mGameWorld.Render();
	auto& camera = mGameWorld.GetService<CameraService>()->GetCamera();
}

void GameState::DebugUI()
{
	mGameWorld.DebugUI();
}

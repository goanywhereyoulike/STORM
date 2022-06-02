#include "GameState.h"
#include "BallControllerComponent.h"

using namespace Storm;
using namespace Storm::Graphics;
using namespace Storm::Input;
using namespace Storm::Math;
using namespace Storm::Physics;

namespace
{
	bool OnMake(const char* componentName, const rapidjson::Value& value, GameObject& gameObject)
	{
		if (strcmp(componentName, "BallControllerComponent") == 0)
		{
			auto ballControllerComponent = gameObject.AddComponent<BallControllerComponent>();
			if (value.HasMember("MoveSpeed"))
			{
				const float& walkSpeed = value["MoveSpeed"].GetFloat();
				ballControllerComponent->SetMoveSpeed(walkSpeed);
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
	//mGameWorld.AddService<PhysicsService>();
	mGameWorld.AddService<ColliderService>();
	mGameWorld.Initialize(1000);
	GameObjectFactory::SetMakeOverride(OnMake);
	mGameWorld.CreateGameObject("../../Assets/Templates/fps_camera.json");
	mGameObjectHandles.push_back(mGameWorld.CreateGameObject("../../Assets/Templates/car2.json")->GetHandle());
	auto gameObject = mGameWorld.CreateGameObject("../../Assets/Templates/ball.json");
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

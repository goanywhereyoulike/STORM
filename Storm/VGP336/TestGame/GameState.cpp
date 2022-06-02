#include "GameState.h"

using namespace Storm;
using namespace Storm::Graphics;
using namespace Storm::Input;
using namespace Storm::Math;
using namespace Storm::Physics;
namespace
{
	bool OnMake(const char* componentName, const rapidjson::Value& value, GameObject& gameObject)
	{
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
	auto gameObject = mGameWorld.CreateGameObject("../../Assets/Templates/test.json");
	mGameObjectHandle = gameObject->GetHandle();
	mGameWorld.CreateGameObject("../../Assets/Templates/player.json");

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

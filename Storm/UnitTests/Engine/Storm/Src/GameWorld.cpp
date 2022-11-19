#include "Precompiled.h"
#include "GameObjectFactory.h"
#include "GameWorld.h"
#include "TerrainService.h"

using namespace Storm;
namespace rj = rapidjson;

void GameWorld::Initialize(uint32_t capacity)
{
	ASSERT(!mInitialized, "GameWorld - Already initialized.");

	for (auto& service : mServices)
	{
		service->Initialize();
	}

	mGameObjectSlots.resize(capacity);
	mFreeSlots.resize(capacity);
	std::iota(mFreeSlots.rbegin(), mFreeSlots.rend(), 0);
	mInitialized = true;
}

void GameWorld::Terminate()
{
	ASSERT(!mUpdating, "GameWorld - Cannont terminate world during update.");
	if (!mInitialized)
	{
		return;
	}

	for (auto gameObject : mUpdateList)
	{
		DestroyGameObject(gameObject->GetHandle());
	}
	ProcessDestroyList();

	ASSERT(mUpdateList.empty(), "GameWorld - Failed to clean up game objects.");

	for (auto& service : mServices)
	{
		service->Terminate();

	}

	mInitialized = false;


}

void GameWorld::Update(float deltaTime)
{
	ASSERT(!mUpdating, "GameWorld - Already updating the world.");

	mUpdating = true;

	for (auto& service : mServices)
	{
		service->Update(deltaTime);
	}

	for (size_t i = 0; i < mUpdateList.size(); ++i)
	{
		GameObject* gameObject = mUpdateList[i];
		if (IsValid(gameObject->GetHandle()))
		{
			gameObject->Update(deltaTime);
		}

	}

	mUpdating = false;

	ProcessDestroyList();
}

void GameWorld::Render()
{
	for (auto& service : mServices)
	{
		service->Render();
	}
}

void GameWorld::DebugUI()
{
	for (auto& service : mServices)
	{
		service->DebugUI();
	}
}
void GameWorld::LoadLevel(std::filesystem::path levelFile)
{
	FILE* file = nullptr;
	auto err = fopen_s(&file, levelFile.u8string().c_str(), "r");
	ASSERT(err == 0 || file != nullptr, "GameWorld -- Failed to open level file '%s'", levelFile.u8string().c_str());

	char readBuffer[65536];
	rj::FileReadStream readStream(file, readBuffer, sizeof(readBuffer));

	fclose(file);

	rj::Document document;
	document.ParseStream(readStream);

	auto services = document["Services"].GetObj();
	for (auto& service : services)
	{
		const char* serviceName = service.name.GetString();
		if (strcmp(serviceName, "TerrainService") == 0)
		{
			auto terrainService = AddService<TerrainService>();
				if (service.value.HasMember("HeightMap"))
				{
					const auto& heightmap = service.value["HeightMap"].GetString();
					
				}

		}
		/*else if (strcmp(componentName, "ColliderComponent") == 0)
		{
			auto collider = gameObject.AddComponent<ColliderComponent>();
			if (component.value.HasMember("Center"))
			{
				const auto& center = component.value["Center"].GetArray();
				const float x = center[0].GetFloat();
				const float y = center[1].GetFloat();
				const float z = center[2].GetFloat();
				collider->SetCenter({ x,y,z });
			}
			if (component.value.HasMember("Extend"))
			{
				const auto& extend = component.value["Extend"].GetArray();
				const float x = extend[0].GetFloat();
				const float y = extend[1].GetFloat();
				const float z = extend[2].GetFloat();
				collider->SetExtend({ x,y,z });
			}
		}
		else if (strcmp(componentName, "ModelComponent") == 0)
		{
			auto model = gameObject.AddComponent<ModelComponent>();
			if (component.value.HasMember("FileName"))
			{
				const auto& fileName = component.value["FileName"].GetString();
				model->SetFileName(fileName);
			}
			if (component.value.HasMember("Rotation"))
			{
				const auto& rotation = component.value["Rotation"].GetArray();
				const float x = rotation[0].GetFloat() * Math::Constants::DegToRad;
				const float y = rotation[1].GetFloat() * Math::Constants::DegToRad;
				const float z = rotation[2].GetFloat() * Math::Constants::DegToRad;
				model->SetRotation({ x,y,z });
			}
		}
		else if (strcmp(componentName, "TransformComponent") == 0)
		{
			auto transform = gameObject.AddComponent<TransformComponent>();
			if (component.value.HasMember("Position"))
			{
				const auto& position = component.value["Position"].GetArray();
				const float x = position[0].GetFloat();
				const float y = position[1].GetFloat();
				const float z = position[2].GetFloat();
				transform->position = { x,y,z };
			}
			if (component.value.HasMember("Rotation"))
			{
				const auto& rotation = component.value["Rotation"].GetArray();
				const float x = rotation[0].GetFloat() * Math::Constants::DegToRad;
				const float y = rotation[1].GetFloat() * Math::Constants::DegToRad;
				const float z = rotation[2].GetFloat() * Math::Constants::DegToRad;
				transform->rotation = Math::Quaternion::RotationEuler({ x,y,z });
			}
			if (component.value.HasMember("Scale"))
			{
				const auto& scale = component.value["Scale"].GetArray();
				const float x = scale[0].GetFloat();
				const float y = scale[1].GetFloat();
				const float z = scale[2].GetFloat();
				transform->scale = { x,y,z };
			}
		}
		else if (strcmp(componentName, "CameraComponent") == 0)
		{
			auto camera = gameObject.AddComponent<CameraComponent>();
		}
		else if (strcmp(componentName, "FPSCameraControllerComponent") == 0)
		{
			auto camera = gameObject.AddComponent<FPSCameraControllerComponent>();
		}*/

	}
}
GameObject* GameWorld::CreateGameObject(std::filesystem::path templateFile)
{
	ASSERT(mInitialized, "GameWorld - World must be initialized first before creating game objects.");
	if (mFreeSlots.empty())
	{
		return nullptr;
	}

	const uint32_t freeSlot = mFreeSlots.back();
	mFreeSlots.pop_back();

	auto& slot = mGameObjectSlots[freeSlot];
	auto& newObject = slot.gameObject;
	newObject = std::make_unique<GameObject>();

	GameObjectFactory::Make(templateFile, *newObject);

	GameObjectHandle handle;
	handle.mIndex = freeSlot;
	handle.mGeneration = slot.generation;

	newObject->mWorld = this;
	newObject->mHandle = handle;
	newObject->Initialize();

	mUpdateList.push_back(newObject.get());

	return newObject.get();


}

GameObject* GameWorld::FindGameObject(const std::string& name)
{
	auto iter = std::find_if
	(
		mUpdateList.begin(),
		mUpdateList.end(),
		[&](auto gameObject)
		{
			return gameObject->GetName() == name;

		});

	return iter == mUpdateList.end() ? nullptr : *iter;
}


GameObject* GameWorld::GetGameObject(GameObjectHandle handle)
{
	if (IsValid(handle))
	{
		return mGameObjectSlots[handle.mIndex].gameObject.get();
	}

	return nullptr;
}

void GameWorld::DestroyGameObject(GameObjectHandle handle)
{
	if (!IsValid(handle))
	{
		return;
	}

	auto& slot = mGameObjectSlots[handle.mIndex];
	slot.generation++;
	mToBeDestroyed.push_back(handle.mIndex);
}

bool GameWorld::IsValid(GameObjectHandle handle) const
{
	return
		handle.mIndex >= 0 &&
		handle.mIndex < mGameObjectSlots.size() &&
		mGameObjectSlots[handle.mIndex].generation == handle.mGeneration;
}

void GameWorld::ProcessDestroyList()
{
	ASSERT(!mUpdating, "GameWorld - Cannont destroy game objects during update.");

	for (auto index : mToBeDestroyed)
	{
		auto& slot = mGameObjectSlots[index];
		GameObject* gameObject = slot.gameObject.get();
		ASSERT(!IsValid(gameObject->GetHandle()), "GameWorld - Game object is still valid!");

		mUpdateList.erase(std::remove(mUpdateList.begin(), mUpdateList.end(), gameObject));

		gameObject->Terminate();

		slot.gameObject.reset();

		mFreeSlots.push_back(index);


	}

	mToBeDestroyed.clear();

}

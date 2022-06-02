#include "Precompiled.h"
#include "ModelManager.h"

using namespace Storm;
using namespace Storm::Graphics;

namespace
{
	std::unique_ptr<ModelManager> sInstance;
}
void ModelManager::StaticInitialize()
{
	ASSERT(sInstance == nullptr, "ModelManager -- System already initialized!");
	sInstance = std::make_unique<ModelManager>();
}

void ModelManager::StaticTerminate()
{
	if (sInstance != nullptr)
	{
		sInstance.reset();
	}
}

ModelManager* ModelManager::Get()
{
	ASSERT(sInstance != nullptr, "ModelManager -- No system registered.");
	return sInstance.get();
}

ModelId Storm::Graphics::ModelManager::LoadModel(std::filesystem::path fileName)
{
	auto modelId = std::filesystem::hash_value(fileName);
	auto [iter, success] = mInventory.insert({ modelId,nullptr });
	if (success)
	{
		auto& modelPtr = iter->second;
		modelPtr = std::make_unique<Model>();
		Storm::Graphics::ModelIO::LoadModel(fileName, *modelPtr);
		Storm::Graphics::ModelIO::LoadMaterial(fileName, *modelPtr);
		Storm::Graphics::ModelIO::LoadSkeleton(fileName, *modelPtr);
		Storm::Graphics::ModelIO::LoadAnimationSet(fileName, *modelPtr);
	}
	return modelId;
}

Model* ModelManager::GetModel(ModelId modelId)
{
	auto iter = mInventory.find(modelId);
	return iter != mInventory.end() ? iter->second.get() : nullptr;
}

const Model* ModelManager::GetModel(ModelId modelId) const
{
	auto iter = mInventory.find(modelId);
	return iter != mInventory.end() ? iter->second.get() : nullptr;
}

const Model* ModelManager::GetModel(std::filesystem::path fileName) const
{
	auto modelId = ModelId(std::filesystem::hash_value(fileName));
	return GetModel(modelId);
}


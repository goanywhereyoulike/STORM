#include "Precompiled.h"
#include "TextureManager.h"

using namespace Storm;
using namespace Storm::Graphics;

namespace
{
	std::unique_ptr<TextureManager> sInstance;
}

void TextureManager::StaticInitialize()
{
	ASSERT(sInstance == nullptr, "TextureManager -- System already initialized!");
	sInstance = std::make_unique<TextureManager>();
}

void TextureManager::StaticTerminate()
{
	if (sInstance != nullptr)
		sInstance.reset();
}

TextureManager* TextureManager::Get()
{
	ASSERT(sInstance != nullptr, "TextureManager -- No system registered.");
	return sInstance.get();
}

TextureManager::~TextureManager()
{
	for (auto& [id, texture] : mInventory)
		texture->Terminate();
	mInventory.clear();
}

TextureId TextureManager::LoadTexture(std::filesystem::path fileName)
{
	auto textureId = std::filesystem::hash_value(fileName);
	auto [iter, success] = mInventory.insert({ textureId, nullptr });
	if (success)
	{
		auto& texturePtr = iter->second;
		texturePtr = std::make_unique<Texture>();
		texturePtr->Initialize(fileName);
	}
	return textureId;
}

const Texture* TextureManager::GetTexture(TextureId TextureId) const
{
	auto iter = mInventory.find(TextureId);
	return iter != mInventory.end() ? iter->second.get() : nullptr;
}

const Texture* TextureManager::GetTexture(std::filesystem::path fileName) const
{
	auto TextureId = std::filesystem::hash_value(fileName);
	return GetTexture(TextureId);
}

void TextureManager::BindVS(TextureId id, uint32_t slot)
{
	if (id != 0)
		GetTexture(id)->BindVS(slot);
}

void TextureManager::BindPS(TextureId id, uint32_t slot)
{
	if (id != 0)
		GetTexture(id)->BindPS(slot);
}

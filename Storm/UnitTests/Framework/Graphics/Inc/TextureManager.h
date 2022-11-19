#pragma once

#include "Texture.h"

namespace Storm::Graphics
{
	using TextureId = std::size_t;

	class TextureManager final
	{
	public:
		static void StaticInitialize();
		static void StaticTerminate();
		static TextureManager* Get();
		
	public:
		~TextureManager();

		TextureId LoadTexture(std::filesystem::path fileName);

		const Texture* GetTexture(TextureId TextureId) const;
		const Texture* GetTexture(std::filesystem::path fileName) const;

		void BindVS(TextureId id, uint32_t slot);
		void BindPS(TextureId id, uint32_t slot);

	private:
		std::unordered_map<TextureId, std::unique_ptr<Texture>> mInventory;
	};
}
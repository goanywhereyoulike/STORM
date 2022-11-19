#pragma once
#include "Service.h"

namespace Storm
{
	class TerrainService final : public Service
	{
	public:
		SET_TYPE_ID(ServiceId::Terrain)
		void Initialize() override;
		void Terminate() override;

		void DebugUI() override;
		float GetHeight(const Math::Vector3& position) const;

		void LoadTerrain(const char* fileName, float maxHeight);
		void LoadTexture(const char* fileName, uint32_t index);
		const Graphics::RenderObject& GetTerrainRenderObject() const { return mTerrainRenderObject; };
		void SetHeightMapName(const char* heightMapName) { mHeightMapName = heightMapName; }

	private:
		Graphics::Terrain mTerrain;
		Graphics::RenderObject mTerrainRenderObject;
		std::string mHeightMapName;
		float mMaxHeight = 35.0f;
		std::vector<std::string> mTextureNames;
	};


}

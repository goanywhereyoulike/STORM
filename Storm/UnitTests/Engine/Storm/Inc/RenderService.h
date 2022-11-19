#pragma once
#include "Service.h"

namespace Storm
{
	class AnimatorComponent;
	class ModelComponent;
	class TransformComponent;
	class CameraService;
	class TerrainService;
	class RenderService final : public Service
	{

	public:
		SET_TYPE_ID(ServiceId::Render)

		void Initialize() override;
		void DebugUI() override;
		void Terminate() override;
		void Update(float deltaTime) override;
		void Render() override;

		void LoadSkydome(const char* fileName);

		void SetShadowFocus(const Math::Vector3& focusPosition);

		Storm::Graphics::DirectionalLight& GetLight() { return mDirectionLight; }

	private:
		friend class ModelComponent;
		void Register(const ModelComponent* modelComponent);
		void Unregister(const ModelComponent* modelComponent);
		struct Entry
		{
			const AnimatorComponent* animatorComponent = nullptr;
			const ModelComponent* modelComponent = nullptr;
			const TransformComponent* transformComponent = nullptr;
			Graphics::RenderGroup renderGroup;
		};

		std::vector<Entry> mRenderEntries;
		const CameraService* mCameraService = nullptr;
		const TerrainService* mTerrainService = nullptr;
		Storm::Graphics::DirectionalLight mDirectionLight;



		Storm::Graphics::StandardEffect mStandardEffect;
		Storm::Graphics::TexturingEffect mTexturingEffect;
		Storm::Graphics::TerrainEffect mTerrainEffect;
		Storm::Graphics::ShadowEffect mShadowEffect;


		Storm::Graphics::RenderObject mSkydome;
		float mFPS = 0.0f;
		bool mDrawSkeleton = false;

	};


}

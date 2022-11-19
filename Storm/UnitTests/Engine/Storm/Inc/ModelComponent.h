#pragma once
#include "Component.h"

namespace Storm
{
	class ModelComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::Model);
		MEMORY_POOL_DECLARE;

		void Initialize() override;
		void Terminate() override;

		Graphics::Model& GetModel()
		{
			return const_cast<Graphics::Model&>(*Graphics::ModelManager::Get()->GetModel(mModelId));
		}

		const Graphics::Model& GetModel() const
		{
			return *Graphics::ModelManager::Get()->GetModel(mModelId);
		}

		void SetFileName(const char* fileName) { mFileName = fileName; }

		void SetRotation(const Math::Vector3& rotation) { mRotation = rotation; }
		const Math::Vector3& GetRotation() const { return mRotation; }
	private:
		std::string mFileName;
		Graphics::ModelId mModelId;
		Math::Vector3 mRotation = Math::Vector3::Zero;
	};

}
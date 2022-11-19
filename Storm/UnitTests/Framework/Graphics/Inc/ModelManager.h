#pragma once
#include "Model.h"
#include "ModelIO.h"
namespace Storm::Graphics
{
	using ModelId = std::size_t;

	class ModelManager final
	{
	public:
		static void StaticInitialize();
		static void StaticTerminate();
		static ModelManager* Get();

	public:
		ModelId LoadModel(std::filesystem::path fileName);

		Model* GetModel(ModelId modelId);
		const Model* GetModel(ModelId modelId) const;
		const Model* GetModel(std::filesystem::path fileName) const;

	private:
		std::unordered_map<ModelId, std::unique_ptr<Model>> mInventory;
	};
}

#pragma once
namespace Storm::Graphics
{
	struct Model;

	namespace ModelIO
	{
		void SaveModel(std::filesystem::path filePath, const Model& model);
		void LoadModel(std::filesystem::path filePath, Model& model);

		void SaveMaterial(std::filesystem::path filePath, const Model& model);
		void LoadMaterial(std::filesystem::path filePath, Model& model);

		void SaveSkeleton(std::filesystem::path filePath, const Model& model);
		void LoadSkeleton(std::filesystem::path filePath, Model& model);


		void SaveAnimationSet(std::filesystem::path filePath, const Model& model);
		void LoadAnimationSet(std::filesystem::path filePath, Model& model);
		
	}

}
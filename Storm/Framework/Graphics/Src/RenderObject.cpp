#include "Precompiled.h"
#include "RenderObject.h"

using namespace Storm;
using namespace Storm::Graphics;

void RenderObject::Terminate()
{
	meshBuffer.Terminate();
}

RenderGroup Storm::Graphics::CreateRenderGroup(const Model& model, const IAnimator* animator)
{
	RenderGroup renderGroup;
	renderGroup.reserve(model.meshData.size());

	auto TryLoadTexture = [](const auto& textureName) -> TextureId
	{
		if (!textureName.empty())
		{
			return TextureManager::Get()->LoadTexture(textureName);
		}
		return 0;
	};
	for (auto& meshData : model.meshData)
	{
		RenderObject& renderObject = renderGroup.emplace_back();
		auto& materialData = model.materialData[meshData.materialIndex];
		renderObject.material = materialData.material;
		renderObject.diffuseMapId = TryLoadTexture(materialData.diffuseMapName);
		renderObject.displacementMapId = TryLoadTexture(materialData.displacementMapName);
		renderObject.normalMapId = TryLoadTexture(materialData.normalMapName);
		renderObject.specularMapId = TryLoadTexture(materialData.specularMapName);

		renderObject.skeleton = model.skeleton.get();
		renderObject.animator = animator;
		//renderObject.animationclip = &model.animationSet[0];


		renderObject.meshBuffer.Initialize(meshData.mesh);
	}

	return renderGroup;

}

void Storm::Graphics::CleanUpRenderGroup(RenderGroup& renderGroup)
{
	for (auto& renderObject : renderGroup)
	{
		renderObject.Terminate();
	}
}

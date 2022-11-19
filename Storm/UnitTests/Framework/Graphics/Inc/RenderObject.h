#pragma once
#include "Material.h"
#include "MeshBuffer.h"
#include "Skeleton.h"
#include "TextureManager.h"
#include "Transform.h"
#include "Model.h"

namespace Storm::Graphics
{
	struct IAnimator;
	class RenderObject final
	{
	public:
		void Terminate();

		Transform transform;
		Material material;

		TextureId diffuseMapId = 0;
		TextureId specularMapId = 0;
		TextureId displacementMapId = 0;
		TextureId normalMapId = 0;

		const Skeleton* skeleton = nullptr;
		const IAnimator* animator = nullptr;

		bool useAdditiveBlend = false;
		MeshBuffer meshBuffer;
	};

	using RenderGroup = std::vector<RenderObject>;
	[[nodiscard]] RenderGroup CreateRenderGroup(const Model& model, const IAnimator* animator = nullptr);

	void CleanUpRenderGroup(RenderGroup& renderGroup);

}

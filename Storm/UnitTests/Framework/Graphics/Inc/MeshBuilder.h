#pragma once
#include "MeshTypes.h"
namespace Storm::Graphics
{

	class MeshBuilder
	{
	public:
		static MeshPC CreateCubePC();

		static MeshPX CreatePlanePX(uint32_t columns, uint32_t rows, float spacing);
		static MeshPX CreateCylinderPX(uint32_t slices, uint32_t rings, float radius, float height);
		static MeshPX CreateSpherePX(uint32_t slices, uint32_t rings, float radius);
		static MeshPX CreateConePX(uint32_t slices, uint32_t rings, float radius, float height);
		static MeshPX CreateCirclePlanePX(uint32_t slices, float radius);
		static MeshPX CreateCubePX();
		static MeshPX CreateSkyBoxPX();
		static MeshPX CreateSkyDome(uint32_t slices, uint32_t rings, float radius);

		static Mesh CreatePlane(uint32_t columns, uint32_t rows, float spacing, float tiling = 1.0f);
		static Mesh CreateSphere(uint32_t slices, uint32_t rings, float radius);

		static MeshPC CreateParticle(float radius, Color color);
		static MeshPCX CreateParticle2D(float size, Color);
	};


}
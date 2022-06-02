#pragma once

#include "Colors.h"

namespace Storm::Graphics
{
	class Camera;
}

namespace Storm::Graphics::SimpleDraw
{
	void StaticInitialize(uint32_t maxVertexCount = 10000);
	void StaticTerminate();

	void AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color);

	// TODO
	void AddAABB(const Math::Vector3& center, const Math::Vector3& extend, const Color& color);
	void AddSphere(const Math::Vector3& center, float radius, const Color& color, size_t rings = 10, size_t slices = 20);

	void AddCone(const Math::Vector3& center, float radius, float height,const Color& color);
	void AddCylinder(const Math::Vector3& center, float radius, float height, const Color& color);
	void AddCapsule(const Math::Vector3& center, float radius, float height, const Color& color);

	void AddOBB(const Math::OBB& obb, const Color& color);

	void AddTransform(const Math::Matrix4& transform);
	void AddGroundPlane(float size, const Color& color);



	void Render(const Camera& camera);
}

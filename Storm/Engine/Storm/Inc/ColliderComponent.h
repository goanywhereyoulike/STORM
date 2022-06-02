#pragma once
#include "Component.h"

namespace Storm
{
	class TransformComponent;
	class ColliderComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::Collider);
		MEMORY_POOL_DECLARE;

		void Initialize() override;
		void DebugUI() override;
		void Update(float deltaTime) override;
		void Terminate() override;
		Math::AABB GetAABB() const;
		Math::OBB GetOBB() const;
		Math::Sphere GetSphere() const;

		void SetCenter(const Math::Vector3& center) { mCenter = center; }
		void SetExtend(const Math::Vector3& extend) { mExtend = extend; }
		void SetRadius(float radius) { mRadius = radius; }

	private:
		const TransformComponent* mTransformComponent = nullptr;

		Math::Vector3 mCenter = Math::Vector3::Zero;
		Math::Vector3 mExtend = Math::Vector3::One;
		float mRadius = 0.0f;
	};

}
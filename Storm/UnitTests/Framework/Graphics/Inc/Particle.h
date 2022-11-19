#pragma once
#include "ConstantBuffer.h"
#include "Effect.h"
#include "Material.h"
#include "MeshBuffer.h"
#include "Texture.h"
#include "Transform.h"

namespace Storm::Graphics
{
	struct Particle
	{
		Math::Vector3 velocity;
		Math::Vector3 acceleration;

		float lifeTime;
		float lifeRemain;
		Transform transform;
		Color startColor;
		Color endColor;
		Color color = Colors::White;
		bool useAdditiveBlend = true;
		float WaitShoot = 0.0f;
		float size = 0.1f;
		bool IsActive = false;

		TextureId diffuseMapId = 0;
		void Initialize()
		{
			transform.position = Math::Vector3::Zero;
			acceleration = Math::Vector3::Zero;
			velocity = Math::Vector3(0.0f,0.1f,0.0f);
			lifeTime = 1.0f;
			lifeRemain = 0.0f;
			startColor = Colors::White;
			endColor = Colors::White;
			useAdditiveBlend = true;

		}

		void update(float deltaTime)
		{
			velocity += acceleration * deltaTime;
			transform.position += velocity * deltaTime;
		}

		void SetRotation(Math::Quaternion rotation)
		{
		
			transform.rotation = rotation;
		}

	};
}
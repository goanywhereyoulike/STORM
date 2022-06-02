#pragma once
#include "Common.h"

namespace Storm
{
	enum class ComponentId
	{
		Animator,
		Camera,
		Collider,
		FPSCameraController,
		FollowCameraController,
		Model,
		RigidBody,
		Transform,
		Count

	};

	enum class ServiceId
	{
		Render,
		Camera,
		Terrain,
		Collider,
		Physics,
		Count

	};


}

#define SET_TYPE_ID(id)\
	static uint32_t StaticGetTypeId(){return static_cast<uint32_t>(id); }\
	uint32_t GetTypeId() const override {return StaticGetTypeId();}
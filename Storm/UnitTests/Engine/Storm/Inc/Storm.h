#pragma once
#include "Common.h"
//APP Headers
#include "App.h"
#include "AppState.h"
#include "AnimatorComponent.h"
#include "TransformComponent.h"
#include "RigidBodyComponent.h"


//Service headers
#include "CameraService.h"
#include "ColliderService.h"
#include "PhysicsService.h"
#include "RenderService.h"
#include "TerrainService.h"
//World Headers
#include "Component.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "GameObjectHandle.h"
#include "GameWorld.h"
#include "Service.h"

namespace Storm
{
	App& MainApp();

}
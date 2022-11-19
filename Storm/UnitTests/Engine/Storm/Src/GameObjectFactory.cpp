#include "Precompiled.h"
#include "AnimatorComponent.h"
#include "CameraComponent.h"
#include "ColliderComponent.h"
#include "FollowCameraControllerComponent.h"
#include "FPSCameraControllerComponent.h"
#include "GameObjectFactory.h"
#include "GameObject.h"
#include "ModelComponent.h"
#include "RigidBodyComponent.h"
#include "TransformComponent.h"


using namespace Storm;
namespace rj = rapidjson;
namespace
{
	MakeOverride OnMake;
}
void GameObjectFactory::SetMakeOverride(MakeOverride makeOverride)
{

	OnMake = makeOverride;

}
void GameObjectFactory::Make(std::filesystem::path templateFile, GameObject& gameObject)
{

	FILE* file = nullptr;
	auto err = fopen_s(&file, templateFile.u8string().c_str(), "r");
	ASSERT(err == 0 || file != nullptr, "GameObjectFactory -- Failed to template file '%s'", templateFile.u8string().c_str());

	char readBuffer[65536];
	rj::FileReadStream readStream(file, readBuffer, sizeof(readBuffer));

	fclose(file);

	rj::Document document;
	document.ParseStream(readStream);

	auto components = document["Components"].GetObj();
	for (auto& component : components)
	{
		const char* componentName = component.name.GetString();
		if (OnMake(componentName, component.value, gameObject))
		{
			continue;
		}
		else if (strcmp(componentName, "AnimatorComponent") == 0)
		{
			auto animatorComponent = gameObject.AddComponent<AnimatorComponent>();
			if (component.value.HasMember("AnimSets"))
			{
				const auto& animSets = component.value["AnimSets"].GetArray();

				for (auto& animSet : animSets)
				{
					animatorComponent->AddAnimation(animSet.GetString());
				}
			}

		}
		else if (strcmp(componentName, "ColliderComponent") == 0)
		{
			auto collider = gameObject.AddComponent<ColliderComponent>();
			if (component.value.HasMember("Center"))
			{
				const auto& center = component.value["Center"].GetArray();
				const float x = center[0].GetFloat();
				const float y = center[1].GetFloat();
				const float z = center[2].GetFloat();
				collider->SetCenter({ x,y,z });
			}
			if (component.value.HasMember("Extend"))
			{
				const auto& extend = component.value["Extend"].GetArray();
				const float x = extend[0].GetFloat();
				const float y = extend[1].GetFloat();
				const float z = extend[2].GetFloat();
				collider->SetExtend({ x,y,z });
			}
			if (component.value.HasMember("Radius"))
			{
				const auto& radius = component.value["Radius"].GetFloat();
				collider->SetRadius(radius);
			}

		}
		else if (strcmp(componentName, "ModelComponent") == 0)
		{
			auto model = gameObject.AddComponent<ModelComponent>();
			if (component.value.HasMember("FileName"))
			{
				const auto& fileName = component.value["FileName"].GetString();
				model->SetFileName(fileName);
			}
			if (component.value.HasMember("Rotation"))
			{
				const auto& rotation = component.value["Rotation"].GetArray();
				const float x = rotation[0].GetFloat() * Math::Constants::DegToRad;
				const float y = rotation[1].GetFloat() * Math::Constants::DegToRad;
				const float z = rotation[2].GetFloat() * Math::Constants::DegToRad;
				model->SetRotation({ x,y,z });
			}
		}
		else if (strcmp(componentName, "TransformComponent") == 0)
		{
			auto transform = gameObject.AddComponent<TransformComponent>();
			if (component.value.HasMember("Position"))
			{
				const auto& position = component.value["Position"].GetArray();
				const float x = position[0].GetFloat();
				const float y = position[1].GetFloat();
				const float z = position[2].GetFloat();
				transform->position = { x,y,z };
			}
			if (component.value.HasMember("Rotation"))
			{
				const auto& rotation = component.value["Rotation"].GetArray();
				const float x = rotation[0].GetFloat() * Math::Constants::DegToRad;
				const float y = rotation[1].GetFloat() * Math::Constants::DegToRad;
				const float z = rotation[2].GetFloat() * Math::Constants::DegToRad;
				transform->rotation = Math::Quaternion::RotationEuler({ x,y,z });
			}
			if (component.value.HasMember("Scale"))
			{
				const auto& scale = component.value["Scale"].GetArray();
				const float x = scale[0].GetFloat();
				const float y = scale[1].GetFloat();
				const float z = scale[2].GetFloat();
				transform->scale = { x,y,z };
			}
		}
		else if (strcmp(componentName, "CameraComponent") == 0)
		{
			auto camera = gameObject.AddComponent<CameraComponent>();
		}
		else if (strcmp(componentName, "FPSCameraControllerComponent") == 0)
		{
			auto camera = gameObject.AddComponent<FPSCameraControllerComponent>();
		}
		else if (strcmp(componentName, "FollowCameraControllerComponent") == 0)
		{
			auto camera = gameObject.AddComponent<FollowCameraControllerComponent>();
			if (component.value.HasMember("Target"))
			{
				const auto& targetName = component.value["Target"].GetString();
				camera->SetTarget(targetName);
			}
			if (component.value.HasMember("Distance"))
			{
				const auto& distance = component.value["Distance"].GetFloat();
				camera->SetDistance(distance);
			}
			if (component.value.HasMember("CameraElevation"))
			{
				const auto& cameraElevation = component.value["CameraElevation"].GetFloat();
				camera->SetCameraElevation(cameraElevation);
			}
			if (component.value.HasMember("LookElevation"))
			{
				const auto& lookElevation = component.value["LookElevation"].GetFloat();
				camera->SetLookElevation(lookElevation);
			}
		}
		else if (strcmp(componentName, "RigidBodyComponent") == 0)
		{
			auto transform = gameObject.AddComponent<RigidBodyComponent>();
			if (component.value.HasMember("Force"))
			{
				const auto& force = component.value["Force"].GetArray();
				const float x = force[0].GetFloat();
				const float y = force[1].GetFloat();
				const float z = force[2].GetFloat();
				transform->SetForce({ x, y, z });
			}
		}
		//else if (strcmp(componentName, "CameraComponent") == 0)
		//{
		//	auto camera = gameObject.AddComponent<CameraComponent>();
		//	if (component.value.HasMember("FOV"))
		//	{
		//		const auto& fov = component.value["FOV"].GetFloat();
		//		auto rfov = fov * Math::Constants::DegToRad;
		//		camera->SetFov(rfov);
		//	}
		//	if (component.value.HasMember("AspectRatio"))
		//	{
		//		const auto& aspectRatio = component.value["AspectRatio"].GetFloat();
		//		camera->SetAspectRatio(aspectRatio);
		//	}
		//	if (component.value.HasMember("NearPlane"))
		//	{
		//		const auto& nearPlane = component.value["NearPlane"].GetFloat();
		//		camera->SetNearPlane(nearPlane);
		//	}
		//	if (component.value.HasMember("FarPlane"))
		//	{
		//		const auto& farPlane = component.value["FarPlane"].GetFloat();
		//		camera->SetFarPlane(farPlane);
		//	}
		//	if (component.value.HasMember("Position"))
		//	{
		//		const auto& position = component.value["Position"].GetArray();
		//		const float x = position[0].GetFloat();
		//		const float y = position[1].GetFloat();
		//		const float z = position[2].GetFloat();
		//		camera->SetPosition({x,y,z});
		//	}
		//	if (component.value.HasMember("LookAt"))
		//	{
		//		const auto& lookAt = component.value["LookAt"].GetArray();
		//		const float x = lookAt[0].GetFloat();
		//		const float y = lookAt[1].GetFloat();
		//		const float z = lookAt[2].GetFloat();
		//		camera->SetLookAt({ x,y,z });
		//	}
		//	if (component.value.HasMember("Mode"))
		//	{
		//		const auto& mode = component.value["Mode"].GetString();
		//		if (strcmp(mode, "Orthographic") == 0)
		//		{
		//			camera->SetMode(Graphics::Camera::ProjectionMode::Orthographic);
		//		}
		//		if (strcmp(mode, "Perspective") == 0)
		//		{
		//			camera->SetMode(Graphics::Camera::ProjectionMode::Perspective);
		//		}
		//		
		//	}
		//	if (component.value.HasMember("Size"))
		//	{
		//		const auto& size = component.value["Size"].GetArray();
		//		const float x = size[0].GetFloat();
		//		const float y = size[1].GetFloat();
		//		camera->SetSize(x,y);
		//	}
		//}
	}

}

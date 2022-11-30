#include "Precompiled.h"
#include "RenderService.h"

#include "AnimatorComponent.h"
#include "CameraService.h"
#include "GameObject.h"
#include "GameWorld.h"
#include "ModelComponent.h"
#include "TerrainService.h"
#include "TransformComponent.h"

using namespace Storm;
using namespace Storm::Input;
using namespace Storm::Graphics;
using namespace Storm::Math;

void RenderService::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Gray);

	mCameraService = GetWorld().GetService<CameraService>();
	mTerrainService = GetWorld().GetService<TerrainService>();

	mDirectionLight.direction = Math::Normalize({ 1.0f,-1.0f,1.0f });
	mDirectionLight.ambient = { 0.1f,0.1f,0.1f,1.0f };
	mDirectionLight.diffuse = { 0.75f,0.75f,0.75f,1.0f };
	mDirectionLight.specular = { 0.3f,0.3f,0.3f,1.0f };


	mStandardEffect.Initialize();
	mStandardEffect.SetLightCamera(&mShadowEffect.GetLightCamera());
	mStandardEffect.SetDirectionalLight(mDirectionLight);
	mStandardEffect.SetShadowMap(&mShadowEffect.GetDepthMap());
	mStandardEffect.UseDisplacementFlag() = true;
	mStandardEffect.UseNorFlag() = true;
	mStandardEffect.UseSpecularFlag() = true;
	mStandardEffect.UseShadowFlag() = true;
	mStandardEffect.DisplacementWeight() = 0.3f;

	mTexturingEffect.Initialize();

	mTerrainEffect.Initialize();
	mTerrainEffect.SetLightCamera(&mShadowEffect.GetLightCamera());
	mTerrainEffect.SetDirectionalLight(mDirectionLight);
	mTerrainEffect.SetShadowMap(&mShadowEffect.GetDepthMap());
	mTerrainEffect.UseShadowFlag() = true;
	mTerrainEffect.DepthBias() = 0.000222f;

	mShadowEffect.Initialize();
	mShadowEffect.SetDirectionalLight(mDirectionLight);
	mShadowEffect.SetSize(200.0f);

	//LoadSkydome("../../Assets/Images/skydome1.jpg");

}

void RenderService::Terminate()
{
	mSkydome.Terminate();
	mShadowEffect.Terminate();
	mTerrainEffect.Terminate();
	mTexturingEffect.Terminate();
	mStandardEffect.Terminate();
}

void RenderService::Update(float deltaTime)
{
	mFPS = 1.0f / deltaTime;
}

void RenderService::Render()
{
	auto& camera = mCameraService->GetCamera();
	mStandardEffect.SetCamera(camera);
	mTexturingEffect.SetCamera(camera);
	mTerrainEffect.SetCamera(camera);

	for (auto& entry : mRenderEntries)
	{
		for (auto& renderObject : entry.renderGroup)
		{
			auto angles = entry.modelComponent->GetRotation();
			auto rotation = Math::Quaternion::RotationEuler(angles);
			auto transform = *(static_cast<const Graphics::Transform*>(entry.transformComponent));
			transform.rotation = rotation * transform.rotation;
			renderObject.transform = transform;
		}
	}

	if (mSkydome.diffuseMapId != 0)
	{
		mTexturingEffect.Begin();
		mTexturingEffect.Render(mSkydome);
		mTexturingEffect.End();
	}

	mShadowEffect.Begin();
	for (auto& entry : mRenderEntries)
	{
		mShadowEffect.RenderGroup(entry.renderGroup);
	}
	if (mTerrainService)
	{
		mShadowEffect.Render(mTerrainService->GetTerrainRenderObject());
	}
	mShadowEffect.End();

	mStandardEffect.Begin();
	for (auto& entry : mRenderEntries)
	{

		mStandardEffect.RenderGroup(entry.renderGroup);

	}
	mStandardEffect.End();

	if (mTerrainService)
	{
		mTerrainEffect.Begin();
		mTerrainEffect.Render(mTerrainService->GetTerrainRenderObject());
		mTerrainEffect.End();
	}

	SimpleDraw::AddGroundPlane(20.0f, Colors::LightGray);
	SimpleDraw::AddTransform(Matrix4::Translation({ 0.0f, 0.01f, 0.0f }));
	SimpleDraw::Render(camera);

}

void RenderService::LoadSkydome(const char* fileName)
{
	auto tm = TextureManager::Get();
	mSkydome.diffuseMapId = tm->LoadTexture(fileName);
	mSkydome.meshBuffer.Initialize(MeshBuilder::CreateSkyDome(256, 256, 500.0f));
}

void RenderService::SetShadowFocus(const Math::Vector3& focusPosition)
{
	mShadowEffect.SetFocus(focusPosition);
}

void RenderService::Register(const ModelComponent* modelComponent)
{
	Entry& entry = mRenderEntries.emplace_back();
	auto& gameObject = modelComponent->GetOwner();

	entry.animatorComponent = gameObject.GetComponent<AnimatorComponent>();
	entry.modelComponent = modelComponent;
	entry.transformComponent = gameObject.GetComponent<TransformComponent>();

	const IAnimator* animator = nullptr;
	if (entry.animatorComponent != nullptr)
	{
		animator = &entry.animatorComponent->GetAnimator();
	}
	entry.renderGroup = CreateRenderGroup(modelComponent->GetModel(), animator);
}

void RenderService::Unregister(const ModelComponent* modelComponent)
{
	auto match = [&](const auto& entry) {return entry.modelComponent == modelComponent; };

	auto iter = std::find_if(mRenderEntries.begin(), mRenderEntries.end(), match);
	if (iter != mRenderEntries.end())
	{
		Entry& entry = *iter;
		CleanUpRenderGroup(entry.renderGroup);
		mRenderEntries.erase(iter);
	}
}


void RenderService::DebugUI()
{
	ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("fps: %f", mFPS);

	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool directionChanged = false;
		directionChanged |= ImGui::DragFloat("Direction X##Light", &mDirectionLight.direction.x, 0.01f, -1.0f, 1.0f);
		directionChanged |= ImGui::DragFloat("Direction Y##Light", &mDirectionLight.direction.y, 0.01f, -1.0f, 1.0f);
		directionChanged |= ImGui::DragFloat("Direction Z##Light", &mDirectionLight.direction.z, 0.01f, -1.0f, 1.0f);
		if (directionChanged)
		{
			mDirectionLight.direction = Normalize(mDirectionLight.direction);
		}
		ImGui::ColorEdit4("Ambient##Light", &mDirectionLight.ambient.r);
		ImGui::ColorEdit4("Diffuse##Light", &mDirectionLight.diffuse.r);
		ImGui::ColorEdit4("Specular##Light", &mDirectionLight.specular.r);
	}
	if (ImGui::CollapsingHeader("Shadow Settings"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		ImGui::Checkbox("Use Specular Map", &mStandardEffect.UseSpecularFlag());
		ImGui::Checkbox("Use Displacement Map", &mStandardEffect.UseDisplacementFlag());
		ImGui::Checkbox("Use Normal Map", &mStandardEffect.UseNorFlag());

		ImGui::DragFloat("Displacment Weight", &mStandardEffect.DisplacementWeight(), 0.01f, 0.0f, 1.0f);

		ImGui::DragFloat("DepthBias", &mStandardEffect.DepthBias(), 0.00001f, 0.0f, 1.0f, "%.5f");
	}


	ImGui::End();
}

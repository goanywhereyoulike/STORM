#include "GameState.h"


using namespace Storm;
using namespace Storm::Graphics;
using namespace Storm::Input;
using namespace Storm::Math;
using namespace Storm::Physics;

namespace
{
	bool OnMake(const char* componentName, const rapidjson::Value& value, GameObject& gameObject)
	{
		return false;
	}
}

void GameState::Initialize()
{
	mGameWorld.AddService<RenderService>();
	mGameWorld.AddService<CameraService>();
	mGameWorld.Initialize(1000);
	GameObjectFactory::SetMakeOverride(OnMake);
	mGameWorld.CreateGameObject("../../Assets/Templates/fps_camera.json");
	auto gameObject = mGameWorld.CreateGameObject("../../Assets/Templates/city.json");

	mGameObjectHandle = gameObject->GetHandle();
	auto& camera = mGameWorld.GetService<CameraService>()->GetCamera();
	auto& directionLight = mGameWorld.GetService<RenderService>()->GetLight();
	mStandardEffect.Initialize();
	mStandardEffect.SetCamera(camera);
	mStandardEffect.SetLightCamera(&mShadowEffect.GetLightCamera());
	mStandardEffect.SetDirectionalLight(directionLight);
	mStandardEffect.SetShadowMap(&mShadowEffect.GetDepthMap());
	mStandardEffect.UseDisplacementFlag() = true;
	mStandardEffect.UseNorFlag() = true;
	mStandardEffect.UseSpecularFlag() = true;
	mStandardEffect.UseShadowFlag() = true;
	mStandardEffect.DisplacementWeight() = 0.3f;

	mShadowEffect.Initialize();
	mShadowEffect.SetDirectionalLight(directionLight);
	//mGameObjectHandles.push_back(mGameObjectHandle);
	const char* modelFileName = "../../Assets/Models/Animation/Dance.model";
	ModelIO::LoadModel(modelFileName, model);
	ModelIO::LoadMaterial(modelFileName, model);
	ModelIO::LoadSkeleton(modelFileName, model);
	ModelIO::LoadAnimationSet(modelFileName, model);
	modelAnimator.Initialize(&model);
	modelAnimator.PlayAnimation(0);
	mHuman = CreateRenderGroup(model, &modelAnimator);
	mCullNone.Initialize(RasterizerState::CullMode::None, RasterizerState::FillMode::Solid);
	mWireframe.Initialize(RasterizerState::CullMode::Back, RasterizerState::FillMode::Wireframe);


}

void GameState::Terminate()
{
	mCullNone.Terminate();
	mWireframe.Terminate();
	CleanUpRenderGroup(mHuman);
	mGameWorld.Terminate();
	mShadowEffect.Terminate();
	mStandardEffect.Terminate();
}

void GameState::Update(float deltaTime)
{
	mGameWorld.Update(deltaTime);
	mFPS = 1.0f / deltaTime;

	modelAnimator.Update(animationspeed * deltaTime);
}

void GameState::Render()
{
	for (auto& handle : mGameObjectHandles)
	{
		auto gameObject = mGameWorld.GetGameObject(handle);
		if (gameObject != nullptr)
		{
			gameObject->DebugUI();
		}
	}
	//mBaseRenderTarget.BeginRender();
	mGameWorld.Render();

	for (auto& renderObject : mHuman)
	{
		renderObject.transform.position = mPosition;
		renderObject.transform.scale = Storm::Math::Vector3::One * mScale;
	}

	mStandardEffect.Begin();
	if (mDrawSkeleton)
	{
		std::vector<Matrix4> boneTransforms;
		//AnimationUtil::ComputeBindPose(*model.skeleton, boneTransforms);
		AnimationUtil::ComputeBoneTransforms(*model.skeleton, boneTransforms,
			[animator = &modelAnimator](const Bone* bone)
			{
				return animator->GetTransform(bone);
			});
	
		AnimationUtil::DrawSkeleton(*model.skeleton, boneTransforms);
	}
	else
	{
		mStandardEffect.RenderGroup(mHuman);
	}
	mStandardEffect.End();

	mShadowEffect.Begin();
	mShadowEffect.RenderGroup(mHuman);
	mShadowEffect.End();
	//mBaseRenderTarget.EndRender();
	//mPostProcessEffect.Begin();
	//mPostProcessEffect.Render(mScreen);
	//mPostProcessEffect.End();
}

void GameState::DebugUI()
{
	mGameWorld.DebugUI();
	ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::CollapsingHeader("Transform"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		ImGui::DragFloat3("Position", (float*)&mPosition, 0.1f);

	}
	ImGui::Checkbox("Draw Skeleton", &mDrawSkeleton);
	ImGui::Separator();
	ImGui::Text("AnimationSpeed: %f", animationspeed);
	ImGui::DragFloat("Animation Speed", &animationspeed, 0.00001f, 0.0f, 0.5f, "%.5f");
	ImGui::End();
}

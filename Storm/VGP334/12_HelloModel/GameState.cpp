#include "GameState.h"

//-scale 0.1 ../../Assets/Models/Mousey_Dance/Dancing.fbx ../../Assets/Models/Mousey_Dance/Dancing.model
//-scale 0.1 ../../Assets/Models/Jumping.fbx ../../Assets/Models/Jumping.model
//-scale 0.1 ../../Assets/Models/Fighter/Fighter.fbx ../../Assets/Models/Fighter/Fighter.model
//-scale 0.1 ../../Assets/Models/Young/Young.fbx ../../Assets/Models/Young/Young.model
//-scale 0.1 ../../Assets/Models/Duck/duck.fbx ../../Assets/Models/Duck/duck.model
// //-scale 0.1 ../../Assets/Models/Dancing/Dancing.fbx ../../Assets/Models/Dancing/Dancing.model
//-scale 0.1 ../../Assets/Models/Spaceship/spaceship.fbx ../../Assets/Models/Spaceship/spaceship.model
//-scale 0.1 ../../Assets/Models/Soldier/Soldier.fbx ../../Assets/Models/Soldier/Soldier.model
//-scale 0.1 ../../Assets/Models/Ball/Ball.fbx ../../Assets/Models/Ball/Ball.model
//-scale 0.1 -animOnly ../../Assets/Models/Soldier/Soldier_Running.fbx ../../Assets/Models/Soldier/Soldier_Running.model
//-scale 0.1 -animOnly ../../Assets/Models/Soldier/Soldier_Walking.fbx ../../Assets/Models/Soldier/Soldier_Walking.model
//-scale 0.1 ../../Assets/Models/Vehicle/vehicle.fbx ../../Assets/Models/Vehicle/vehicle.model
//-scale 0.001 ../../Assets/Models/City/city.fbx ../../Assets/Models/City/city.model
//-scale 0.1 ../../Assets/Models/Tank/tank.fbx ../../Assets/Models/Tank/tank.model
//-scale 0.1 ../../Assets/Models/Scene/scene.fbx ../../Assets/Models/Scene/scene.model starcraft
//-scale 0.1 ../../Assets/Models/Starcraft/starcraft.fbx ../../Assets/Models/Starcraft/starcraft.model 
//-scale 0.1 ../../Assets/Models/spacefighter/spacefighter.fbx ../../Assets/Models/spacefighter/spacefighter.model
//-scale 0.1 ../../Assets/Models/Animation/Dance.fbx ../../Assets/Models/Animation/Dance.model
using namespace Storm::Graphics;
using namespace Storm::Input;
using namespace Storm::Math;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::SteelBlue);
	mPosition = { 0.0f,0.1f,0.0f };
	mRotation = { 0.0f,0.0f,0.0f };
	mCamera.SetPosition(Vector3{ 0.0f,30.0f,-40.0f });
	mCamera.SetLookAt(mPosition);
	mCamera.SetMode(Camera::ProjectionMode::Perspective);
	mCamera.SetSize(40.0f, 20.0f);

	mStandardEffect.Initialize();
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetLightCamera(&mShadowEffect.GetLightCamera());
	mStandardEffect.SetDirectionalLight(mDirectionLight);
	mStandardEffect.SetShadowMap(&mShadowEffect.GetDepthMap());
	mStandardEffect.UseDisplacementFlag() = true;
	mStandardEffect.UseNorFlag() = true;
	mStandardEffect.UseSpecularFlag() = true;
	mStandardEffect.UseShadowFlag() = true;
	mStandardEffect.DisplacementWeight() = 0.3f;

	mShadowEffect.Initialize();
	mShadowEffect.SetDirectionalLight(mDirectionLight);
	mTexturingEffect.Initialize();
	mTexturingEffect.SetCamera(mCamera);

	const char* modelFileName = "../../Assets/Models/Animation/Dance.model";
	ModelIO::LoadModel(modelFileName, model);
	ModelIO::LoadMaterial(modelFileName, model);
	ModelIO::LoadSkeleton(modelFileName, model);
	ModelIO::LoadAnimationSet(modelFileName, model);
	//ModelIO::LoadAnimationSet("../../Assets/Models/Soldier/Soldier_idle.animation", model);
	//ModelIO::LoadAnimationSet("../../Assets/Models/Soldier/Soldier_Walking.animation", model);
	//ModelIO::LoadAnimationSet("../../Assets/Models/Soldier/Soldier_Running.animation", model);
	//mHuman.transform.rotation = Storm::Math::Quaternion::RotationEuler(mRotation);

	//auto dance=modelAnimator.AddAnimator();
	//dance->Initialize(&model);
	//dance->PlayAnimation(0);
	//auto idle = modelAnimator.AddAnimator();
	//idle->Initialize(&model);
	//idle->PlayAnimation(0);

	//auto walk = modelAnimator.AddAnimator();
	//walk->Initialize(&model);
	//walk->PlayAnimation(1);

	//auto run = modelAnimator.AddAnimator();
	//run->Initialize(&model);
	//run->PlayAnimation(2);
	modelAnimator.Initialize(&model);
	modelAnimator.PlayAnimation(0);
	mHuman = CreateRenderGroup(model,&modelAnimator);

	//for (auto& renderObject : mHuman)
	//{
	//	renderObject.material.ambient = renderObject.material.diffuse;
	//}
	// 
	// 
	//mHuman.material = model.materialData[0].material;
	//if (!model.materialData[0].diffuseMapName.empty())
	//{
	//	mHuman.diffuseMap.Initialize(std::string("../../Assets/Models/") + model.materialData[0].diffuseMapName);
	//}
	//if (!model.materialData[0].normalMapName.empty())
	//{
	//	mHuman.normalMap.Initialize(std::string("../../Assets/Models/") + model.materialData[0].normalMapName);
	//}
	//if (!model.materialData[0].specularMapName.empty())
	//{
	//	mHuman.specularMap.Initialize(std::string("../../Assets/Models/") + model.materialData[0].specularMapName);
	//}
	//mHuman.meshBuffer.Initialize(model.meshData[0].mesh);
	//mHuman.skeleton = model.skeleton.get();
	auto tm = TextureManager::Get();
	mGround.material.ambient = { 1.0f,1.0f,1.0f,1.0f };
	mGround.material.diffuse = { 0.75f,0.75f,0.75f,1.0f };
	mGround.material.specular = { 1.0f,1.0f,1.0f,1.0f };
	mGround.material.power = 10.0f;
	mGround.diffuseMapId = tm->LoadTexture("../../Assets/Images/ground/diffuse.jpg");
	mGround.normalMapId = tm->LoadTexture("../../Assets/Images/ground/normal.jpg");
	mGround.displacementMapId = tm->LoadTexture("../../Assets/Images/ground/glossiness.jpg");
	mGround.specularMapId = tm->LoadTexture("../../Assets/Images/ground/ao.jpg");
	//mGround.diffuseMap.Initialize("../../Assets/Images/ground.jpg");
	mGround.meshBuffer.Initialize(MeshBuilder::CreatePlane(100, 100, 5.0f, 10.0f));

	mDirectionLight.direction = Normalize({ 1.0f,-1.0f,1.0f });
	mDirectionLight.ambient = { 0.1f,0.1f,0.1f,1.0f };
	mDirectionLight.diffuse = { 0.75f,0.75f,0.75f,1.0f };
	mDirectionLight.specular = { 0.3f,0.3f,0.3f,1.0f };

	mSkydome.diffuseMapId = tm->LoadTexture("../../Assets/Images/cSky.png");
	mSkydome.meshBuffer.Initialize(MeshBuilder::CreateSkyDome(512, 512, 1000.0f));

	//for (auto& renderobject : mHuman)
	//{
	//	renderobject.transform.scale *= 0.1f;
	//}

	mCullNone.Initialize(RasterizerState::CullMode::None, RasterizerState::FillMode::Solid);
	mWireframe.Initialize(RasterizerState::CullMode::Back, RasterizerState::FillMode::Wireframe);

}

void GameState::Terminate()
{
	mCullNone.Terminate();
	mWireframe.Terminate();
	mGround.Terminate();
	mSkydome.Terminate();
	CleanUpRenderGroup(mHuman);
	mShadowEffect.Terminate();
	mStandardEffect.Terminate();
	mTexturingEffect.Terminate();

}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	//Camera control---------------------------------
	const float movespeed = inputSystem->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
	const float turnspeed = 0.3f;
	if (inputSystem->IsKeyDown(KeyCode::W))
	{

		mCamera.Walk(movespeed * deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::A))
	{
		mCamera.Strafe(-movespeed * deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::S))
	{
		mCamera.Walk(-movespeed * deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::D))
	{
		mCamera.Strafe(movespeed * deltaTime);
	}

	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{

		mCamera.Yaw(inputSystem->GetMouseMoveX() * turnspeed * deltaTime);
		mCamera.Pitch(inputSystem->GetMouseMoveY() * turnspeed * deltaTime);

	}
	mFPS = 1.0f / deltaTime;

	if (inputSystem->IsKeyDown(KeyCode::UP))
	{
		mSpeed += deltaTime;
	}
	else
	{
		mSpeed -= deltaTime;
	}
	mSpeed = Clamp(mSpeed, 0.0f, 10.0f);

	//if (mSpeed <= 5.0f)
	//{
	//	modelAnimator.SetBlendWeight(1, mSpeed / 10.0f);
	//	modelAnimator.SetBlendWeight(0, 0.5f - mSpeed/10.0f);
	//	modelAnimator.SetBlendWeight(2, mSpeed / 10.0f - 0.5f);
	//}
	//else if (mSpeed > 5.0f)
	//{
	//	modelAnimator.SetBlendWeight(2, mSpeed / 10.0f);
	//	modelAnimator.SetBlendWeight(0, 0.5f - mSpeed / 10.0f);
	//	modelAnimator.SetBlendWeight(1, 1.0-mSpeed/10.0f);
	//	
	//}

	//modelAnimator.SetBlendWeight(2, mSpeed);


	mSkydome.transform.position = mPosition;
	modelAnimator.Update(animationspeed * deltaTime);

}

void GameState::Render()
{
	//Draw depth map
	mShadowEffect.Begin();
	mShadowEffect.RenderGroup(mHuman);
	mShadowEffect.End();

	if (mSkydome.diffuseMapId != 0)
	{
		mTexturingEffect.Begin();
		mTexturingEffect.Render(mSkydome);
		mTexturingEffect.End();
	}

	//Draw scene
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

	mStandardEffect.Render(mGround);
	mStandardEffect.End();

	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("fps: %f", mFPS);

	if (ImGui::CollapsingHeader("Camera"), ImGuiTreeNodeFlags_DefaultOpen)
	{

		ImGui::Image(mShadowEffect.GetDepthMap().GetRawData(),
			{ 150.0f,150.0f },
			{ 0.0f,0.0f },
			{ 1.0f,1.0f },
			{ 1.0f,1.0f,1.0f,1.0f },
			{ 1.0f,1.0f,1.0f,1.0f });

	}

	//Material and light control
	ImGui::PushID("Light");
	if (ImGui::CollapsingHeader("Light"), ImGuiTreeNodeFlags_DefaultOpen)
	{
	
		ImGui::DragFloat("Direction X", &mDirectionLight.direction.x,0.01f);
		ImGui::DragFloat("Direction Y", &mDirectionLight.direction.y,0.01f);
		ImGui::DragFloat("Direction Z", &mDirectionLight.direction.z,0.01f);
		mDirectionLight.direction = Storm::Math::Normalize(mDirectionLight.direction);

		ImGui::ColorEdit4("Ambient", (float*)&mDirectionLight.ambient);
		ImGui::ColorEdit4("Diffuse", (float*)&mDirectionLight.diffuse);
		ImGui::ColorEdit4("Specular", (float*)&mDirectionLight.specular);


	}
	ImGui::PopID();
	ImGui::PushID("Material");

	//if (ImGui::CollapsingHeader("Material"), ImGuiTreeNodeFlags_DefaultOpen)
	//{

	//	ImGui::ColorEdit4("Ambient ", (float*)&mHuman.material.ambient);
	//	ImGui::ColorEdit4("Diffuse ", (float*)&mHuman.material.diffuse);
	//	ImGui::ColorEdit4("Specular ", (float*)&mHuman.material.specular);

	//	ImGui::DragFloat("Power", &mHuman.material.power);

	//}

	ImGui::PopID();
	ImGui::PushID("Settings");
	if (ImGui::CollapsingHeader("Settings"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		ImGui::Checkbox("Use Specular Map", &mStandardEffect.UseSpecularFlag());
		ImGui::Checkbox("Use Displacement Map", &mStandardEffect.UseDisplacementFlag());
		ImGui::Checkbox("Use Normal Map", &mStandardEffect.UseNorFlag());

		ImGui::DragFloat("Displacment Weight", &mStandardEffect.DisplacementWeight(), 0.01f, 0.0f, 1.0f);

		ImGui::DragFloat("DepthBias", &mStandardEffect.DepthBias(), 0.00001f, 0.0f, 1.0f, "%.5f");
	}

	ImGui::PopID();
	ImGui::PushID("Transform");
	if (ImGui::CollapsingHeader("Transform"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		ImGui::DragFloat3("Position", (float*)&mPosition, 0.1f);
		//ImGui::DragFloat3("Rotation", (float*)&mHuman.transform.rotation, 0.01f);
		//ImGui::DragFloat3("Revolution", (float*)&mRevolution, 0.01f);

	}
	ImGui::PopID();

	ImGui::Separator();
	//ImGui::Text("Depth: %d", mDepth);

	//if (ImGui::Button("Add"))
	//{
	//	mDepth++;
	//}
	//if (ImGui::Button("Subtract") && mDepth > 0)
	//{
	//	mDepth--;
	//}

	ImGui::Checkbox("Draw Skeleton", &mDrawSkeleton);

	if (ImGui::Checkbox("Wireframe", &Wireframe) || ImGui::Checkbox("CullNone", &CullNone))
	{
		if (CullNone)
		{
			mCullNone.Set();
		}
		else if (Wireframe)
		{
			mWireframe.Set();
		}
		else
		{
			RasterizerState::Clear();
		}

	}

	ImGui::Separator();
	ImGui::Text("AnimationSpeed: %f", animationspeed);
	ImGui::DragFloat("Animation Speed", &animationspeed, 0.00001f, 0.0f, 0.5f, "%.5f");

	
	ImGui::End();




}




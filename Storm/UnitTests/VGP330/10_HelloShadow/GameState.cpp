#include "GameState.h"

using namespace Storm::Graphics;
using namespace Storm::Input;
using namespace Storm::Math;

void GameState::Initialize()
{
	mPosition = { 0.0f,5.0f,0.0f };
	mCamera.SetPosition(Vector3{ 0.0f,8.0f,-8.0f });
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

	Model model;
	auto tm = TextureManager::Get();
	ModelIO::LoadModel("../../Assets/Models/Spaceship/spaceship.model", model);
	mSpaceShip.transform.rotation = Storm::Math::Quaternion::RotationEuler(Vector3(Constants::HalfPi));
	mSpaceShip.material.ambient = { 1.0f,1.0f,1.0f,1.0f };
	mSpaceShip.material.diffuse = { 0.75f,0.75f,0.75f,1.0f };
	mSpaceShip.material.specular = { 1.0f,1.0f,1.0f,1.0f };
	mSpaceShip.material.power = 10.0f;
	mSpaceShip.diffuseMapId = tm->LoadTexture("../../Assets/Models/Spaceship/spaceship_diffuse.jpg");
	mSpaceShip.normalMapId = tm->LoadTexture("../../Assets/Models/Spaceship/spaceship_normal.jpg");
	mSpaceShip.specularMapId = tm->LoadTexture("../../Assets/Models/Spaceship/spaceship_specular.jpg");
	//mSpaceShip.diffuseMap.Initialize("../../Assets/Models/Spaceship/spaceship_diffuse.jpg");
	//mSpaceShip.normalMap.Initialize("../../Assets/Models/Spaceship/spaceship_normal.jpg");
	//mSpaceShip.specularMap.Initialize("../../Assets/Models/Spaceship/spaceship_specular.jpg");
	mSpaceShip.meshBuffer.Initialize(model.meshData[0].mesh);

	mSkydome.diffuseMapId = tm->LoadTexture("../../Assets/Images/space2.jpg");
	mSkydome.meshBuffer.Initialize(MeshBuilder::CreateSkyDome(512, 512, 1000.0f));

	mEarth.material.ambient = { 1.0f,1.0f,1.0f,1.0f };
	mEarth.material.diffuse = { 0.75f,0.75f,0.75f,1.0f };
	mEarth.material.specular = { 1.0f,1.0f,1.0f,1.0f };
	mEarth.material.power = 10.0f;
	mEarth.diffuseMapId = tm->LoadTexture("../../Assets/Images/textures/planet2.tif");
	mEarth.specularMapId = tm->LoadTexture("../../Assets/Images/textures/planet_gloss.tif");
	mEarth.displacementMapId = tm->LoadTexture("../../Assets/Images/textures/planet_bump.jpg");
	mEarth.normalMapId = tm->LoadTexture("../../Assets/Images/textures/planet_normal.tif");

	mEarth.meshBuffer.Initialize(MeshBuilder::CreateSphere(1024, 1024, 10.0f));

	//mEarthCloud.diffuseMapId = tm->LoadTexture("../../Assets/Images/earth_clouds.jpg");
	mEarthCloud.diffuseMapId = tm->LoadTexture("../../Assets/Images/textures/clouds_diffuse.tif");
	mEarthCloud.specularMapId = tm->LoadTexture("../../Assets/Images/textures/clouds_Gloss.tif");
	mEarthCloud.displacementMapId = tm->LoadTexture("../../Assets/Images/textures/clouds_displace.tif");
	mEarthCloud.normalMapId = tm->LoadTexture("../../Assets/Images/textures/clouds_normal.tif");
	mEarthCloud.meshBuffer.Initialize(MeshBuilder::CreateSpherePX(1024, 1024, 11.0f));
	mEarthCloud.useAdditiveBlend = true;

	mGround.material.ambient = { 1.0f,1.0f,1.0f,1.0f };
	mGround.material.diffuse = { 0.75f,0.75f,0.75f,1.0f };
	mGround.material.specular = { 1.0f,1.0f,1.0f,1.0f };
	mGround.material.power = 10.0f;
	mGround.diffuseMapId = tm->LoadTexture("../../Assets/Images/ground.jpg");
	//mGround.diffuseMap.Initialize("../../Assets/Images/ground.jpg");
	mGround.meshBuffer.Initialize(MeshBuilder::CreatePlane(20, 20, 15.0f,10.0f));

	mDirectionLight.direction = Normalize({ 1.0f,-1.0f,1.0f });
	mDirectionLight.ambient = { 0.1f,0.1f,0.1f,1.0f };
	mDirectionLight.diffuse = { 0.75f,0.75f,0.75f,1.0f };
	mDirectionLight.specular = { 0.3f,0.3f,0.3f,1.0f };

	mEarth.transform.position = {-200.0f,0.0f,0.0f};
}

void GameState::Terminate()
{
	mEarthCloud.Terminate();
	mEarth.Terminate();
	mSkydome.Terminate();
	mGround.Terminate();
	mSpaceShip.Terminate();
	mShadowEffect.Terminate();
	mStandardEffect.Terminate();
	mTexturingEffect.Terminate();

}

void GameState::Update(float deltaTime)
{

	auto inputSystem = InputSystem::Get();
	//Camera control---------------------------------
	const float movespeed = inputSystem->IsKeyDown(KeyCode::LSHIFT) ? 50.0f : 10.0f;
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
	const float currentTime = Storm::Core::TimeUtil::GetTime();
	mSpaceShip.transform.position = mPosition;
	mSpaceShip.transform.position.y += sinf(currentTime) * 0.3f;
	mSpaceShip.transform.rotation = Storm::Math::Quaternion::RotationEuler(mRotation);

	mCloudRotation = deltaTime * 0.02f;

	//mEarth.transform.position = mPosition;
	mEarth.transform.rotation = Quaternion::RotationEuler(mRotation);

	mEarthCloud.transform.position = mEarth.transform.position;
	mEarthCloud.transform.rotation.y += mCloudRotation;

}

void GameState::Render()
{
	const int shipCount = 5;
	std::vector<Vector3> offsets(shipCount);
	for (int i = 0; i < shipCount; ++i)
	{
		float distance = 15.0f * i;
		float height = 5.0f * i;
		float angle = 3.0f * i;
		//offsets[i] = { cos(angle) * distance,height,sin(angle) * distance };
		offsets[i] = { distance,height, distance };
	}
	auto position = mSpaceShip.transform.position;
	//Draw depth map

	const Quaternion modelFixRotation = Quaternion::RotationEuler({ Constants::HalfPi,Constants::Pi,0.0f });
	mShadowEffect.Begin();

	for (int i = 0; i < shipCount; ++i)
	{
		mSpaceShip.transform.position = position + offsets[i];
		mSpaceShip.transform.rotation = modelFixRotation;
		mShadowEffect.Render(mSpaceShip);
	}

	mShadowEffect.End();
	//Draw scene
	mStandardEffect.Begin();
	mStandardEffect.Render(mEarth);
	for (int i = 0; i < shipCount; ++i)
	{
		mSpaceShip.transform.position = position + offsets[i];
		mStandardEffect.Render(mSpaceShip);
	}
	mStandardEffect.Render(mGround);
	mStandardEffect.End();

	mTexturingEffect.Begin();
	mTexturingEffect.Render(mEarthCloud);
	if (mSkydome.diffuseMapId != 0)
	{
		mTexturingEffect.Render(mSkydome);
	}
	mTexturingEffect.End();

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
		mDirectionLight.direction = Storm::Math::Normalize(mDirectionLight.direction);
		ImGui::DragFloat("Direction X", &mDirectionLight.direction.x,0.01f);
		ImGui::DragFloat("Direction Y", &mDirectionLight.direction.y,0.01f);
		ImGui::DragFloat("Direction Z", &mDirectionLight.direction.z,0.01f);

		ImGui::ColorEdit4("Ambient", (float*)&mDirectionLight.ambient);
		ImGui::ColorEdit4("Diffuse", (float*)&mDirectionLight.diffuse);
		ImGui::ColorEdit4("Specular", (float*)&mDirectionLight.specular);


	}
	ImGui::PopID();
	ImGui::PushID("Material");

	if (ImGui::CollapsingHeader("Material"), ImGuiTreeNodeFlags_DefaultOpen)
	{

		ImGui::ColorEdit4("Ambient ", (float*)&mSpaceShip.material.ambient);
		ImGui::ColorEdit4("Diffuse ", (float*)&mSpaceShip.material.diffuse);
		ImGui::ColorEdit4("Specular ", (float*)&mSpaceShip.material.specular);

		ImGui::DragFloat("Power", &mSpaceShip.material.power);

	}

	ImGui::PopID();
	ImGui::PushID("Settings");
	if (ImGui::CollapsingHeader("Settings"), ImGuiTreeNodeFlags_DefaultOpen)
	{

		//ImGui::DragFloat("Displacment Weight", &mStandardEffect.DisplacementWeight(), 0.01f, 0.0f, 1.0f);
		ImGui::Checkbox("Use Specular Map", &mStandardEffect.UseSpecularFlag());
		ImGui::Checkbox("Use Normal Map", &mStandardEffect.UseNorFlag());
		ImGui::Checkbox("Use Displacement Map", &mStandardEffect.UseDisplacementFlag());

		ImGui::DragFloat("Displacment Weight", &mStandardEffect.DisplacementWeight(), 0.1f, 0.0f, 1.0f);
		ImGui::DragFloat("DepthBias", &mStandardEffect.DepthBias(), 0.00001f, 0.0f, 1.0f, "%.5f");
	}

	ImGui::PopID();
	ImGui::PushID("Transform");
	if (ImGui::CollapsingHeader("Transform"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		ImGui::DragFloat3("Position", (float*)&mPosition, 0.1f);
		ImGui::DragFloat3("Rotation", (float*)&mRotation, 0.01f);
		//ImGui::DragFloat3("Revolution", (float*)&mRevolution, 0.01f);

	}
	ImGui::PopID();

	ImGui::End();
}




#include "GameState.h"

using namespace Storm::Graphics;
using namespace Storm::Input;
using namespace Storm::Math;

void GameState::Initialize()
{
	mPosition = { 0.0f,5.0f,0.0f };
	mCamera.SetPosition(Vector3{ 0.0f,0.0f,-40.0f });
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

	auto tm = TextureManager::Get();

	Model model;
	ModelIO::LoadModel("../../Assets/Models/Spaceship/spaceship.model", model);
	mSpaceShip.transform.rotation = Storm::Math::Quaternion::RotationEuler({ Constants::HalfPi,-Constants::HalfPi,0 });
	mSpaceShip.transform.scale = Vector3(0.3f);
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



	mGround.material.ambient = { 1.0f,1.0f,1.0f,1.0f };
	mGround.material.diffuse = { 0.75f,0.75f,0.75f,1.0f };
	mGround.material.specular = { 1.0f,1.0f,1.0f,1.0f };
	mGround.material.power = 10.0f;
	mGround.diffuseMapId = tm->LoadTexture("../../Assets/Images/ground.jpg");
	//mGround.diffuseMap.Initialize("../../Assets/Images/ground.jpg");
	mGround.meshBuffer.Initialize(MeshBuilder::CreatePlane(20, 20, 5.0f, 10.0f));

	mDirectionLight.direction = Normalize({ 1.0f,-1.0f,1.0f });
	mDirectionLight.ambient = { 0.1f,0.1f,0.1f,1.0f };
	mDirectionLight.diffuse = { 0.75f,0.75f,0.75f,1.0f };
	mDirectionLight.specular = { 0.3f,0.3f,0.3f,1.0f };

	Storm::Graphics::AnimationBuilder animationBuilder;
	for (int i = 0; i <= 100; ++i)
	{
		const float x = sin(i * Constants::TwoPi / 100.0f) * 20.0f;
		const float y = cos(i * Constants::TwoPi / 50.0f) * -5.0f;
		const float z = cos(i * Constants::TwoPi / 100.0f) * 20.0f;
		animationBuilder.AddPositionKey({ x,y,z, }, i / 100.0f * 8.0f);
	}
	animationBuilder.AddRotationKey(Quaternion::RotationEuler({ 0.0f,Constants::HalfPi,0.0f }), 0.0f);
	animationBuilder.AddRotationKey(Quaternion::RotationEuler({ 0.0f,Constants::Pi,Constants::HalfPi }), 1.0f);
	animationBuilder.AddRotationKey(Quaternion::RotationEuler({ 0.0f,Constants::Pi,Constants::HalfPi }), 2.0f);
	animationBuilder.AddRotationKey(Quaternion::RotationEuler({ -Constants::Pi,Constants::HalfPi,0.0f }), 3.0f);
	animationBuilder.AddRotationKey(Quaternion::RotationEuler({ -Constants::Pi,Constants::HalfPi,0.0f }), 4.0f);
	animationBuilder.AddRotationKey(Quaternion::RotationEuler({ 0.0f,0.0f ,-Constants::HalfPi }), 5.0f);
	animationBuilder.AddRotationKey(Quaternion::RotationEuler({ 0.0f,0.0f ,-Constants::HalfPi }), 6.0f);
	animationBuilder.AddRotationKey(Quaternion::RotationEuler({ 0.0f,Constants::HalfPi,0.0f }), 7.0f);
	animationBuilder.AddRotationKey(Quaternion::RotationEuler({ 0.0f,Constants::HalfPi,0.0f }), 8.0f);


	//animationBuilder.AddPositionKey({ 0.0f,5.0f,-5.0f }, 0.0f);
	//animationBuilder.AddPositionKey({ 0.0f,5.0f,0.0f }, 4.0f);
	//animationBuilder.AddPositionKey({ 0.0f,5.0f,5.0f }, 8.0f);

	//animationBuilder.AddRotationKey(Quaternion::RotationEuler({ 0.0f,0.0f,0.0f }), 0.0f);
	//animationBuilder.AddRotationKey(Quaternion::RotationEuler({ 0.0f,0.0f,1.0f }), 8.0f);
	animationBuilder.AddScaleKey({ 1,1,1 }, 0.0f);
	animationBuilder.AddScaleKey({ 1,1,1 }, 8.0f);

	mShipAnimation = animationBuilder.Get();
}

void GameState::Terminate()
{
	mGround.Terminate();
	mSpaceShip.Terminate();
	mShadowEffect.Terminate();
	mStandardEffect.Terminate();

}

void GameState::Update(float deltaTime)
{
	auto graphicsSystem = GraphicsSystem::Get();

	graphicsSystem->SetClearColor(Colors::Gray);



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

	//const float currentTime = Storm::Core::TimeUtil::GetTime();
	//mSpaceShip.transform.position = mPosition;
	//mSpaceShip.transform.position.y += sinf(currentTime) * 0.1f;
	//mSpaceShip.transform.rotation = Storm::Math::Quaternion::RotationEuler(mRotation);
	const float duration = mShipAnimation.GetDuration();
	if (timer > duration)
	{
		timer -= duration;
	}
	const Quaternion modelFixRotation = Quaternion::RotationEuler({ Constants::HalfPi,Constants::Pi,0.0f });

	mSpaceShip.transform.position = mShipAnimation.GetPosition(timer);
	mSpaceShip.transform.rotation = modelFixRotation * mShipAnimation.GetRotation(timer);
	timer += deltaTime;
	//mSpaceShip.transform.rotation = mShipAnimation.GetRotation(deltaTime * 8.0f);




}

void GameState::Render()
{
	//Draw depth map
	mShadowEffect.Begin();
	mShadowEffect.Render(mSpaceShip);
	mShadowEffect.End();
	//Draw scene
	mStandardEffect.Begin();
	mStandardEffect.Render(mSpaceShip);
	//mStandardEffect.Render(mGround);
	mStandardEffect.End();

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
		ImGui::DragFloat("Direction X", &mDirectionLight.direction.x);
		ImGui::DragFloat("Direction Y", &mDirectionLight.direction.y);
		ImGui::DragFloat("Direction Z", &mDirectionLight.direction.z);

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

		ImGui::DragFloat("Displacment Weight", &mStandardEffect.DisplacementWeight(), 0.01f, 0.0f, 1.0f);

		ImGui::DragFloat("DepthBias", &mStandardEffect.DepthBias(), 0.00001f, 0.0f, 1.0f, "%.5f");
	}

	ImGui::PopID();
	ImGui::PushID("Transform");
	if (ImGui::CollapsingHeader("Transform"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		ImGui::DragFloat3("Position", (float*)&mSpaceShip.transform.position, 0.1f);
		ImGui::DragFloat3("Rotation", (float*)&mSpaceShip.transform.rotation, 0.01f);
		//ImGui::DragFloat3("Revolution", (float*)&mRevolution, 0.01f);

	}
	ImGui::PopID();

	ImGui::End();




}




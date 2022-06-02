#include "GameState.h"

using namespace Storm::Graphics;
using namespace Storm::Input;
using namespace Storm::Math;

void GameState::Initialize()
{
	mCamera.SetPosition(Vector3{ 0.0f,2.0f,-15.0f });
	mCamera.SetDirection(Normalize(-mCamera.GetPosition()));

	auto graphicsSystem = GraphicsSystem::Get();
	const auto screenWidth = graphicsSystem->GetBackBufferWidth();
	const auto screenHeight = graphicsSystem->GetBackBufferHeight();
	mBaseRenderTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);
	mBloomRenderTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);


	mStandardEffect.Initialize();
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionLight);
	mStandardEffect.UseDisplacementFlag() = true;
	mStandardEffect.UseNorFlag() = true;
	mStandardEffect.UseSpecularFlag() = true;
	mStandardEffect.DisplacementWeight() = 0.3f;


	mTexturingEffect.Initialize();
	mTexturingEffect.SetCamera(mCamera);

	mGaussianBlurEffect.Initialize();
	mGaussianBlurEffect.SetSourceTexture(&mBloomRenderTarget);

	mPostProcessEffect.Initialize();
	mPostProcessEffect.SetMode(PostProcessEffect::Mode::Bloom);
	mPostProcessEffect.SetTexture(&mBaseRenderTarget, 0);
	mPostProcessEffect.SetTexture(&mGaussianBlurEffect.GetResultTexture(), 1);


	auto tm = TextureManager::Get();

	mEarth.material.ambient = { 1.0f,1.0f,1.0f,1.0f };
	mEarth.material.diffuse = { 0.75f,0.75f,0.75f,1.0f };
	mEarth.material.specular = { 1.0f,1.0f,1.0f,1.0f };
	mEarth.material.power = 10.0f;
	mEarth.diffuseMapId = tm->LoadTexture("../../Assets/Images/earth.jpg");
	mEarth.specularMapId = tm->LoadTexture("../../Assets/Images/earth_spec.jpg");
	mEarth.displacementMapId = tm->LoadTexture("../../Assets/Images/earth_bump.jpg");
	mEarth.normalMapId = tm->LoadTexture("../../Assets/Images/earth_normal.jpg");

	//mEarth.diffuseMap.Initialize("../../Assets/Images/earth.jpg");
	//mEarth.specularMap.Initialize("../../Assets/Images/earth_spec.jpg");
	//mEarth.displacementMap.Initialize("../../Assets/Images/earth_bump.jpg");
	//mEarth.normalMap.Initialize("../../Assets/Images/earth_normal.jpg");
	mEarth.meshBuffer.Initialize(MeshBuilder::CreateSphere(512, 512, 1.0f));

	mEarthNoTexture.meshBuffer.Initialize(MeshBuilder::CreateSphere(512, 512, 1.0f));

	mEarthCloud.diffuseMapId = tm->LoadTexture("../../Assets/Images/earth_clouds.jpg");
	mEarthCloud.meshBuffer.Initialize(MeshBuilder::CreateSpherePX(512, 512, 1.3f));
	mEarthCloud.useAdditiveBlend = true;

	mSun.diffuseMapId = tm->LoadTexture("../../Assets/Images/sun.jpg");
	mSun.meshBuffer.Initialize(MeshBuilder::CreateSpherePX(512, 512, 1.5f));

	Storm::Graphics::MeshPX mScreenMesh;
	mScreenMesh.vertices.push_back({ {-1.0f, -1.0f, 0.0f },{0.0f,1.0f} });
	mScreenMesh.vertices.push_back({ {-1.0f,  1.0f, 0.0f },{0.0f,0.0f} });
	mScreenMesh.vertices.push_back({ { 1.0f,  1.0f, 0.0f },{1.0f,0.0f} });
	mScreenMesh.vertices.push_back({ { 1.0f, -1.0f, 0.0f },{1.0f,1.0f} });
	mScreenMesh.indices = { 0,1,2,0,2,3 };
	mScreen.meshBuffer.Initialize(mScreenMesh);

	Model model;

	ModelIO::LoadModel("../../Assets/Models/SpaceShip2/spaceship2.model", model);
	mSpaceShip.material.ambient = { 1.0f,1.0f,1.0f,1.0f };
	mSpaceShip.material.diffuse = { 0.75f,0.75f,0.75f,1.0f };
	mSpaceShip.material.specular = { 1.0f,1.0f,1.0f,1.0f };
	mSpaceShip.material.power = 10.0f;
	mSpaceShip.diffuseMapId = tm->LoadTexture("../../Assets/Models/SpaceShip2/SF_Corvette-F3_diffuse.jpg");
	mSpaceShip.normalMapId = tm->LoadTexture("../../Assets/Models/SpaceShip2/SF_Corvette-F3_normal.jpg");
	mSpaceShip.specularMapId = tm->LoadTexture("../../Assets/Models/SpaceShip2/SF_Corvette-F3_specular.jpg");
	mSpaceShip.meshBuffer.Initialize(model.meshData[0].mesh);

	mSpaceShipNoTexture.meshBuffer.Initialize(model.meshData[0].mesh);

	mDirectionLight.direction = Normalize({ 1.0f,-1.0f,1.0f });
	mDirectionLight.ambient = { 0.1f,0.1f,0.1f,1.0f };
	mDirectionLight.diffuse = { 0.75f,0.75f,0.75f,1.0f };
	mDirectionLight.specular = { 0.3f,0.3f,0.3f,1.0f };

	mPosition = { 0.0f,0.0f,-10.0f };
	mSun.transform.scale = Vector3(4.0f);
}

void GameState::Terminate()
{
	mSpaceShipNoTexture.Terminate();
	mSpaceShip.Terminate();
	mScreen.Terminate();
	mSun.Terminate();
	mEarthCloud.Terminate();
	mEarthNoTexture.Terminate();
	mEarth.Terminate();

	mPostProcessEffect.Terminate();
	mGaussianBlurEffect.Terminate();
	mTexturingEffect.Terminate();
	mStandardEffect.Terminate();

	mBloomRenderTarget.Terminate();
	mBaseRenderTarget.Terminate();

}

void GameState::Update(float deltaTime)
{

	auto inputSystem = InputSystem::Get();
	//Camera control---------------------------------
	const float movespeed = 10.0f;
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
	mCloudRoation = deltaTime * 0.02f;

	mEarth.transform.position = TransformCoord(mPosition, Matrix4::RotationY(mRevolution.y));
	mEarth.transform.rotation = Quaternion::RotationEuler(mRotation);

	mEarthNoTexture.transform = mEarth.transform;

	mEarthCloud.transform.position = mEarth.transform.position;
	mEarthCloud.transform.rotation.y += mCloudRoation;
	mSpaceShip.transform.position = mEarth.transform.position;
	mSpaceShip.transform.position.y = 3.0f;
	//mSpaceShip.transform.rotation.x = 30.0f;
	mSpaceShip.transform.rotation.y = -30.0f;
	mSpaceShip.transform.scale = Vector3(0.0025f);

	mSpaceShipNoTexture.transform = mSpaceShip.transform;
}

void GameState::Render()
{
	//Draw scene
	mBaseRenderTarget.BeginRender();

	mStandardEffect.Begin();
	mStandardEffect.Render(mSpaceShip);
	mStandardEffect.Render(mEarth);
	mStandardEffect.End();

	mTexturingEffect.Begin();
	mTexturingEffect.Render(mEarthCloud);
	mTexturingEffect.Render(mSun);
	mTexturingEffect.End();

	SimpleDraw::AddGroundPlane(30, Colors::White);
	SimpleDraw::Render(mCamera);

	mBaseRenderTarget.EndRender();

	//Render brightness texture
	mBloomRenderTarget.BeginRender();
	mTexturingEffect.Begin();
	mTexturingEffect.Render(mSpaceShipNoTexture);
	mTexturingEffect.Render(mEarthNoTexture);
	mTexturingEffect.Render(mSun);
	mTexturingEffect.End();
	mBloomRenderTarget.EndRender();

	////Applu blur
	mGaussianBlurEffect.Begin();
	mGaussianBlurEffect.Render(mScreen);
	mGaussianBlurEffect.End();

	// Post process
	mPostProcessEffect.Begin();
	mPostProcessEffect.Render(mScreen);
	mPostProcessEffect.End();
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("fps: %f", mFPS);

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

		ImGui::ColorEdit4("Ambient ", (float*)&mEarth.material.ambient);
		ImGui::ColorEdit4("Diffuse ", (float*)&mEarth.material.diffuse);
		ImGui::ColorEdit4("Specular ", (float*)&mEarth.material.specular);

		ImGui::DragFloat("Power", &mEarth.material.power);

	}

	ImGui::PopID();
	ImGui::PushID("Settings");
	if (ImGui::CollapsingHeader("Settings"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		ImGui::Checkbox("Use Specular Map", &mStandardEffect.UseSpecularFlag());
		ImGui::Checkbox("Use Displacement Map", &mStandardEffect.UseDisplacementFlag());
		ImGui::Checkbox("Use Normal Map", &mStandardEffect.UseNorFlag());


		//bool useBlurFlag = (mPostProcessEffect.UseBlurEffect() == 1);
		//if (ImGui::Checkbox("Use Blur effect", &useBlurFlag))
		//{
		//	mPostProcessEffect.SetMode(PostProcessEffect::Mode::Blur);
		//	mPostProcessEffect.UseBlurEffect() = useBlurFlag;
		//}
		//bool useEmbossEffect = (mPostProcessEffect.UseEmbossEffect() == 1);
		//if (ImGui::Checkbox("Use Emboss effect", &useEmbossEffect))
		//{
		//	mPostProcessEffect.SetMode(PostProcessEffect::Mode::Emboss);
		//	mPostProcessEffect.UseEmbossEffect() = useEmbossEffect;
		//}

		//static bool bloom = true;
		//if (ImGui::Checkbox("Bloom", &bloom))
		//{
		//	mPostProcessEffect.SetMode(PostProcessEffect::Mode::Bloom);
		//}
		//ImGui::DragFloat("Displacment Weight", &mStandardEffect.DisplacementWeight(), 0.01f, 0.0f, 1.0f);

		//ImGui::DragInt("Texture Dimensions", &mPostProcessEffect.TextureDimensions(), 1, 0, 500);
		//ImGui::DragFloat("Emboss amount", &mPostProcessEffect.Embossamount(), 0.1f, 0, 50);
	}

	ImGui::PopID();
	ImGui::PushID("Transform");
	if (ImGui::CollapsingHeader("Transform"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		ImGui::DragFloat3("Position", (float*)&mPosition, 0.1f);
		ImGui::DragFloat3("Rotation", (float*)&mRotation, 0.01f);
		ImGui::DragFloat3("Revolution", (float*)&mRevolution, 0.01f);

	}
	ImGui::PopID();
	ImGui::PushID("Blur Settings");
	if (ImGui::CollapsingHeader("Blur Settings"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		ImGui::DragInt("Blur Iterations", &mGaussianBlurEffect.BlurIterations(), 1, 1, 100);
		ImGui::DragFloat("Blur Saturation", &mGaussianBlurEffect.BlurSaturation(), 0.001f, 1.0f, 10.0f);

	}

	ImGui::PopID();
	ImGui::End();


	ImGui::Begin("Render Targets", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("Base");
	ImGui::Image(mBaseRenderTarget.GetRawData(), { 256,144 });
	ImGui::Text("Bloom");
	ImGui::Image(mBloomRenderTarget.GetRawData(), { 256,144 });
	ImGui::Text("Horizontal Blur");
	ImGui::Image(mGaussianBlurEffect.GetHorizontalBlurTexture().GetRawData(), { 256,144 });
	ImGui::Text("Vertical Blur ");
	ImGui::Image(mGaussianBlurEffect.GetVerticalBlurTexture().GetRawData(), { 256,144 });


	ImGui::End();



}




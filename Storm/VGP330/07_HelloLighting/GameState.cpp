#include "GameState.h"

using namespace Storm::Graphics;
using namespace Storm::Input;
using namespace Storm::Math;

void GameState::Initialize()
{
	mPosition = { 0.0f,15.0f,0.0f };
	mCamera.SetPosition(Vector3{ 0.0f,15.0f,-30.0f });
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

	mTexturingEffect.Initialize();
	mTexturingEffect.SetCamera(mCamera);

	auto tm = TextureManager::Get();

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

	mDirectionLight.direction = Normalize({ 1.0f,-1.0f,1.0f });
	mDirectionLight.ambient = { 0.1f,0.1f,0.1f,1.0f };
	mDirectionLight.diffuse = { 0.75f,0.75f,0.75f,1.0f };
	mDirectionLight.specular = { 0.3f,0.3f,0.3f,1.0f };

	mEarth.transform.position = { 0.0f,0.0f,0.0f };

	////TODO:Finish create sphere
	//mSphereMesh = MeshBuilder::CreateSphere(512, 512, 1.0f);
	//mSphereMeshBuffer.Initialize(mSphereMesh);

	////Clouds
	//mCloudsSphereMesh = MeshBuilder::CreateSpherePX(512, 512, 1.5f);
	//mCloudsSphereMeshBuffer.Initialize(mCloudsSphereMesh);

	//// Create constant buffer
	//mConstantBuffer.Initialize();


	//mVertexShader.Initialize<Vertex>("../../Assets/Shaders/Standard.fx");
	//mPixelShader.Initialize("../../Assets/Shaders/Standard.fx");

	//mTexturingVertexShader.Initialize<VertexPX>(L"../../Assets/Shaders/DoTexturing.fx");
	//mTexturingPixelShader.Initialize(L"../../Assets/Shaders/DoTexturing.fx");


	//mTransformBuffer.Initialize();
	//mLightBuffer.Initialize();
	//mMaterialBuffer.Initialize();
	//mSettingsBuffer.Initialize();

	//mDiffuseMap.Initialize("../../Assets/Images/earth.jpg");
	//mSpecularMap.Initialize("../../Assets/Images/earth_spec.jpg");
	//mDisplacementMap.Initialize("../../Assets/Images/earth_bump.jpg");
	//mCloudsMap.Initialize("../../Assets/Images/earth_clouds.jpg");
	//mNormalMap.Initialize("../../Assets/Images/earth_normal.jpg");
	//mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

	//mBlendstate.Initialize(Storm::Graphics::BlendState::Mode::Additive);

	//mDirectionLight.direction = Normalize({ 1.0f,-1.0f,1.0f });
	//mDirectionLight.ambient = { 0.1f,0.1f,0.1f,1.0f };
	//mDirectionLight.diffuse = { 0.75f,0.75f,0.75f,1.0f };
	//mDirectionLight.specular = { 0.3f,0.3f,0.3f,1.0f };
	//mMaterial.ambient = { 1.0f,1.0f,1.0f,1.0f };
	//mMaterial.diffuse = { 0.75f,0.75f,0.75f,1.0f };
	//mMaterial.specular = { 1.0f,1.0f,1.0f,1.0f };
	//mMaterial.power = 10.0f;
}

void GameState::Terminate()
{
	//mBlendstate.Terminate();
	//mSampler.Terminate();

	//mCloudsMap.Terminate();
	//mNormalMap.Terminate();
	//mDisplacementMap.Terminate();
	//mSpecularMap.Terminate();
	//mDiffuseMap.Terminate();

	//mSettingsBuffer.Terminate();
	//mMaterialBuffer.Terminate();
	//mLightBuffer.Terminate();
	//mTransformBuffer.Terminate();

	//mTexturingPixelShader.Terminate();
	//mTexturingVertexShader.Terminate();
	//mPixelShader.Terminate();
	//mVertexShader.Terminate();
	//mConstantBuffer.Terminate();
	//mCloudsSphereMeshBuffer.Terminate();
	//mSphereMeshBuffer.Terminate();
	mEarthCloud.Terminate();
	mEarth.Terminate();
	mStandardEffect.Terminate();
	mTexturingEffect.Terminate();
}

void GameState::Update(float deltaTime)
{

	auto inputSystem = InputSystem::Get();
	//if (inputSystem->IsKeyDown(KeyCode::LEFT))
	//	angleY += deltaTime;
	//if (inputSystem->IsKeyDown(KeyCode::RIGHT))
	//	angleY -= deltaTime;
	//if (inputSystem->IsKeyDown(KeyCode::UP))
	//	angleX -= deltaTime;
	//if (inputSystem->IsKeyDown(KeyCode::DOWN))
	//	angleX += deltaTime;
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
	mCloudRotation = deltaTime * 0.02f;
	//mEarth.transform.position = mPosition;
	mEarth.transform.position = mPosition;
	mEarth.transform.rotation = Quaternion::RotationEuler(mRotation);

	mEarthCloud.transform.position = mEarth.transform.position;
	mEarthCloud.transform.rotation.y += mCloudRotation;

}
void GameState::Render()
{
	SimpleDraw::AddGroundPlane(30, Colors::White);
	//mCloudsRotation.y += 0.0005f;
	//Matrix4 view = mCamera.GetViewMatrix();
	//Matrix4 proj = mCamera.GetProjectionMatrix();
	//Matrix4 world = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y) * Matrix4::RotationZ(mRotation.z) * Matrix4::Translation(mPosition);
	//Matrix4 Cloudsworld= Matrix4::RotationX(mCloudsRotation.x) * Matrix4::RotationY(mCloudsRotation.y) * Matrix4::RotationZ(mCloudsRotation.z) * Matrix4::Translation(mPosition);
	//mTransform.wvp = Transpose(world * view * proj);
	//mTransform.world = Transpose(world);
	//mTransform.viewPosition = mCamera.GetPosition();
	////Cloud transform
	//mTransform0.wvp = Transpose(Cloudsworld * view * proj);
	////Matrix4 wvp = Transpose(world * view * proj); // DX always want the transpos

	//mTransformBuffer.BindVS(0);
	//mTransformBuffer.BindPS(0);
	//mLightBuffer.BindVS(1);
	//mLightBuffer.BindPS(1);
	//mMaterialBuffer.BindVS(2);
	//mMaterialBuffer.BindPS(2);

	//mSettingsBuffer.Update(mSettingsData);
	//mSettingsBuffer.BindPS(3);
	//mSettingsBuffer.BindVS(3);

	//mVertexShader.Bind();
	//mPixelShader.Bind();

	//Storm::Graphics::DirectionalLight Light = mDirectionLight;
	//Light.direction = Normalize(Light.direction);

	//mTransformBuffer.Update(mTransform);
	//mLightBuffer.Update(Light);
	//mMaterialBuffer.Update(mMaterial);

	//mDiffuseMap.BindPS(0);
	//mSpecularMap.BindPS(1);
	//mDisplacementMap.BindVS(2);
	//mNormalMap.BindPS(3);

	//mSampler.BindVS(0);

	//mSphereMeshBuffer.Render();

	//mTexturingVertexShader.Bind();
	//mTexturingPixelShader.Bind();
	//mConstantBuffer.BindVS(0);
	//mConstantBuffer.Update(mTransform0);
	//mSampler.BindPS(0);
	//mCloudsMap.BindPS(0);

	//mBlendstate.Set();
	////mCloudsSphereMeshBuffer.Render();
	//mBlendstate.ClearState();
	mStandardEffect.Begin();
	mStandardEffect.Render(mEarth);
	mStandardEffect.End();

	mTexturingEffect.Begin();
	mTexturingEffect.Render(mEarthCloud);
	mTexturingEffect.End();
	SimpleDraw::Render(mCamera);

}

void GameState::DebugUI()
{
	ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("fps: %f", mFPS);

	if (ImGui::CollapsingHeader("Light"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		ImGui::DragFloat("Direction X", &mDirectionLight.direction.x);
		ImGui::DragFloat("Direction Y", &mDirectionLight.direction.y);
		ImGui::DragFloat("Direction Z", &mDirectionLight.direction.z);
		//Storm::Math::Normalize(mDirectionLight.direction);
		ImGui::ColorEdit4("Ambient", (float*)&mDirectionLight.ambient);
		ImGui::ColorEdit4("Diffuse", (float*)&mDirectionLight.diffuse);
		ImGui::ColorEdit4("Specular", (float*)&mDirectionLight.specular);
	}
	if (ImGui::CollapsingHeader("Material"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		ImGui::ColorEdit4("Ambient ", (float*)&mEarth.material.ambient);
		ImGui::ColorEdit4("Diffuse ", (float*)&mEarth.material.diffuse);
		ImGui::ColorEdit4("Specular ", (float*)&mEarth.material.specular);
		ImGui::DragFloat("Power", &mEarth.material.power);
	}
	if (ImGui::CollapsingHeader("Settings"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		ImGui::Checkbox("Use Specular Map", &mStandardEffect.UseSpecularFlag());
		ImGui::Checkbox("Use Normal Map", &mStandardEffect.UseNorFlag());
		ImGui::Checkbox("Use Displacement Map", &mStandardEffect.UseDisplacementFlag());

		ImGui::DragFloat("Displacment Weight", &mStandardEffect.DisplacementWeight(), 0.1f, 0.0f, 1.0f);
		ImGui::DragFloat("DepthBias", &mStandardEffect.DepthBias(), 0.00001f, 0.0f, 1.0f, "%.5f");
	}

	if (ImGui::CollapsingHeader("Transform"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		ImGui::DragFloat3("Position", (float*)&mPosition, 0.1f);
		ImGui::DragFloat3("Rotation", (float*)&mRotation, 0.01f);
	}
	//ImGui::PopID();
	//ImGui::PushID("Material");

	//if (ImGui::CollapsingHeader("Material"), ImGuiTreeNodeFlags_DefaultOpen)
	//{

	//	ImGui::ColorEdit4("Ambient ", (float*)&mMaterial.ambient);
	//	ImGui::ColorEdit4("Diffuse ", (float*)&mMaterial.diffuse);
	//	ImGui::ColorEdit4("Specular ", (float*)&mMaterial.specular);

	//	ImGui::DragFloat("Power", &mMaterial.power);

	//}

	//ImGui::PopID();
	//ImGui::PushID("Material");
	//if (ImGui::CollapsingHeader("Settings"), ImGuiTreeNodeFlags_DefaultOpen)
	//{
	//	bool useSpecularFlag = (mSettingsData.useSpecularMap == 1);
	//	if (ImGui::Checkbox("Use Specular Map", &useSpecularFlag))
	//	{
	//		mSettingsData.useSpecularMap = useSpecularFlag ? 1 : 0;

	//	}
	//	bool useDisplacementFlag = (mSettingsData.useDisplacementMap == 1);
	//	if (ImGui::Checkbox("Use Displacement Map", &useDisplacementFlag))
	//	{
	//		mSettingsData.useDisplacementMap = useDisplacementFlag ? 1 : 0;
	//	}

	//	bool useNorFlag= (mSettingsData.useNormalMap == 1);
	//	if (ImGui::Checkbox("Use Normal Map", &useNorFlag))
	//	{
	//		mSettingsData.useNormalMap = useNorFlag ? 1 : 0;
	//	}
	//	ImGui::DragFloat("Displacment Weight", &mSettingsData.displacementWeight, 0.01f, 0.0f, 1.0f);
	//}
	//if (ImGui::CollapsingHeader("Transform"), ImGuiTreeNodeFlags_DefaultOpen)
	//{
	//	ImGui::DragFloat3("Position", (float*)&mPosition, 0.1f);
	//	ImGui::DragFloat3("Rotation", (float*)&mRotation, 0.01f);

	//}
	ImGui::End();
}

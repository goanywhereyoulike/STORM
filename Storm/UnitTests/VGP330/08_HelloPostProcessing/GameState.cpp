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


	mStandardEffect.Initialize();
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionLight);
	mStandardEffect.UseDisplacementFlag() = true;
	mStandardEffect.UseNorFlag() = true;
	mStandardEffect.UseSpecularFlag() = true;
	mStandardEffect.DisplacementWeight() = 0.3f;

	mTexturingEffect.Initialize();
	mTexturingEffect.SetCamera(mCamera);

	mPostProcessEffect.Initialize();
	mPostProcessEffect.SetMode(PostProcessEffect::Mode::Bloom);
	mPostProcessEffect.SetTexture(&mBaseRenderTarget, 0);
	mPostProcessEffect.SetTexture(&mNoPostProcessEffectRenderTarget, 1);

	mBlockGlitchEffect.Initialize();
	mBlockGlitchEffect.SetMode(BlockGlitchEffect::Mode::Bloom);
	mBlockGlitchEffect.SetTexture(&mBaseRenderTarget, 0);
	mBlockGlitchEffect.SetTexture(&mNoPostProcessEffectRenderTarget, 1);
	//mEarth.diffuseMapId = tm->LoadTexture("../../Assets/Images/earth.jpg");
//mEarth.specularMapId = tm->LoadTexture("../../Assets/Images/earth_spec.jpg");
//mEarth.displacementMapId = tm->LoadTexture("../../Assets/Images/earth_bump.jpg");
//mEarth.normalMapId = tm->LoadTexture("../../Assets/Images/earth_normal.jpg");
	auto tm = TextureManager::Get();

	mEarth.material.ambient = { 1.0f,1.0f,1.0f,1.0f };
	mEarth.material.diffuse = { 0.75f,0.75f,0.75f,1.0f };
	mEarth.material.specular = { 1.0f,1.0f,1.0f,1.0f };
	mEarth.material.power = 10.0f;
	mEarth.diffuseMapId = tm->LoadTexture("../../Assets/Images/textures/planet2.tif");
	mEarth.specularMapId = tm->LoadTexture("../../Assets/Images/textures/planet_gloss.tif");
	mEarth.displacementMapId = tm->LoadTexture("../../Assets/Images/textures/planet_bump.jpg");
	mEarth.normalMapId = tm->LoadTexture("../../Assets/Images/textures/planet_normal.tif");

	mEarth.meshBuffer.Initialize(MeshBuilder::CreateSphere(1024, 1024, 100.0f));

	//mEarthCloud.diffuseMapId = tm->LoadTexture("../../Assets/Images/earth_clouds.jpg");
	mEarthCloud.diffuseMapId = tm->LoadTexture("../../Assets/Images/textures/clouds_diffuse.tif");
	mEarthCloud.specularMapId = tm->LoadTexture("../../Assets/Images/textures/clouds_Gloss.tif");
	mEarthCloud.displacementMapId = tm->LoadTexture("../../Assets/Images/textures/clouds_displace.tif");
	mEarthCloud.normalMapId = tm->LoadTexture("../../Assets/Images/textures/clouds_normal.tif");
	mEarthCloud.meshBuffer.Initialize(MeshBuilder::CreateSpherePX(1024, 1024, 103.0f));
	mEarthCloud.useAdditiveBlend = true;
	Model model;

	//ModelIO::LoadModel("../../Assets/Models/City/city.model", model);
	const char* modelFileName = "../../Assets/Models/City3/city.model";
	ModelIO::LoadModel(modelFileName, model);
	ModelIO::LoadMaterial(modelFileName, model);
	mCity = CreateRenderGroup(model);
	for (auto& part : mCity)
	{
		part.transform.scale *= 0.1f;
	}

	Storm::Graphics::MeshPX mScreenMesh;
	mScreenMesh.vertices.push_back({ {-1.0f, -1.0f, 0.0f },{0.0f,1.0f} });
	mScreenMesh.vertices.push_back({ {-1.0f,  1.0f, 0.0f },{0.0f,0.0f} });
	mScreenMesh.vertices.push_back({ { 1.0f,  1.0f, 0.0f },{1.0f,0.0f} });
	mScreenMesh.vertices.push_back({ { 1.0f, -1.0f, 0.0f },{1.0f,1.0f} });
	mScreenMesh.indices = { 0,1,2,0,2,3 };
	mScreen.meshBuffer.Initialize(mScreenMesh);

	mSkydome.diffuseMapId = tm->LoadTexture("../../Assets/Images/cSky.png");
	mSkydome.meshBuffer.Initialize(MeshBuilder::CreateSkyDome(512, 512, 1000.0f));

	mDirectionLight.direction = Normalize({ 1.0f,-1.0f,1.0f });
	mDirectionLight.ambient = { 0.1f,0.1f,0.1f,1.0f };
	mDirectionLight.diffuse = { 0.75f,0.75f,0.75f,1.0f };
	mDirectionLight.specular = { 0.3f,0.3f,0.3f,1.0f };

	mPosition = { 0.0f,0.0f,-10.0f };

}

void GameState::Terminate()
{
	CleanUpRenderGroup(mCity);
	mScreen.Terminate();
	mEarthCloud.Terminate();
	mEarth.Terminate();
	mSkydome.Terminate();
	mBlockGlitchEffect.Terminate();
	mPostProcessEffect.Terminate();
	mTexturingEffect.Terminate();
	mStandardEffect.Terminate();

	mBaseRenderTarget.Terminate();

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
	const float movespeed = inputSystem->IsKeyDown(KeyCode::LSHIFT) ? 100.0f : 1.0f;
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

	mEarth.transform.position = mPosition;
	mEarth.transform.rotation = Quaternion::RotationEuler(mRotation);

	mEarthCloud.transform.position = mEarth.transform.position;
	mEarthCloud.transform.rotation.y += mCloudRotation;


}
void GameState::Render()
{
	mBaseRenderTarget.BeginRender();
	mStandardEffect.Begin();
	mStandardEffect.Render(mEarth);
	mStandardEffect.RenderGroup(mCity);
	mStandardEffect.End();

	mTexturingEffect.Begin();
	mTexturingEffect.Render(mEarthCloud);
	if (mSkydome.diffuseMapId != 0)
	{
		mTexturingEffect.Render(mSkydome);
	}
	mTexturingEffect.End();


	//SimpleDraw::AddGroundPlane(30, Colors::White);
	SimpleDraw::Render(mCamera);
	mBaseRenderTarget.EndRender();
	//if (UsePostprocessing == false)
	//{
	//	mBlockGlitchEffect.Begin();
	//	mBlockGlitchEffect.Render(mScreen);
	//	mBlockGlitchEffect.End();
	//}
	//else
	{
		mPostProcessEffect.Begin();
		mPostProcessEffect.Render(mScreen);
		mPostProcessEffect.End();
	}
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
		ImGui::DragFloat("Direction X", &mDirectionLight.direction.x, 0.1f);
		ImGui::DragFloat("Direction Y", &mDirectionLight.direction.y, 0.1f);
		ImGui::DragFloat("Direction Z", &mDirectionLight.direction.z, 0.1f);
		//Storm::Math::Normalize(mDirectionLight.direction);

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
		ImGui::DragFloat("Displacment Weight", &mStandardEffect.DisplacementWeight(), 0.01f, 0.0f, 1.0f);
		bool useBlurFlag = (mPostProcessEffect.UseBlurEffect() == 1);
		if (ImGui::Checkbox("Use Blur effect", &useBlurFlag))
		{
			mPostProcessEffect.SetMode(PostProcessEffect::Mode::Blur);
			mPostProcessEffect.UseBlurEffect() = useBlurFlag;
			UsePostprocessing = true;
		}
		ImGui::DragInt("Texture Dimensions", &mPostProcessEffect.TextureDimensions(), 1, 0, 500);
		ImGui::Separator();

		bool useEmbossEffect = (mPostProcessEffect.UseEmbossEffect() == 1);
		if (ImGui::Checkbox("Use Emboss effect", &useEmbossEffect))
		{
			mPostProcessEffect.SetMode(PostProcessEffect::Mode::Emboss);
			mPostProcessEffect.UseEmbossEffect() = useEmbossEffect;
			UsePostprocessing = true;
		}
		ImGui::DragFloat("Emboss amount", &mPostProcessEffect.Embossamount(), 0.1f, 0, 50);

		ImGui::Separator();
		bool useGlitchEffect = (mPostProcessEffect.UseGlitchEffect() == 1);
		if (ImGui::Checkbox("Use Glitch effect", &useGlitchEffect))
		{
			mPostProcessEffect.SetMode(PostProcessEffect::Mode::Glitch);
			mPostProcessEffect.UseGlitchEffect() = useGlitchEffect;
		}
		ImGui::DragFloat("Block Size", &mPostProcessEffect.BlockSize(), 0.5f, 0, 50);
		ImGui::DragFloat("Change Speed", &mPostProcessEffect.Speed(), 0.5f, 0, 50);


	}

	ImGui::PopID();
	//ImGui::PushID("Transform");
	//if (ImGui::CollapsingHeader("Transform"), ImGuiTreeNodeFlags_DefaultOpen)
	//{
	//	ImGui::DragFloat3("Position", (float*)&mPosition, 0.1f);
	//	ImGui::DragFloat3("Rotation", (float*)&mRotation, 0.01f);

	//}
	//ImGui::PopID();


	//ImGui::Separator();
	//if (ImGui::CollapsingHeader("Block Glitch"), ImGuiTreeNodeFlags_DefaultOpen)
	//{
	//	bool useGlitchEffect = (mBlockGlitchEffect.UseGlitchEffect() == 1);
	//	if (ImGui::Checkbox("Use Glitch effect", &useGlitchEffect))
	//	{
	//		mBlockGlitchEffect.SetMode(BlockGlitchEffect::Mode::Glitch);
	//		mBlockGlitchEffect.UseGlitchEffect() = useGlitchEffect;
	//		UsePostprocessing = false;
	//	}
	//	//ImGui::DragFloat("Block_size", &mBlockGlitchEffect.Block_size(), 0.1f, 0, 50);
	//	ImGui::DragFloat("Speed", &mBlockGlitchEffect.Speed(), 0.1f, 0, 50);
	//	ImGui::DragFloat("Fade", &mBlockGlitchEffect.Fade(), 0.1f, 0, 50);
	//	ImGui::DragFloat("Offset", &mBlockGlitchEffect.Offset(), 0.1f, 0, 50);
	//	ImGui::DragFloat("BlockLayer1_U", &mBlockGlitchEffect.BlockLayer1_U(), 0.1f, 0, 50);
	//	ImGui::DragFloat("BlockLayer1_V", &mBlockGlitchEffect.BlockLayer1_V(), 0.1f, 0, 50);
	//	ImGui::DragFloat("BlockLayer2_U", &mBlockGlitchEffect.BlockLayer2_U(), 0.1f, 0, 50);
	//	ImGui::DragFloat("BlockLayer2_V", &mBlockGlitchEffect.BlockLayer2_V(), 0.1f, 0, 50);
	//	ImGui::DragFloat("RGBSplit_Indensity", &mBlockGlitchEffect.RGBSplit_Indensity(), 0.1f, 0, 50);
	//	ImGui::DragFloat("BlockLayer1_Indensity", &mBlockGlitchEffect.BlockLayer1_Indensity(), 0.1f, 0, 50);
	//	ImGui::DragFloat("BlockLayer2_Indensity", &mBlockGlitchEffect.BlockLayer2_Indensity(), 0.1f, 0, 50);


	//}


	ImGui::End();
}

//void GameState::RenderScene()
//{
//	SimpleDraw::AddGroundPlane(30, Colors::White);
//
//	Matrix4 view = mCamera.GetViewMatrix();
//	Matrix4 proj = mCamera.GetProjectionMatrix();
//	Matrix4 world = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y) * Matrix4::RotationZ(mRotation.z) * Matrix4::Translation(mPosition);
//	Matrix4 Cloudsworld = Matrix4::RotationX(mCloudsRotation.x) * Matrix4::RotationY(mCloudsRotation.y) * Matrix4::RotationZ(mCloudsRotation.z) * Matrix4::Translation(mPosition);
//	mTransform.wvp = Transpose(world * view * proj);
//	mTransform.world = Transpose(world);
//	mTransform.viewPosition = mCamera.GetPosition();
//	//Cloud transform
//	mTransform0.wvp = Transpose(Cloudsworld * view * proj);
//	//Matrix4 wvp = Transpose(world * view * proj); // DX always want the transpos
//
//	mTransformBuffer.BindVS(0);
//	mTransformBuffer.BindPS(0);
//	mLightBuffer.BindVS(1);
//	mLightBuffer.BindPS(1);
//	mMaterialBuffer.BindVS(2);
//	mMaterialBuffer.BindPS(2);
//
//	mSettingsBuffer.Update(mSettingsData);
//	mSettingsBuffer.BindPS(3);
//	mSettingsBuffer.BindVS(3);
//
//
//
//	mVertexShader.Bind();
//	mPixelShader.Bind();
//
//	Storm::Graphics::DirectionalLight Light = mDirectionLight;
//	Light.direction = Normalize(Light.direction);
//
//	mTransformBuffer.Update(mTransform);
//	mLightBuffer.Update(Light);
//	mMaterialBuffer.Update(mMaterial);
//
//	mDiffuseMap.BindPS(0);
//	mSpecularMap.BindPS(1);
//	mDisplacementMap.BindVS(2);
//	mNormalMap.BindPS(3);
//
//	mSampler.BindVS(0);
//
//	mSphereMeshBuffer.Render();
//
//	mTexturingVertexShader.Bind();
//	mTexturingPixelShader.Bind();
//	mConstantBuffer.BindVS(0);
//	mConstantBuffer.Update(mTransform0);
//	mSampler.BindPS(0);
//	mCloudsMap.BindPS(0);
//
//	mBlendstate.Set();
//
//	mCloudsSphereMeshBuffer.Render();
//
//	mBlendstate.ClearState();
//
//	mPostProcessBuffer.Update(mPostProcessData);
//	mPostProcessBuffer.BindPS(0);
//	mPostProcessBuffer.BindVS(0);
//
//	SimpleDraw::Render(mCamera);
//
//}
//void GameState::PostProcess()
//{
//	mPostProcessVertexShader.Bind();
//	mPostProcessPixelShader.Bind();
//
//	mSampler.BindPS(0);
//	mRenderTarget.BindPS(0);
//	mScreenMeshBuffer.Render();
//	Texture::UnbindPS(0);
//
//
//}

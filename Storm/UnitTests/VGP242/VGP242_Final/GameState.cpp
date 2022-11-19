#include "GameState.h"

using namespace Storm::Graphics;
using namespace Storm::Input;
using namespace Storm::Math;

void GameState::Initialize()
{
	mCubeMesh = MeshBuilder::CreateCubePX();

	mSkyboxMesh = MeshBuilder::CreateSkyBoxPX();

	mPlaneMesh = MeshBuilder::CreatePlanePX(10, 10, 3.0f);

	mCirclePlaneMesh = MeshBuilder::CreateCirclePlanePX(10, 10.0f);

	mCylinderMesh = MeshBuilder::CreateCylinderPX(20, 4, 2.0f, 5.0f);

	mConeMesh = MeshBuilder::CreateConePX(20, 4, 2.0f, 5.0f);

	mSphereMesh = MeshBuilder::CreateSpherePX(20, 20, 2.0f);

	mSkydomeMesh = MeshBuilder::CreateSkyDome(100, 100, 10.0f);

	// Create vertex buffer
	mCubeMeshBuffer.Initialize(mCubeMesh);

	mSkyboxMeshBuffer.Initialize(mSkyboxMesh);

	mPlaneMeshBuffer.Initialize(mPlaneMesh);

	mCirclePlaneMeshBuffer.Initialize(mCirclePlaneMesh);

	mCylinderMeshBuffer.Initialize(mCylinderMesh);

	mConeMeshBuffer.Initialize(mConeMesh);

	mSphereMeshBuffer.Initialize(mSphereMesh);

	mSkydomeMeshBuffer.Initialize(mSkydomeMesh);
	// Create constant buffer
	mConstantBuffer.Initialize();

	// Compile and create vertex shader
	mVertexShader.Initialize<VertexPX>(L"../../Assets/Shaders/DoTexturing.fx");

	// Compile and create pixel shader
	mPixelShader.Initialize(L"../../Assets/Shaders/DoTexturing.fx");

	mCubeTexture.Initialize(L"../../Assets/Images/Minecraft.png");
	mSkyboxTexture.Initialize(L"../../Assets/Images/skybox.png");
	mPlaneTexture.Initialize(L"../../Assets/Images/Cyberpunk.png");
	mCirclePlaneTexture.Initialize(L"../../Assets/Images/Cyberpunk.png");
	mCylinderTexture.Initialize(L"../../Assets/Images/Cyberpunk.png");
	mConeTexture.Initialize(L"../../Assets/Images/Cyberpunk.png");
	mSphereTexture.Initialize(L"../../Assets/Images/earth.jpg");
	mSkydomeTexture.Initialize(L"../../Assets/Images/skydome1.jpg");

	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void GameState::Terminate()
{

	mSampler.Terminate();
	mCubeTexture.Terminate();
	mSkyboxTexture.Terminate();
	mPlaneTexture.Terminate();
	mCirclePlaneTexture.Terminate();
	mCylinderTexture.Terminate();
	mConeTexture.Terminate();
	mSphereTexture.Terminate();
	mSkydomeTexture.Terminate();

	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mConstantBuffer.Terminate();

	mCubeMeshBuffer.Terminate();
	mSkyboxMeshBuffer.Terminate();
	mPlaneMeshBuffer.Terminate();
	mCirclePlaneMeshBuffer.Terminate();
	mCylinderMeshBuffer.Terminate();
	mConeMeshBuffer.Terminate();
	mSphereMeshBuffer.Terminate();
	mSkydomeMeshBuffer.Terminate();

}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyDown(KeyCode::LEFT))
		angleY += deltaTime;
	if (inputSystem->IsKeyDown(KeyCode::RIGHT))
		angleY -= deltaTime;
	if (inputSystem->IsKeyDown(KeyCode::UP))
		angleX -= deltaTime;
	if (inputSystem->IsKeyDown(KeyCode::DOWN))
		angleX += deltaTime;
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

		float distanceX = static_cast<float>(inputSystem->GetMouseMoveX());
		mCamera.Yaw(distanceX * turnspeed * deltaTime);

		float distanceY = static_cast<float>(inputSystem->GetMouseMoveY());
		mCamera.Pitch(distanceY * turnspeed * deltaTime);

	}



}
void GameState::Render()
{

	Matrix4 view = mCamera.GetViewMatrix();
	Matrix4 proj = mCamera.GetProjectionMatrix();
	//Matrix4 wvp = Transpose(world * view * proj); // DX always want the transpose
	mConstantBuffer.BindVS(0);
	mVertexShader.Bind();
	mPixelShader.Bind();
	mSampler.BindPS(0);

	Matrix4 world = Matrix4::RotationX(angleX) * Matrix4::RotationY(angleY) * Matrix4::Translation({ 0, 0, posZ });
	TransformData data;

	//Draw box
	if (DrawBox)
	{
		data.wvp = Transpose(world * view * proj);
		mConstantBuffer.Update(data);
		mCubeTexture.BindPS(0);
		mCubeMeshBuffer.Render();
	}

	//Draw skybox
	if (DrawSkyBox)
	{
		world = Matrix4::Translation(mCamera.GetPosition());
		data.wvp = Transpose(world * view * proj);
		mConstantBuffer.Update(data);
		mSkyboxTexture.BindPS(0);
		mSkyboxMeshBuffer.Render();
	}

	//Draw plane
	if (DrawPlane)
	{
		world = Matrix4::Translation({ 0, 0, posZ - 5.0f });
		data.wvp = Transpose(world * view * proj);
		mConstantBuffer.Update(data);
		mPlaneTexture.BindPS(0);
		mPlaneMeshBuffer.Render();
	}

	//Draw cylinder
	if (DrawCylinder)
	{
		world = Matrix4::Translation({ 0, 0, posZ });
		data.wvp = Transpose(world * view * proj);
		mConstantBuffer.Update(data);
		mCylinderTexture.BindPS(0);
		mCylinderMeshBuffer.Render();
	}

	//Draw Circleplane
	if (DrawCircleplane)
	{
		world = Matrix4::Translation({ 0, -5.0f, posZ });
		data.wvp = Transpose(world * view * proj);
		mConstantBuffer.Update(data);
		mCirclePlaneTexture.BindPS(0);
		mCirclePlaneMeshBuffer.Render();
	}

	//Draw cone
	if (DrawCone)
	{
		world = Matrix4::Translation({ 0, 0, posZ });
		data.wvp = Transpose(world * view * proj);
		mConstantBuffer.Update(data);
		mConeTexture.BindPS(0);
		mConeMeshBuffer.Render();
	}

	//Draw sphere
	if (DrawSphere)
	{
		world = Matrix4::Translation({ 0, 0, posZ });
		data.wvp = Transpose(world * view * proj);
		mConstantBuffer.Update(data);
		mSphereTexture.BindPS(0);
		mSphereMeshBuffer.Render();
	}

	//Draw skydome
	if (DrawSkydome)
	{
		world = Matrix4::Translation(mCamera.GetPosition());
		data.wvp = Transpose(world * view * proj);
		mConstantBuffer.Update(data);
		mSkydomeTexture.BindPS(0);
		mSkydomeMeshBuffer.Render();
	}
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::CollapsingHeader("Settings"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		ImGui::Checkbox("Draw Box", &DrawBox);
		ImGui::Checkbox("Draw SkyBox", &DrawSkyBox);
		ImGui::Checkbox("Draw Plane", &DrawPlane);
		ImGui::Checkbox("Draw Cylinder", &DrawCylinder);
		ImGui::Checkbox("Draw Circleplane", &DrawCircleplane);
		ImGui::Checkbox("Draw Cone", &DrawCone);
		ImGui::Checkbox("Draw Sphere", &DrawSphere);
		ImGui::Checkbox("Draw Skydome", &DrawSkydome);
	}
	ImGui::End();
}

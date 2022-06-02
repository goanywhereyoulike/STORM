#include "GameState.h"

using namespace Storm;
using namespace Storm::Graphics;
using namespace Storm::Input;
using namespace Storm::Math;
using namespace Storm::Physics;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);
	mCamera.SetPosition(Vector3{ 0.0f,1.0f,-3.0f });
	mCamera.SetLookAt({ 0.0f,1.0f,0.0f });
	mCamera.SetMode(Camera::ProjectionMode::Perspective);
	mCamera.SetSize(40.0f, 20.0f);

	PhysicsWorld::Settings settings
	{
		{ 0.0f,-9.8f,0.0f },
		1.0f / 60.0f,
		10
	};


	mPhysicsWorld.Initialize(settings);
	//mPhysicsWorld.AddPlane({ Vector3::YAxis,0.0f });

	mStandardEffect.Initialize();
	mShadowEffect.Initialize();
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionLight);
	mShadowEffect.SetDirectionalLight(mDirectionLight);

	mDirectionLight.direction = Normalize({ 1.0f,-1.0f,1.0f });
	mDirectionLight.ambient = { 0.1f,0.1f,0.1f,1.0f };
	mDirectionLight.diffuse = { 0.75f,0.75f,0.75f,1.0f };
	mDirectionLight.specular = { 0.3f,0.3f,0.3f,1.0f };

	auto tm = Graphics::TextureManager::Get();
	

	mGround.material.ambient = { 1.0f,1.0f,1.0f,1.0f };
	mGround.material.diffuse = { 0.75f,0.75f,0.75f,1.0f };
	mGround.material.specular = { 1.0f,1.0f,1.0f,1.0f };
	mGround.material.power = 10.0f;
	//mGround.diffuseMap.Initialize("../../Assets/Images/ground.jpg");
	mGround.diffuseMapId = tm->LoadTexture("../../Assets/Images/ground.jpg");
	mGround.meshBuffer.Initialize(MeshBuilder::CreatePlane(20, 20, 5.0f, 10.0f));

	mMesh = MeshBuilder::CreatePlane(10, 10, 0.2f, 1.0f);

	Cloth.material.ambient = { 1.0f,1.0f,1.0f,1.0f };
	Cloth.material.diffuse = { 0.75f,0.75f,0.75f,1.0f };
	Cloth.material.specular = { 1.0f,1.0f,1.0f,1.0f };
	Cloth.material.power = 10.0f;
	Cloth.diffuseMapId= tm->LoadTexture("../../Assets/Images/ground.jpg");
	//Cloth.diffuseMap.Initialize("../../Assets/Images/space.jpg"); 
	Cloth.meshBuffer.Initialize(nullptr,sizeof(mMesh.vertices),mMesh.vertices.size(),true);

	mCullNone.Initialize(RasterizerState::CullMode::None, RasterizerState::FillMode::Solid);
	mWireframe.Initialize(RasterizerState::CullMode::Back, RasterizerState::FillMode::Wireframe);


}

void GameState::Terminate()
{
	Cloth.Terminate();
	mWireframe.Terminate();
	mCullNone.Terminate(); 
	mGround.Terminate();
	CleanUpRenderGroup(mHuman);
	mShadowEffect.Terminate();
	mStandardEffect.Terminate();

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
	mPhysicsWorld.Update(deltaTime * speed);

	Cloth.meshBuffer.Update(mMesh.vertices.data(), mMesh.vertices.size());

}

void GameState::Render()
{
	mPhysicsWorld.DebugDraw();
	//SimpleDraw::AddGroundPlane(10.0f, Colors::LightGray);
	SimpleDraw::AddTransform(Matrix4::Translation({ 0.0f,0.01f,0.0f }));
	SimpleDraw::Render(mCamera);
	mStandardEffect.Begin();
	mStandardEffect.Render(mGround);
	mStandardEffect.Render(Cloth);
	mStandardEffect.End();

}

void GameState::DebugUI()
{
	ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("fps: %f", mFPS);

	if (ImGui::Button("Cloth!"))
	{
		mPhysicsWorld.Clear();
		const size_t rows = 10;
		const size_t columns = 10;
		std::vector<Physics::Particle*> particles;
		for (int i = 0; i < rows; ++i)
		{
			for (int j = 0; j < columns; j++)
			{
				auto p1 = mPhysicsWorld.AddParticle();
				p1->SetPosition({ -3.0f + 0.2f * j,6.0f - 0.2f * i,0.0f });
				p1->radius = 0.02f;
				p1->invMass = 1.0f;

				particles.push_back(p1);
			}
		}
		mPhysicsWorld.AddConstraint <Physics::Fixed>(particles[0]);
		mPhysicsWorld.AddConstraint <Physics::Fixed>(particles[columns - 1]);

		for (uint32_t r = 0; r < rows; ++r)
		{

			for (uint32_t c = 0; c < columns - 1; ++c)
			{
				mPhysicsWorld.AddConstraint<Physics::Spring>(particles[r * columns + c], particles[r * columns + c + 1], 0.2f);
				if (r < rows - 1)
				{
					mPhysicsWorld.AddConstraint<Physics::Spring>(particles[r * columns + c], particles[(r + 1) * columns + c + 1], 0.283f);
				}
			}
		}
		for (int i = 0; i < particles.size() - columns; ++i)
		{
			mPhysicsWorld.AddConstraint<Physics::Spring>(particles[i], particles[i + columns], 0.2f);

		}

		mMesh.vertices.clear();
		for (int i = 0; i < particles.size(); ++i)
		{
			Vertex p{};
			p.position = particles[i]->position;

			mMesh.vertices.push_back(p);
		}
		
	}

	
	ImGui::DragFloat("Speed", &speed, 0.01f, 0.0f, 1.0f);


	if (ImGui::Checkbox("CullNone", &CullNone)|| ImGui::Checkbox("Wireframe", &Wireframe))
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
	ImGui::End();
	mPhysicsWorld.DebugUI();
}




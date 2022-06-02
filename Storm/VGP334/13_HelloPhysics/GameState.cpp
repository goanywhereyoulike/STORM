#include "GameState.h"

using namespace Storm;
using namespace Storm::Graphics;
using namespace Storm::Input;
using namespace Storm::Math;
using namespace Storm::Physics;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Gray);
	mCamera.SetPosition(Vector3{ 0.0f,1.0f,-3.0f });
	mCamera.SetLookAt({ 0.0f,1.0f,0.0f });
	mCamera.SetMode(Camera::ProjectionMode::Perspective);
	mCamera.SetSize(40.0f, 20.0f);
	auto tm = TextureManager::Get();
	mSkydome.diffuseMapId = tm->LoadTexture("../../Assets/Images/space3.jpg");
	mSkydome.meshBuffer.Initialize(MeshBuilder::CreateSkyDome(512, 512, 1000.0f));

	PhysicsWorld::Settings settings
	{
		{ 0.0f,-9.8f,0.0f },
		1.0f / 60.0f,
		0.1f,
		10
	};


	mPhysicsWorld.Initialize(settings);
	//mPhysicsWorld.AddPlane({ Vector3::YAxis,0.0f });
	mPhysicsWorld.AddOBB({
		{0.0f,2.0f,0.0f},
		{3.0f,0.2f,2.0f},
		Quaternion::RotationEuler({0.0f,0.0f,-0.5f}) }
	);
	mPhysicsWorld.AddOBB({
		{5.0f,0.0f,0.0f},
		{8.0f,0.2f,5.0f},
	Quaternion::RotationEuler({0.0f,0.0f,0.0f}) }
	);
	const char* modelFileName = "../../Assets/Models/Duck/duck.model";
	ModelIO::LoadModel(modelFileName, model);
	ModelIO::LoadMaterial(modelFileName, model);
	//ModelIO::LoadSkeleton(modelFileName, model);

	mHuman = CreateRenderGroup(model);
	mHuman[0].transform.scale = Vector3{ 0.1f };
	mStandardEffect.Initialize();
	mShadowEffect.Initialize();
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionLight);
	mShadowEffect.SetDirectionalLight(mDirectionLight);

	mDirectionLight.direction = Normalize({ 1.0f,-1.0f,1.0f });
	mDirectionLight.ambient = { 0.1f,0.1f,0.1f,1.0f };
	mDirectionLight.diffuse = { 0.75f,0.75f,0.75f,1.0f };
	mDirectionLight.specular = { 0.3f,0.3f,0.3f,1.0f };

	mTexturingEffect.Initialize();
	mTexturingEffect.SetCamera(mCamera);

	mCullNone.Initialize(RasterizerState::CullMode::None, RasterizerState::FillMode::Solid);
	mWireframe.Initialize(RasterizerState::CullMode::Back, RasterizerState::FillMode::Wireframe);

}

void GameState::Terminate()
{
	mWireframe.Terminate();
	mCullNone.Terminate();
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
	mPhysicsWorld.Update(deltaTime * speed);

}

void GameState::Render()
{
	mPhysicsWorld.DebugDraw();

	mTexturingEffect.Begin();
	if (mSkydome.diffuseMapId != 0)
	{
		mTexturingEffect.Render(mSkydome);
	}
	mTexturingEffect.End();
	//SimpleDraw::AddGroundPlane(10.0f, Colors::LightGray);
	//SimpleDraw::AddTransform(Matrix4::Translation({ 0.0f,0.01f,0.0f }));
	SimpleDraw::Render(mCamera);
	if (mDrawModel)
	{
		mShadowEffect.Begin();
		for (auto& t : mTetrahedrons)
		{
			auto worldMatrix = t.GetWorldMatrix();
			auto translation = GetTranslation(worldMatrix);
			auto rotation = Quaternion::RotationMatrix(worldMatrix);
			for (auto& robj : mHuman)
			{
				robj.transform.position = translation;
				robj.transform.rotation = rotation;
			}
			mShadowEffect.RenderGroup(mHuman);
		}

		mShadowEffect.End();
		mStandardEffect.Begin();
		for (auto& t : mTetrahedrons)
		{
			auto worldMatrix = t.GetWorldMatrix();
			auto translation = GetTranslation(worldMatrix);
			auto rotation = Quaternion::RotationMatrix(worldMatrix);
			for (auto& robj : mHuman)
			{
				robj.transform.position = translation;
				robj.transform.rotation = rotation;
			}
			mStandardEffect.RenderGroup(mHuman);
		}

		mStandardEffect.End();
	}
}
//namespace
//{
//	float RandomFloat(float min, float max)
//	{
//		return std::uniform_real_distribution<float>{ min, max }(myRandomEngine);
//	}
//}

void GameState::DebugUI()
{
	ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("fps: %f", mFPS);
	if (ImGui::Button("Cloth!"))
	{
		mPhysicsWorld.Clear(true);
		mTetrahedrons.clear();
		const size_t rows = 30;
		const size_t columns = 30;
		std::vector<Physics::Particle*> particles;
		std::vector<int> indices;
		for (int i = 0; i < rows; ++i)
		{
			for (int j = 0; j < columns; j++)
			{
				auto p1 = mPhysicsWorld.AddParticle();
				p1->SetPosition({ 0.0f + 0.2f * j,10.0f - 0.2f * i,0.0f });
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
	}
	//if (ImGui::Button("Boom!"))
	//{
	//	mPhysicsWorld.Clear(true);
	//	mTetrahedrons.clear();
	//	for (int i = 0; i < 100; ++i)
	//	{
	//		Storm::Math::Vector3 vel = Math::Random::OnUnitSphere() * 0.1f;
	//		vel.y = Abs(vel.y);

	//		auto particle = mPhysicsWorld.AddParticle();
	//		particle->SetPosition({ 0.0f,1.0f,0.0f });
	//		particle->SetVelocity(vel);
	//		particle->radius = Math::Random::UniformFloat(1.02f, 1.1f);
	//		particle->invMass = 1.0f;
	//		particle->bounce = Math::Random::UniformFloat(0.5f, 0.95f);

	//	}

	//}
	/*if (ImGui::Button("Sticks!"))
	{
		mPhysicsWorld.Clear(true);
		mTetrahedrons.clear();
		for (int i = 0; i < 100; ++i)
		{
			Storm::Math::Vector3 vel1 = Math::Random::OnUnitSphere() * 0.1f;
			vel1.y = Abs(vel1.y);
			Storm::Math::Vector3 vel2 = Math::Random::OnUnitSphere() * 0.1f;
			vel2.y = Abs(vel2.y);

			auto particle1 = mPhysicsWorld.AddParticle();
			particle1->SetPosition({ -0.1f,1.0f,0.0f });
			particle1->SetVelocity(vel1);
			particle1->radius = 0.02f;
			particle1->invMass = 1.0f;

			auto particle2 = mPhysicsWorld.AddParticle();
			particle2->SetPosition({ 0.1f,1.0f,0.0f });
			particle2->SetVelocity(vel2);
			particle2->radius = 0.02f;
			particle2->invMass = 1.0f;

			mPhysicsWorld.AddConstraint<Physics::Spring>(particle1, particle2);

		}

	}*/
	//if (ImGui::Button("Ball and Chain!"))
	//{
	//	mPhysicsWorld.Clear(true);
	//	mTetrahedrons.clear();
	//	Math::Vector3 position{ 0.0f,3.0f,0.0f };
	//	Physics::Particle* last = nullptr;
	//	for (int i = 0; i < 10; ++i)
	//	{
	//		auto p = mPhysicsWorld.AddParticle();
	//		p->SetPosition(position);
	//		p->SetVelocity(Math::Vector3::Zero);
	//		p->radius = (i == 9) ? 0.5f : 0.02f;
	//		p->invMass = (i == 9) ? 0.1f : 1.0f;
	//		if (i == 0)
	//		{
	//			mPhysicsWorld.AddConstraint<Physics::Fixed>(p);
	//		}
	//		else
	//		{
	//			mPhysicsWorld.AddConstraint<Physics::Spring>(p, last);
	//		}

	//		position.x += 0.5f;
	//		last = p;

	//	}

	//}
	//if (ImGui::Button("Tetrahedron!"))
	//{
	//	mPhysicsWorld.Clear(true);
	//	mTetrahedrons.clear();
	//	for (int i = 0; i < 50; ++i)
	//	{
	//		auto p1 = mPhysicsWorld.AddParticle();
	//		p1->SetPosition({ 0.0f,1.0f,0.0f });
	//		p1->radius = 0.02f;
	//		p1->invMass = 1.0f;

	//		auto p2 = mPhysicsWorld.AddParticle();
	//		p2->SetPosition({ 0.0f,0.8f,0.1f });
	//		p2->radius = 0.02f;
	//		p2->invMass = 1.0f;

	//		auto p3 = mPhysicsWorld.AddParticle();
	//		p3->SetPosition({ 0.1f,0.8f,-0.1f });
	//		p3->radius = 0.02f;
	//		p3->invMass = 1.0f;

	//		auto p4 = mPhysicsWorld.AddParticle();
	//		p4->SetPosition({ -0.1f,0.8f,-0.1f });
	//		p4->radius = 0.02f;
	//		p4->invMass = 1.0f;

	//		Storm::Math::Vector3 vel = Math::Random::OnUnitSphere() * 0.25f;
	//		vel.y = Abs(vel.y);
	//		p1->SetVelocity(vel);

	//		mPhysicsWorld.AddConstraint<Physics::Spring>(p1, p2, 0.2f);
	//		mPhysicsWorld.AddConstraint<Physics::Spring>(p1, p3, 0.2f);
	//		mPhysicsWorld.AddConstraint<Physics::Spring>(p1, p4, 0.2f);
	//		mPhysicsWorld.AddConstraint<Physics::Spring>(p2, p3, 0.2f);
	//		mPhysicsWorld.AddConstraint<Physics::Spring>(p3, p4, 0.2f);
	//		mPhysicsWorld.AddConstraint<Physics::Spring>(p4, p2, 0.2f);

	//	}

	//}

	//if (ImGui::Button("Cube!"))
	//{
	//	mPhysicsWorld.Clear(true);
	//	mTetrahedrons.clear();
	//	for (int i = 0; i < 50; ++i)
	//	{
	//		auto p1 = mPhysicsWorld.AddParticle(0.02f, 1.0f);
	//		auto p2 = mPhysicsWorld.AddParticle(0.02f, 1.0f);
	//		auto p3 = mPhysicsWorld.AddParticle(0.02f, 1.0f);
	//		auto p4 = mPhysicsWorld.AddParticle(0.02f, 1.0f);
	//		auto p5 = mPhysicsWorld.AddParticle(0.02f, 1.0f);
	//		auto p6 = mPhysicsWorld.AddParticle(0.02f, 1.0f);
	//		auto p7 = mPhysicsWorld.AddParticle(0.02f, 1.0f);
	//		auto p8 = mPhysicsWorld.AddParticle(0.02f, 1.0f);
	//		p1->SetPosition({ 0.0f,0.0f,0.0f });
	//		p2->SetPosition({ 1.0f,0.0f,0.0f });
	//		p3->SetPosition({ 0.0f,1.0f,0.0f });
	//		p4->SetPosition({ 1.0f,1.0f,0.0f });
	//		p5->SetPosition({ 0.0f,0.0f,1.0f });
	//		p6->SetPosition({ 1.0f,0.0f,1.0f });
	//		p7->SetPosition({ 0.0f,1.0f,1.0f });
	//		p8->SetPosition({ 1.0f,1.0f,1.0f });


	//		Storm::Math::Vector3 vel = Math::Random::OnUnitSphere() * 0.5f;
	//		vel.y = Abs(vel.y);
	//		p1->SetVelocity(vel);

	//		mPhysicsWorld.AddConstraint<Physics::Spring>(p1, p2, 0.2f);
	//		mPhysicsWorld.AddConstraint<Physics::Spring>(p2, p4, 0.2f);
	//		mPhysicsWorld.AddConstraint<Physics::Spring>(p4, p3, 0.2f);
	//		mPhysicsWorld.AddConstraint<Physics::Spring>(p3, p1, 0.2f);

	//		mPhysicsWorld.AddConstraint<Physics::Spring>(p5, p6, 0.2f);
	//		mPhysicsWorld.AddConstraint<Physics::Spring>(p6, p8, 0.2f);
	//		mPhysicsWorld.AddConstraint<Physics::Spring>(p8, p7, 0.2f);
	//		mPhysicsWorld.AddConstraint<Physics::Spring>(p7, p5, 0.2f);

	//		mPhysicsWorld.AddConstraint<Physics::Spring>(p1, p5, 0.2f);
	//		mPhysicsWorld.AddConstraint<Physics::Spring>(p2, p6, 0.2f);
	//		mPhysicsWorld.AddConstraint<Physics::Spring>(p3, p7, 0.2f);
	//		mPhysicsWorld.AddConstraint<Physics::Spring>(p4, p8, 0.2f);

	//		mPhysicsWorld.AddConstraint<Physics::Spring>(p1, p8, 0.35f);
	//		mPhysicsWorld.AddConstraint<Physics::Spring>(p3, p6, 0.35f);
	//		mPhysicsWorld.AddConstraint<Physics::Spring>(p4, p5, 0.35f);
	//		mPhysicsWorld.AddConstraint<Physics::Spring>(p2, p7, 0.35f);
	//	}

	//}

	if (ImGui::Button("Draw Model Using Tetrahedron!!"))
	{
		mPhysicsWorld.Clear(true);
		mTetrahedrons.clear();
		for (int i = 0; i < 10; ++i)
		{
			auto& t = mTetrahedrons.emplace_back(mPhysicsWorld.AddTetrahedron());
			t.SetPosition({ 0.0f,1.0f,0.0f });
			Vector3 vel = Math::Random::OnUnitSphere() * 0.25f;
			vel.y = Abs(vel.y);
			t.SetVelocity(vel);

			Vector3 vel2 = Math::Random::OnUnitSphere() * 0.25f;
			t.a->SetVelocity(vel2);

		}
		mDrawModel = true;
	}

	//ImGui::DragFloat("Speed", &speed, 0.01f, 0.0f, 1.0f);


	//if (ImGui::Checkbox("CullNone", &CullNone) || ImGui::Checkbox("Wireframe", &Wireframe))
	//{
	//	if (CullNone)
	//	{
	//		mCullNone.Set();
	//	}
	//	else if (Wireframe)
	//	{
	//		mWireframe.Set();
	//	}
	//	else
	//	{
	//		RasterizerState::Clear();
	//	}

	//}
	//if (ImGui::Button("Icosahedron!"))
	//{
	//	mPhysicsWorld.Clear(true);
	//	mTetrahedrons.clear();
	//	mBalls.clear();
	//	Vector3 center = { 0.0f,8.0f,0.0f };

	//	//#define X .525731112119133606
	//	//#define Z .850650808352039932
	//	float X = 0.5;
	//	float Z = 0.8;

	//	static Vector3 vdata[12] = {
	//	   {-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
	//	   {0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
	//	   {Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
	//	};

	//	static int tindices[20][3] = {
	//	   {0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},
	//	   {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},
	//	   {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6},
	//	   {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11} };

	//	for (size_t i = 0; i < 12; i++)
	//	{
	//		auto p = mPhysicsWorld.AddParticle();
	//		p->SetPosition(vdata[i] + center);
	//		p->radius = 0.01f;
	//		mBalls.push_back(p);

	//	}
	//	for (int i = 0; i < 20; i++)
	//	{
	//		for (int k = 0; k < 3; k++)
	//		{
	//			mPhysicsWorld.AddConstraint<Spring>(mBalls[tindices[i][k % 3]], mBalls[tindices[i][(k + 1) % 3]]);

	//		}

	//	}


	//}
	//if (ImGui::Button("Icosahedron2!"))
	//{
	//	mPhysicsWorld.Clear(true);
	//	mTetrahedrons.clear();
	//	mBalls.clear();
	//	Vector3 center = { 0.0f,8.0f,0.0f };
	//	float phi = (1.0f + sqrt(5.0f)) * 0.5f; // golden ratio
	//	float a = 1.0f;
	//	float b = 1.0f / phi;

	//	static Vector3 vdata[12] = {
	//	{0, b, -a  },{b, a, 0   },{-b, a, 0  },{0, b, a   },{0, -b, a  },{-a, 0, b  },
	//	{0, -b, -a },{a, 0, -b  },{a, 0, b   },{-a, 0, -b },{b, -a, 0  },{-b, -a, 0 }
	//	};

	//	static int tindices[20][3] = {
	//	{2, 1, 0},{1, 2, 3},{5, 4, 3},{4, 8, 3},{7, 6, 0},{6, 9,0},
	//	{11,10,4},{10,11,6},{9,5, 2},{5,9,11},{8, 7, 1},{7, 8,10},
	//	{2, 5, 3},{8, 1, 3},{9,2, 0},{1, 7, 0},{11,9,6},{7, 10,6},
	//	{5, 11,4},{10,8, 4} };

	//	//for (size_t i = 0; i < 12; i++)
	//	//{
	//	//	auto p = mPhysicsWorld.AddParticle();
	//	//	p->SetPosition(vdata[i] + center);
	//	//	p->radius = 0.01f;
	//	//	mBalls.push_back(p);

	//	//}
	//	//for (int i = 0; i < 20; i++)
	//	//{
	//	//	for (int k = 0; k < 3; k++)
	//	//	{
	//	//		mPhysicsWorld.AddConstraint<Spring>(mBalls[tindices[i][k % 3]], mBalls[tindices[i][(k + 1) % 3]]);
	//	//	}

	//	//}
	//	auto pCenter = mPhysicsWorld.AddParticle();
	//	pCenter->SetPosition(center);
	//	pCenter->radius = 0.01f;
	//	for (int i = 0; i < 20; i++)
	//	{
	//		subdivide(vdata[tindices[i][0]], vdata[tindices[i][1]], vdata[tindices[i][2]], 2, tindices[i]);
	//	}
	//	for (auto& p : mBalls)
	//	{
	//		p->SetPosition(p->position + center);
	//		mPhysicsWorld.AddConstraint<Spring>(p, pCenter);
	//	}


	//}
	//;
	if (ImGui::Button("Ball!"))
	{
		mPhysicsWorld.Clear(true);
		mTetrahedrons.clear();


		float X = 0.5;
		float Z = 0.8;
		static Vector3 vdata[12] = {
		   {-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
		   {0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
		   {Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
		};
		static int tindices[20][3] = {
		   {0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},
		   {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},
		   {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6},
		   {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11} };

		for (int a = 0; a < 1; ++a)
		{
			mBalls.clear();
			indices.clear();
			Vector3 center = { 0.0f,a * 2 + 8.0f,0.0f };
			auto pCenter = mPhysicsWorld.AddParticle();
			pCenter->SetPosition(center);
			pCenter->radius = 0.01f;
			for (int i = 0; i < 20; i++)
			{
				subdivide(vdata[tindices[i][0]], vdata[tindices[i][1]], vdata[tindices[i][2]], 2, indices);
			}

			for (auto& p : mBalls)
			{
				p->SetPosition(Normalize(p->position) * 1.0f + center);
			}
			for (auto& p : mBalls)
			{
				mPhysicsWorld.AddConstraint<Spring>(p, pCenter);
			}

			for (int i = 0; i < indices.size(); ++i)
			{
				for (int k = 0; k < 3; k++)
				{
					mPhysicsWorld.AddConstraint<Spring>(mBalls[(int)indices[i].x], mBalls[(int)indices[i].y]);
					mPhysicsWorld.AddConstraint<Spring>(mBalls[(int)indices[i].y], mBalls[(int)indices[i].z]);
					mPhysicsWorld.AddConstraint<Spring>(mBalls[(int)indices[i].x], mBalls[(int)indices[i].z]);
				}
			}
		}
	}

	ImGui::End();
	mPhysicsWorld.DebugUI();
}

void GameState::subdivide(Storm::Math::Vector3& v1, Storm::Math::Vector3& v2, Storm::Math::Vector3& v3, long depth, std::vector<Vector3>& indices)
{
	Vector3 v12, v23, v31;
	//Vector3 center= { 0.0f,8.0f,0.0f };

	int i;
	if (0 == depth)
	{
		int index1, index2, index3 = -1;
		Storm::Physics::Particle* p1 = nullptr;
		Storm::Physics::Particle* p2 = nullptr;
		Storm::Physics::Particle* p3 = nullptr;
		for (int i = 0; i < mBalls.size(); ++i)
		{
			auto p = mBalls[i];
			if (MagnitudeSqr(p->position - v1) < 0.01f)
			{
				p1 = p;
				index1 = i;
			}
			if (MagnitudeSqr(p->position - v2) < 0.01f)
			{
				p2 = p;
				index2 = i;
			}
			if (MagnitudeSqr(p->position - v3) < 0.01f)
			{
				p3 = p;
				index3 = i;
			}
		}
		if (p1 == nullptr)
		{
			p1 = mPhysicsWorld.AddParticle();
			p1->SetPosition(v1);
			p1->radius = 0.01f;
			mBalls.push_back(p1);
			index1 = mBalls.size() - 1;
		}
		if (p2 == nullptr)
		{
			p2 = mPhysicsWorld.AddParticle();
			p2->SetPosition(v2);
			p2->radius = 0.01f;
			mBalls.push_back(p2);
			index2 = mBalls.size() - 1;
		}
		if (p3 == nullptr)
		{
			p3 = mPhysicsWorld.AddParticle();
			p3->SetPosition(v3);
			p3->radius = 0.01f;
			mBalls.push_back(p3);
			index3 = mBalls.size() - 1;
		}

		indices.push_back(Vector3{ (float)index1,(float)index2,(float)index3 });
		return;
	}

	v12 = (v1 + v2) / 2.0;
	v23 = (v2 + v3) / 2.0;
	v31 = (v3 + v1) / 2.0;

	v12 = Normalize(v12);
	v23 = Normalize(v23);
	v31 = Normalize(v31);
	subdivide(v1, v12, v31, depth - 1, indices);
	subdivide(v2, v23, v12, depth - 1, indices);
	subdivide(v3, v31, v23, depth - 1, indices);
	subdivide(v12, v23, v31, depth - 1, indices);

}



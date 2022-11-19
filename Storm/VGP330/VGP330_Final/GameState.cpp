#include "GameState.h"

using namespace Storm::Graphics;
using namespace Storm::Input;
using namespace Storm::Math;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);
	mPosition = { 0.0f,2.0f,0.0f };
	mCamera.SetPosition(Vector3{ 0.0f,6.0f,-8.0f });
	mCamera.SetLookAt(mPosition);
	particlepros.textureName = "../../Assets/Images/white.png";
	particlesytem.Initialize(particlepros);
	particlesytem.SetCamera(mCamera);
	mVelocity = { 0.0f,1.0f,0.0f };
	mAcceleration = { 0.0f,0.0f,0.0f };

	//particlepros.velocityvariation = { 3.0f,1.0f ,0.0f };
}

void GameState::Terminate()
{
	particlesytem.Terminate();

}

void GameState::Update(float deltaTime)
{

	auto inputSystem = InputSystem::Get();
	//Camera control---------------------------------
	const float movespeed = inputSystem->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 5.0f;
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
	mTime += deltaTime;

	if (!mPaused)
	{
		for (int i = 0; i < 6; ++i)
		{
			particlesytem.Emit(particlepros);
		}
	}

	particlesytem.SetCamera(mCamera);
	particlesytem.UpdateParticles(mPaused ? 0.0f : deltaTime, particlepros);

}

void GameState::Render()
{
	SimpleDraw::AddGroundPlane(30, Colors::White);
	SimpleDraw::Render(mCamera);

	particlesytem.Render();
}

void GameState::DebugUI()
{
	bool my_tool_active = false;
	ImGui::Begin("Particle System", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	ImGui::BeginMenuBar();
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Load"))
		{
			ImGuiFileDialog::Instance()->OpenDialog("LoadFileDlgKey", "Choose File", ".json", ".");

		}
		if (ImGui::MenuItem("Save"))
		{

			ImGuiFileDialog::Instance()->OpenDialog("SaveFileDlgKey", "Choose File", nullptr, ".");

		}
		ImGui::EndMenu();
	}
	ImGui::EndMenuBar();

	// display
	if (ImGuiFileDialog::Instance()->Display("LoadFileDlgKey"))
	{
		// action if OK
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();

			particlesytem.LoadParticleProps(filePathName, particlepros);
			particlesytem.Terminate();
			particlesytem.Initialize(particlepros);
			particlesytem.SetCamera(mCamera);
			// action
		}

		// close
		ImGuiFileDialog::Instance()->Close();
	}
	if (ImGuiFileDialog::Instance()->Display("SaveFileDlgKey"))
	{
		// action if OK
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
			particlesytem.SaveParticleProps(filePath, particlepros);
			// action
		}

		// close
		ImGuiFileDialog::Instance()->Close();
	}


	//ImGui::Text("fps: %f", mFPS);
	if (ImGui::CollapsingHeader("Emitter Properties"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		ImGui::RadioButton("Rectangle", &particlepros.EmitterShape, 0); ImGui::SameLine();
		ImGui::RadioButton("Sphere", &particlepros.EmitterShape, 1);
		ImGui::DragFloat3("Emit Position Offset", (float*)&particlepros.mEmitPosOffset, 0.1f, 0.0f, 10.0f);
		ImGui::DragInt("Particle Number", (int*)&particlepros.number, 1, 0, 5000);
		ImGui::DragFloat3("Emitter Position", (float*)&particlepros.position, 0.1f);
		ImGui::Checkbox("Use Destination", &particlepros.UseDestination);
		ImGui::DragFloat3("Destination", (float*)&particlepros.destination, 0.1f);
	}
	if (ImGui::CollapsingHeader("Particle Properties"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		bool randomColor = false;
		ImGui::DragFloat("Lifetime", (float*)&particlepros.lifetime, 0.1f, 0.0f, 10.0f);
		ImGui::DragFloat("Size", (float*)&particlepros.size, 0.01f, 0.0f, 10.0f);
		ImGui::DragFloat3("Velocity", (float*)&particlepros.velocity, 0.1f);
		ImGui::DragFloat3("Velocity Variation", (float*)&particlepros.velocityvariation, 0.1f);
		if (!ImGui::Checkbox("Use Random color", &particlepros.UseRandomColor))
		{
			ImGui::ColorEdit4("StartColor ", (float*)&particlepros.startColor);
			ImGui::ColorEdit4("EndColor ", (float*)&particlepros.endColor);
		}
	}
	ImGui::Separator();
	ImGui::Text("Particle Texture");
	auto ParticleTexture = particlesytem.GetParticleTexture();
	ImGui::Image(ParticleTexture->GetRawData(), { 200,144 });
	ImGui::Separator();
	ImGui::Checkbox("Pause", &mPaused);

	ImGui::End();
}




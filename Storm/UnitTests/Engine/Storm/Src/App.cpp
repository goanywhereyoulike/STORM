#include "Precompiled.h"
#include "App.h"
#include "AppState.h"

using namespace Storm;
using namespace Storm::Core;
using namespace Storm::Graphics;
using namespace Storm::Input;
void App::ChangeState(const std::string& statename)
{
	auto iter = mAppStates.find(statename);
	if (iter != mAppStates.end())
	{
		mNextState = iter->second.get();
	}

}
void App::Run(const AppConfig& appConfig)
{
	Window myWindow;
	myWindow.Initialize(GetModuleHandle(nullptr), appConfig.appName.c_str(), appConfig.windowWidth, appConfig.windowHeight);

	auto handle = myWindow.GetWindowHandle();
	InputSystem::StaticInitialize(handle);
	GraphicsSystem::StaticInitialize(handle, false);
	SimpleDraw::StaticInitialize(appConfig.debugDrawLimit);
	DebugUI::StaticInitialize(handle, false, true);
	ModelManager::StaticInitialize();
	TextureManager::StaticInitialize();
	//auto graphicsSystem = Graphics::GraphicsSystem::Get();
	//graphicsSystem->SetClearColor(Graphics::Colors::Chocolate);
	ASSERT(mCurrentState, "App -- No app state found! App must have at least one AppState.");
	mCurrentState->Initialize();


	mRunning = true;
	while (mRunning)
	{


		myWindow.ProcessMessage();
		if (!myWindow.IsActive())
		{
			Quit();
			continue;
		}
		auto inputSystem=InputSystem::Get();
		inputSystem->Update();
	

		if (appConfig.useEscToQuit && inputSystem->IsKeyPressed(KeyCode::ESCAPE))
		{
			Quit();
			continue;
		}

		if (mNextState)
		{
			mCurrentState->Terminate();
			mCurrentState = std::exchange(mNextState, nullptr);
			mCurrentState->Initialize();

		}

		auto deltaTime = TimeUtil::GetDeltaTime();
		mCurrentState->Update(deltaTime);
		auto graphicsSystem = GraphicsSystem::Get();
		graphicsSystem->BeginRender();

		mCurrentState->Render();
		DebugUI::BeginRender();
		mCurrentState->DebugUI();
		DebugUI::EndRender();

		graphicsSystem->EndRender();

	}

	//clean up current state
	mCurrentState->Terminate();
	TextureManager::StaticTerminate();
	ModelManager::StaticTerminate();
	DebugUI::StaticTerminate();
	SimpleDraw::StaticTerminate();
	GraphicsSystem::StaticTerminate();
	InputSystem::StaticTerminate();

	myWindow.Terminate();

}
void App::Quit()
{
	mRunning = false;

}
#include "GameState.h"


using namespace Storm::Graphics;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Aqua);

}

void GameState::Update(float deltaTime)
{
	auto graphicsSystem = GraphicsSystem::Get();
	if (GetAsyncKeyState('1'))
	{
		graphicsSystem->SetClearColor(Colors::BlueViolet);

	}
	if (GetAsyncKeyState('2'))
	{
		graphicsSystem->SetClearColor(Colors::DarkOrange);
	}

}

#include "GameState.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	using namespace Storm;
	MainApp().AddState<GameState>("Game");
	MainApp().Run({ L"Hello Model" });

	return 0;
}
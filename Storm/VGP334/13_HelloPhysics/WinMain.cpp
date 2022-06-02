#include "GameState.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	using namespace Storm;

	AppConfig appConfig;
	appConfig.appName = L"Hello Physics";
	//appConfig.debugDrawLimit = 500000;

	MainApp().AddState<GameState>("Game");
	MainApp().Run(appConfig);

	return 0;
}
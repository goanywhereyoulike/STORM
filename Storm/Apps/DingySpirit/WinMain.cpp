#include "GameState.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	using namespace Storm;

	AppConfig appConfig;
	appConfig.appName = L"Dingy Soirit";
	//appConfig.debugDrawLimit = 500000;

	MainApp().AddState<GameState>("Game");
	MainApp().Run(appConfig);

	return 0;
}
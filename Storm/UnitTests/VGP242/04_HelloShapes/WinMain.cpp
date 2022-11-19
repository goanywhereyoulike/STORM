#include "GameState.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	using namespace Storm;

	//AppConfig appConfig;
	//appConfig.appName = L"Hello Window";
	MainApp().AddState<GameState>("Game");
	MainApp().Run({ L"Hello Shapes" });


	return 0;
}
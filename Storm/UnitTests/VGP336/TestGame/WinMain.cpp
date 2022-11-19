#include "GameState.h"
//void* operator new(size_t size)
//{
//	LOG("Allocating %uz bytes.", size);
//	void* mem = malloc(size);
//	return mem;
//}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	using namespace Storm;

	AppConfig appConfig;
	appConfig.appName = L"Test Game";
	//appConfig.debugDrawLimit = 500000;

	MainApp().AddState<GameState>("Game");
	MainApp().Run(appConfig);

	return 0;
}
#include <Core/Inc/Core.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//Storm::Core::TimeUtil::Initialize();
	LOG("Hello, I am %s.", "Peter");
	Sleep(1000);
	LOG("%d seconds have passed and I am still fine.",1);



	const int n = 42;
	//ASSERT(n == 7, "42 is not 7.");

	return 0;
}
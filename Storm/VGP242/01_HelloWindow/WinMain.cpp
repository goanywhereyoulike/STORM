#include <Core/Inc/Core.h>

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	Storm::Core::Window myWindow;
	myWindow.Initialize(instance, L"Hello Window", 800, 600);
	bool running = true;
	while (true)
	{
		myWindow.ProcessMessage();
		if (!myWindow.IsActive())
		{
			break;
		}

		if (GetAsyncKeyState(VK_ESCAPE))
		{
			break;
		}
	}

	myWindow.Terminate();
	return 0;
}
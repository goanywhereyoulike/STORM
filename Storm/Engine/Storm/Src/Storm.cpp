#include "Precompiled.h"
#include "Storm.h"

Storm::App& Storm::MainApp()
{
	static App sApp;
	return sApp;

}
#pragma once
namespace Storm::Core::TimeUtil
{
	//This function returns the time since the first time you call it.
	float GetTime();

	//This function returns the time difference since you last called it.
	float GetDeltaTime();

}
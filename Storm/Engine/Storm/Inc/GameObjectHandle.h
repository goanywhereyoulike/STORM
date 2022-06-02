#pragma once
#include "Common.h"

namespace Storm
{

	class GameObjectHandle
	{

	public:
		GameObjectHandle() = default;

	private:
		friend class GameWorld;

		int mIndex = -1;
		int mGeneration = -1;
	};

}
#pragma once
#include "Component.h"

namespace Storm
{
	class TransformComponent final 
		:public Component,
		public Graphics::Transform
	{
	public:
		SET_TYPE_ID(ComponentId::Transform);
		MEMORY_POOL_DECLARE;

		void DebugUI() override;
	};

}
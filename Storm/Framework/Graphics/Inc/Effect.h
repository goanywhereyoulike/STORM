#pragma once

#include "Common.h"
#include "RenderObject.h"

namespace Storm::Graphics
{
	class RenderObject;
	class Effect
	{
	public:
		Effect() = default;
		virtual ~Effect() = default;

		virtual void Initialize() = 0;
		virtual void Terminate() = 0;

		virtual void Begin() = 0;
		virtual void End() = 0;

		void RenderGroup(const RenderGroup& renderGroup)
		{
			for (auto& renderObject : renderGroup)
			{
				Render(renderObject); 
			}
		}
		virtual void Render(const RenderObject& renderObject) = 0;

	};



}
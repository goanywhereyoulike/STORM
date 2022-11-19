#pragma once
#include "Colors.h"
namespace Storm::Graphics
{
	struct Material
	{
		Color ambient;
		Color diffuse;
		Color specular;
		float power = 0.0f;
		float padding[3] = { 0.0f };
	};

}
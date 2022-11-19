#include "Precompiled.h"
#include "TransformComponent.h"

using namespace Storm;
MEMORY_POOL_DEFINE(TransformComponent, 1000);
void TransformComponent::DebugUI()
{
	Graphics::SimpleDraw::AddTransform(GetMatrix4());
}

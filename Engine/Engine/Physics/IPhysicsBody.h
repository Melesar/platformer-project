#pragma once
#include "Core.h"

namespace Engine
{
	class ENGINE_API IPhysicsBody
	{
	public:
		virtual const BoundingBox& getBoundingBox() const = 0;
	};
}

#pragma once
#include <Core.h>

namespace Engine
{
	class ENGINE_API IRenderable
	{
	public:
		virtual void Render() const = 0;
	};
}
#pragma once
#include <Core.h>

namespace Engine
{
	class ENGINE_API IRenderable
	{
	public:
		virtual void render() const = 0;
		virtual int sortingOrder() const = 0;
		virtual void setViewMatrix(const glm::mat3x3& matrix) = 0;
	};
}
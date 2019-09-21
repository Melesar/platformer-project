#pragma once
#include "Core.h"

namespace Engine
{
	struct ENGINE_API Color
	{
		float r, g, b, a;

		Color(float r, float g, float b, float a)
			:r(r), g(g), b(b), a(a) {}

		Color() : r(0.f), g(0.f), b(0.f), a(0.f) {}
	};
}
#pragma once
#include "Core.h"

namespace Engine
{
	class ENGINE_API Time
	{
	public:
		void update();
		
		float delta();

	private:
		unsigned int _lastTick;
	};
}
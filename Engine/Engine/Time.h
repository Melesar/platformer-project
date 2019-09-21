#pragma once
#include "Core.h"

namespace Engine
{
	class ENGINE_API Time
	{
	public:
		void Update();
		
		float delta();

	private:
		unsigned int _lastTick;
	};
}
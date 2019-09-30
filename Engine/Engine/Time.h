#pragma once
#include "Core.h"

namespace Engine
{
	class ENGINE_API Time
	{
	public:
		void update();
		void init();
		
		float delta() const;

	private:
		unsigned int _lastTick;
		float _delta;
	};
}
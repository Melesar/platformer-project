#include "Core.h"
#include "Time.h"

void Engine::Time::update()
{
	_lastTick = SDL_GetTicks();
}

float Engine::Time::delta() const
{
	return static_cast<float>(SDL_GetTicks() - _lastTick) / 1000.0f;
}
